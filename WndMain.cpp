#include "WndMain.h"
#include "ui_WndMain.h"

WndMain::WndMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WndMain)
{
    ui->setupUi(this);

    // Load standard icons
    ui->pbtnRefresh->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    ui->pbtnSettings->setIcon(style()->standardIcon(QStyle::SP_FileDialogListView));

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

        // Get configuration
        QEventLoop loop;
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

        // Synchronously on the second thread:
        // 2. Get location - store in cache
        // 2. Get sunset-sundown times
        // 2. Set background colors
    }
}

WndMain::~WndMain()
{
    delete ui;
}

void WndMain::showServerErrorMessage(int retcode, QString msg) {
    QMessageBox::critical(this, tr("Server Error"), tr("Server error %1:\n%2")
                          .arg(retcode).arg(msg), QMessageBox::Ok);
}

void WndMain::configReplyFinished(QNetworkReply *reply) {
    // Check reply status
    if (reply->error() == QNetworkReply::NoError) {
        // Get reply and jsonify it
        QString strReply = QString(reply->readAll());
        QJsonDocument jsonDoc = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject mainObject = jsonDoc.object();
        QJsonObject results = mainObject["results"].toObject();

        // Check error code
        int errcode = mainObject["errcode"].toInt();
        if (errcode != 0) {
            showServerErrorMessage(errcode, mainObject["errmsg"].toString());
            return;
        }

        // Save configuration
        ConfigurationManager::instance().setValue(CONFIG_SERVER_UNIT, results["units"].toString());
        ConfigurationManager::instance().setValue(CONFIG_MEASURE_LOCATION,
                                                  results["measurement_location"].toString());

        qDebug() << "Configuration settings saved.";        
    }
    else {
        QMessageBox::question(this, tr("Error receiving server configuration"),
                                    tr("An error occurred while trying to receive server" \
                                       "configuration:\n%1.").arg(reply->errorString()),
                                    QMessageBox::Cancel);
    }

    // Delete the reply later
    reply->deleteLater();
}

void WndMain::currentReplyFinished(QNetworkReply *reply) {
    // Check reply status
    if (reply->error() == QNetworkReply::NoError) {
        // Get reply and jsonify it
        QString strReply = QString(reply->readAll());
        QJsonDocument jsonDoc = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject mainObject = jsonDoc.object();
        QJsonObject results = mainObject["-1"].toObject();
        float temperature = results["temperature"].toDouble(),
              humidity = results["humidity"].toDouble();
        char mUnit;

        // Check error code
        int errcode = mainObject["errcode"].toInt();
        if (errcode != 0) {
            showServerErrorMessage(errcode, mainObject["errmsg"].toString());
            return;
        }

        // Get temperature both in F and C.
        // This is needed to calculate the heat index and the humidex, respectivly.
        float tempC = Utils::convertTemperatureIfNeeded(temperature,
            ConfigurationManager::instance().value(CONFIG_SERVER_UNIT).toString(),
            QString(RPIWEATHERD_UNITS_METRIC));
        float tempF = Utils::convertTemperatureIfNeeded(temperature,
            ConfigurationManager::instance().value(CONFIG_SERVER_UNIT).toString(),
            QString(RPIWEATHERD_UNITS_IMPERIAL));

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
        QMessageBox::question(this, tr("Error receiving current measurement"),
                                    tr("An error occurred while trying to receive " \
                                       "the current measurement:\n%1.").arg(reply->errorString()),
                                    QMessageBox::Cancel);
    }

    // Delete the reply later
    reply->deleteLater();
}

void WndMain::locationCoordinatesReplyFinished(QNetworkReply *reply) {
    qDebug() << reply->readAll();

    // Delete the reply later
    reply->deleteLater();
}

void WndMain::on_pbtnSettings_clicked()
{
    // Show settings UI.
    // If accepted, save settings and reload view.
    DlgConfiguration *config = new DlgConfiguration(this);
    int result = config->exec();

    // If accepted, reload all data
    if (result == QDialog::Accepted) {
        ;
        // TODO: Reload data
    }
}
