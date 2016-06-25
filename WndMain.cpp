#include "WndMain.h"
#include "ui_WndMain.h"

WndMain::WndMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WndMain),
    skyColorMap(nullptr), sunColorMap(nullptr),
    _locationChanged(false)
{
    ui->setupUi(this);

    // Load standard icons
    ui->pbtnRefresh->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    ui->pbtnSettings->setIcon(style()->standardIcon(QStyle::SP_FileDialogListView));

    // Register F1 shortcut to about dialog
    QShortcut *f1AboutShortcut = new QShortcut(QKeySequence(tr("F1", "About...")), this);
    QObject::connect(f1AboutShortcut, &QShortcut::activated, this, &WndMain::on_f1AboutShortcut);

    // Check if we need first configuration
    if (!ConfigurationManager::instance().value(CONFIG_FIRST_CONFIG, false).toBool())
        on_pbtnSettings_clicked();
    else {
        // Initialize connection manager and connect signals
        connManager = new RequestManager(
                    ConfigurationManager::instance().value(CONFIG_SERVER_IP).toString(),
                    ConfigurationManager::instance().value(CONFIG_SERVER_PORT).toInt(),
                    this);
        QObject::connect(connManager, &RequestManager::configRequestFinished,
                         this, &WndMain::configReplyFinished);
        QObject::connect(connManager, &RequestManager::currentRequestFinished,
                         this, &WndMain::currentReplyFinished);
        QObject::connect(connManager, &RequestManager::locationCoordinatesRequestFinished,
                         this, &WndMain::locationCoordinatesReplyFinished);
        QObject::connect(connManager, &RequestManager::sunsetSunriseTimesRequestFinished,
                         this, &WndMain::sunsetSunriseReplyFinished);

        // Initiate procedure
        fetchProcedure();
    }
}

WndMain::~WndMain() {
    delete ui;
}

