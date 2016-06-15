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

void RequestManager::getConfiguration(void) {
    // Format the request
    QString request = QString(GET_REQUEST_TEMPLATE).arg(_hostName).arg(_port).arg("config");
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

// TODO: Put in constants
void RequestManager::replyFinished(QNetworkReply *reply) {
    // Get reply URL path.
    QString urlPath = reply->url().path().remove("/");

    // Check it
    qDebug() << urlPath;

    if (urlPath == "config")
        emit configRequestFinished(reply);
    else if (urlPath == "current")
        emit currentRequestFinished(reply);
    else if (urlPath == "mapsapigeocodejson")
        emit locationCoordinatesRequestFinished(reply);
}
