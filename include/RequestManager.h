#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include "ConfigurationConstants.h"
#include "ConfigurationManager.h"

#define GET_REQUEST_TEMPLATE "http://%1:%2/%3"
#define GET_LOCATION_TEMPLATE "https://maps.googleapis.com/maps/api/geocode/json?address=%1"
#define GET_SUNSET_SUNRISE_TEMPLATE "http://api.sunrise-sunset.org/json?lat=%1&lng=%2&formatted=0"

class RequestManager : public QObject {
    Q_OBJECT
public:
    // Cnstr.
    RequestManager(QString hostName, int port, QObject *parent = 0);

    // Methods
    QNetworkReply *getConfiguration(void);
    QNetworkReply *getCurrentMeasurement(void);
    QNetworkReply *getLocationCoordinates(void);
    QNetworkReply *getSunsetSunriseTimes(void);

    // Utility methods
    QJsonObject readJsonObject(QNetworkReply *reply);

signals:
    void configRequestFinished(QNetworkReply *reply);
    void currentRequestFinished(QNetworkReply *reply);
    void locationCoordinatesRequestFinished(QNetworkReply *reply);
    void sunsetSunriseTimesRequestFinished(QNetworkReply *reply);

public slots:
    void replyFinished(QNetworkReply *reply);

private:
    // Properties
    QString _hostName;
    int _port;
    QNetworkAccessManager *_netManager;
};

#endif // CONNECTIONMANAGER_H
