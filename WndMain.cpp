#include "WndMain.h"
#include "ui_WndMain.h"

WndMain::WndMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WndMain),
    _settings("RonenL", "rpiweatherd-qtclient")
{
    ui->setupUi(this);

    // Load standard icons
    ui->pbtnRefresh->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    ui->pbtnSettings->setIcon(style()->standardIcon(QStyle::SP_FileDialogListView));

    // Check if we need first configuration
    if (!_settings.value(CONFIG_FIRST_CONFIG, false).toBool())
        on_pbtnSettings_clicked();
    else {
        // Initialize connection manager
        connManager = new RequestManager(_settings.value(CONFIG_SERVER_IP).toString(),
                                         _settings.value(CONFIG_SERVER_PORT).toInt(),
                                         this);
        QObject::connect(connManager, &RequestManager::configRequestFinished,
                         this, &WndMain::configReplyFinished);
        QObject::connect(connManager, &RequestManager::currentRequestFinished,
                         this, &WndMain::currentReplyFinished);

        // Synchronously: Do configuration
        connManager->getConfiguration();

        // On separate threads:
        // 1. Get current measurement

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

void WndMain::configReplyFinished(QNetworkReply *reply) {
    // Check reply status
    if (reply->error() == QNetworkReply::NoError) {
        // Get reply and jsonify it
        QString strReply = QString(reply->readAll());
        QJsonDocument jsonDoc = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject results = jsonDoc.object()["results"].toObject();

        // Save configuration
        _settings.setValue(CONFIG_SERVER_UNIT, results["units"].toString());
        _settings.setValue(CONFIG_MEASURE_LOCATION, results["measurement_location"].toString());

        qDebug() << "Configuration settings saved.";

        // Delete the reply later
        reply->deleteLater();

        // Set flag so that main initialization can continue
        _flag = true;
    }
    else {
        QMessageBox::question(this, tr("Error receiving server configuration"),
                                    tr("An error occurred while trying to receive server" \
                                       "configuration: %1."),
                                    QMessageBox::Cancel);
    }
}

void WndMain::currentReplyFinished(QNetworkReply *reply) {
    qDebug() << reply->readAll();

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
