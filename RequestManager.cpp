#include "RequestManager.h"

RequestManager::RequestManager(QString hostName, int port, QObject *parent)
    : QObject(parent) {
    _hostName = hostName;
    _port = port;

    // Initialize network manager
    _netManager = new QNetworkAccessManager(this);
    QObject::connect(_netManager, &QNetworkAccessManager::finished,
                     this, &RequestManager::replyFinished);
}

QNetworkReply *RequestManager::getConfiguration(void) {
    // Format the request
    QString request = QString(GET_REQUEST_TEMPLATE).arg(_hostName).arg(_port).arg("config");
    qDebug() << "Doing request: " << request;

    // Launch request.
    // Upon finishing the request, the correct signal will be emitted.
    return _netManager->get(QNetworkRequest(QUrl(request)));
}

void RequestManager::getCurrentMeasurement() {
    // Format the request
    QString request = QString(GET_REQUEST_TEMPLATE).arg(_hostName).arg(_port).arg("current");
    qDebug() << "Doing request: " << request;

    // Launch request.
    // Upon finishing the request, the correct signal will be emitted.
    _netManager->get(QNetworkRequest(QUrl(request)));
}

void RequestManager::getLocationCoordinates(QString location) {
    // Format the request
    QString request = QString(GET_LOCATION_TEMPLATE).arg(location);
    qDebug() << "Doing request: " << request;

    // Launch request.
    // Upon finishing the request, the correct signal will be emitted.
    _netManager->get(QNetworkRequest(QUrl(request)));
}

QJsonObject RequestManager::readJsonObject(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        // Get reply and jsonify it
        QString strReply = QString(reply->readAll());
        QJsonDocument jsonDoc = QJsonDocument::fromJson(strReply.toUtf8());
        QJsonObject mainObject = jsonDoc.object();

        // Check error code
        int errcode = mainObject["errcode"].toInt();
        if (errcode != 0)
            return QJsonObject();
        else
            return mainObject;
    }
    else
        return QJsonObject();
}

// TODO: Put in constants
void RequestManager::replyFinished(QNetworkReply *reply) {
    // Get reply URL path.
    QString urlPath = reply->url().path().remove("/");

    if (urlPath == "config")
        emit configRequestFinished(reply);
    else if (urlPath == "current")
        emit currentRequestFinished(reply);
    else if (urlPath == "mapsapigeocodejson")
        emit locationCoordinatesRequestFinished(reply);
}
