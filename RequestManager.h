#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QThread>

#include "ConfigurationConstants.h"

#define GET_REQUEST_TEMPLATE "http://%1:%2/%3"
#define GET_LOCATION_TEMPLATE "https://maps.googleapis.com/maps/api/geocode/json?address=%1"

class RequestManager : public QObject {
    Q_OBJECT
public:
    // Cnstr.
    RequestManager(QString hostName, int port, QObject *parent = 0);

    // Methods
    QNetworkReply *getConfiguration(void);
    void getCurrentMeasurement(void);
    void getLocationCoordinates(QString location);

signals:
    void configRequestFinished(QNetworkReply *reply);
    void currentRequestFinished(QNetworkReply *reply);
    void locationCoordinatesRequestFinished(QNetworkReply *reply);

public slots:
    void replyFinished(QNetworkReply *reply);

private:
    // Properties
    QString _hostName;
    int _port;
    QNetworkAccessManager *_netManager;
};

#endif // CONNECTIONMANAGER_H
