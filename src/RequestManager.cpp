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

QNetworkReply *RequestManager::getCurrentMeasurement(void) {
    // Format the request
    QString request = QString(GET_REQUEST_TEMPLATE).arg(_hostName).arg(_port).arg("current");
    qDebug() << "Doing request: " << request;

    // Launch request.
    // Upon finishing the request, the correct signal will be emitted.
    return _netManager->get(QNetworkRequest(QUrl(request)));
}

QNetworkReply *RequestManager::getLocationCoordinates(void) {
    // Format the request
    QString request = QString(GET_LOCATION_TEMPLATE).arg(
                ConfigurationManager::instance().value(CONFIG_MEASURE_LOCATION).toString());
    qDebug() << "Doing request: " << request;

    // Launch request.
    // Upon finishing the request, the correct signal will be emitted.
    return _netManager->get(QNetworkRequest(QUrl(request)));
}

QNetworkReply *RequestManager::getSunsetSunriseTimes(void) {
    // Parse location string
    QString locationString =
            ConfigurationManager::instance().value(CONFIG_LOCATION_COORDINATES).toString();
    QStringList locationParts = locationString.split(';');
    QString lat = locationParts[1], lng = locationParts[2];

    // Format the request
    QString request = QString(GET_SUNSET_SUNRISE_TEMPLATE).arg(lat).arg(lng);
    qDebug() << "Doing request: " << request;

    // Launch request.
    // Upon finishing the request, the correct signal will be emitted.
    return _netManager->get(QNetworkRequest(QUrl(request)));
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
    else if (urlPath == "json") // YES, I KNOW IT'S UNCLEAR! TODO: Fix this.
        emit sunsetSunriseTimesRequestFinished(reply);
    else
        qDebug() << urlPath;
}