void WndMain::fetchProcedure() {
    QEventLoop loop;

    // Reset all UIs
    this->resetState();
    ui->wgIndexDisplay->resetState();
    ui->wgMeasureDisplay->resetState();

    // Get configuration
    QNetworkReply *configReply = connManager->getConfiguration();
    QObject::connect(configReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QObject::disconnect(configReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    // Getting the configuration should work. If not, do not do anything else.
    if (configReply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(this, tr("Configuration Error"), tr("Can not proceed."),
                              QMessageBox::Ok);
        return;
    }

    // Get current measurement
    connManager->getCurrentMeasurement();

    // If the location has been changed, get all data again.
    if (ConfigurationManager::instance().value(CONFIG_ENABLE_DYNAMIC_BACKGROUND).toBool()) {
        if (_locationChanged) {
            // Get location if needed - then store in cache.
            QNetworkReply *coordsReply = connManager->getLocationCoordinates();
            QObject::connect(coordsReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();
            QObject::disconnect(coordsReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

            // Get sunset-sundown times
            QNetworkReply *sunsetSunriseReply = connManager->getSunsetSunriseTimes();
            QObject::connect(sunsetSunriseReply, &QNetworkReply::finished, &loop,
                             &QEventLoop::quit);
            loop.exec();
            QObject::disconnect(sunsetSunriseReply, &QNetworkReply::finished, &loop,
                                &QEventLoop::quit);
        }

        // Sunrise and sunset times are required to load the phase maps.
        QTime sunriseTime = ConfigurationManager::instance().value(CONFIG_SUNRISE_TIME).toTime(),
                sunsetTime = ConfigurationManager::instance().value(CONFIG_SUNSET_TIME).toTime();

        // Check if maps are already loaded. If they are, delete them and re-load.
        if (skyColorMap != nullptr && sunColorMap != nullptr) {
            QObject::disconnect(skyColorMap, &PhaseImageMap::phaseChaged, this,
                                &WndMain::changeTextColor);

            delete skyColorMap;
            delete sunColorMap;

            skyColorMap = nullptr;
            sunColorMap = nullptr;
        }

        // Load maps
        skyColorMap = new PhaseImageMap(":/maps/res/SkyColorMap.png", sunriseTime, sunsetTime,
                                        67, this);
        sunColorMap = new PhaseImageMap(":/maps/res/SunColorMap.png", sunriseTime, sunsetTime,
                                        67, this);

        // Check if the maps were loaded
        if (!skyColorMap->imageLoaded() || !sunColorMap->imageLoaded())
            QMessageBox::critical(this, tr("Failed to load phase images"),
                                  tr("Failed to load phase images.\n" \
                                     "Live background feature is disabled."),
                                  QMessageBox::Ok);
        else
            QObject::connect(skyColorMap, &PhaseImageMap::phaseChaged, this,
                             &WndMain::changeTextColor);
    }

    // Repaint just in case
    this->repaint();
}

void WndMain::showServerErrorMessage(int retcode, QString msg) {
    QMessageBox::critical(this, tr("Server Error"), tr("Server error %1:\n%2")
                          .arg(retcode).arg(msg), QMessageBox::Ok);
}

void WndMain::resetState() {
    // Remove stylesheet
    this->setStyleSheet("");
}

void WndMain::changeTextColor() {
    // Get application palette
    QPalette appPalette = QApplication::palette();

    // Determine text color
    if (skyColorMap->getCurrentPhaseColor().lightness() <
            appPalette.color(QPalette::WindowText).lightness())
        ui->wgMeasureDisplay->setTextColor(appPalette.color(QPalette::BrightText));
    else
        ui->wgMeasureDisplay->setTextColor(appPalette.color(QPalette::WindowText));
}

void WndMain::configReplyFinished(QNetworkReply *reply) {
    // Read the json object
    QJsonObject jsonObject = connManager->readJsonObject(reply);

    // Check reply status
    if (!jsonObject.empty()) {
        // Get reply and jsonify it
        QJsonObject results = jsonObject["results"].toObject();

        // Save configuration
        ConfigurationManager::instance().setValue(CONFIG_SERVER_UNIT, results["units"].toString());

        // Check if the location has been changed.
        // If it did, set the flag and store new value.
        // If not, do not store new value.
        if (ConfigurationManager::instance().value(CONFIG_MEASURE_LOCATION).toString() !=
                results["measure_location"].toString()) {
            ConfigurationManager::instance().setValue(CONFIG_MEASURE_LOCATION,
                                                  results["measure_location"].toString());
            _locationChanged = true;
        }

        // Flush settings
        ConfigurationManager::instance().settings().sync();
    }
    else {
        QMessageBox::question(this, tr("Error receiving server configuration"),
                                    tr("An error occurred while trying to receive server " \
                                       "configuration:\n%1.").arg(reply->errorString()),
                                    QMessageBox::Cancel);
    }

    // Delete the reply later
    reply->deleteLater();
}

void WndMain::currentReplyFinished(QNetworkReply *reply) {
    // Read the json object
    QJsonObject jsonObject = connManager->readJsonObject(reply);

    // Check reply status
    if (!jsonObject.empty()) {
        // Get reply and jsonify it
        QJsonObject results = jsonObject["-1"].toObject();
        float temperature = results["temperature"].toDouble(),
              humidity = results["humidity"].toDouble();
        char mUnit;

        // Get temperature in Celsius, for humidex calculations.
        float tempC = Utils::convertTemperatureIfNeeded(temperature,
            ConfigurationManager::instance().value(CONFIG_SERVER_UNIT).toString(),
            QString(RPIWEATHERD_UNITS_METRIC));

        // Get unit to use.
        QString strUnit = ConfigurationManager::instance().value(CONFIG_PREFERRED_UNIT).toString();
        mUnit = Utils::unitDescriptionToUnitChar(strUnit);

        // Put measurement details in control.
        ui->wgMeasureDisplay->setMeasurementDetails(QDate::currentDate(),
            Utils::convertTemperatureIfNeeded(temperature),
            mUnit, humidity);

        // Calculate humidex
        ui->wgIndexDisplay->setValue(Utils::calculateHumidex(tempC, humidity));
    }
    else {
        QMessageBox::critical(this, tr("Error receiving current measurement"),
                                    tr("An error occurred while trying to receive " \
                                       "the current measurement:\n%1.").arg(reply->errorString()),
                                    QMessageBox::Ok);
    }

    // Delete the reply later
    reply->deleteLater();
}

void WndMain::locationCoordinatesReplyFinished(QNetworkReply *reply) {
    // Read JSON data
    QJsonObject jsonObject = connManager->readJsonObject(reply);
    QJsonArray results = jsonObject["results"].toArray();

    if (!jsonObject.isEmpty() || !results.isEmpty()) {
        QJsonObject coords = results[0].toObject()["geometry"].toObject()["location"].toObject();

        // Build string to save
        QString coordString = Utils::buildCoordinatesString(
                    ConfigurationManager::instance().value(CONFIG_MEASURE_LOCATION).toString(),
                    coords["lat"].toDouble(), coords["lng"].toDouble());

        // Save to settings
        ConfigurationManager::instance().setValue(CONFIG_LOCATION_COORDINATES, coordString);
        ConfigurationManager::instance().settings().sync();
    }
    else
        QMessageBox::critical(this, tr("Error retrieving location coordinates"),
                                    tr("Error from Google Maps:\n%1")
                                       .arg(jsonObject["error_message"].toString()),
                              QMessageBox::Ok);

    // Delete the reply later
    reply->deleteLater();

    // Sync settings
    ConfigurationManager::instance().settings().sync();
}

void WndMain::sunsetSunriseReplyFinished(QNetworkReply *reply) {
    // Read JSON data
    QJsonObject jsonObject = connManager->readJsonObject(reply);
    QJsonObject results = jsonObject["results"].toObject();

    // Get the UTC time and convert to local time.
    QDateTime sunriseDate = QDateTime::fromString(results["sunrise"].toString(), Qt::ISODate),
              sunsetDate = QDateTime::fromString(results["sunset"].toString(), Qt::ISODate);
    sunriseDate.setTimeSpec(Qt::UTC);
    sunsetDate.setTimeSpec(Qt::UTC);

    sunriseDate = sunriseDate.toLocalTime();
    sunsetDate = sunsetDate.toLocalTime();

    // Store in configuration
    ConfigurationManager::instance().setValue(CONFIG_SUNRISE_TIME, sunriseDate.time());
    ConfigurationManager::instance().setValue(CONFIG_SUNSET_TIME, sunsetDate.time());

    // Delete the reply later
    reply->deleteLater();

    // Sync settings
    ConfigurationManager::instance().settings().sync();
}

void WndMain::on_pbtnSettings_clicked()
{
    // Show settings UI.
    // If accepted, save settings and reload view.
    DlgConfiguration *config = new DlgConfiguration(this);
    int result = config->exec();

    // If accepted, reload all data
    if (result == QDialog::Accepted) {
        // Reload data
        fetchProcedure();
    }
}

void WndMain::on_pbtnRefresh_clicked() {
    fetchProcedure();
}

void WndMain::on_f1AboutShortcut() {
    DlgConfiguration *config = new DlgConfiguration(this);
    config->showAboutTab();
    config->exec();
}

void WndMain::paintEvent(QPaintEvent *ev) {
    Q_UNUSED(ev);

    // All of this should only be done if dynamic background is enabled.
    if (ConfigurationManager::instance().value(CONFIG_ENABLE_DYNAMIC_BACKGROUND).toBool()) {
        // This check is needed for an edge case - if the program is run without dynamic background,
        // and it is then enabled, the paint event happens before the maps are loaded
        // so a segfault occurres.
        if (sunColorMap && skyColorMap) {
            // Get painter
            QPainter painter(this);

            // Get current time
            QTime currTime = QTime::currentTime();

            // Create the gradient
            QRadialGradient radialGrad(QPointF(0, 0), this->width());
            radialGrad.setColorAt(0, sunColorMap->getPhaseColor(currTime));
            radialGrad.setColorAt(0.8, skyColorMap->getPhaseColor(currTime));

            // Draw gradient
            painter.fillRect(ui->centralWidget->rect(), radialGrad);
        }
    }
}
