#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QMainWindow>
#include <QStyle>
#include <QMessageBox>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>

#include "DlgConfiguration.h"
#include "IndexesDisplay.h"
#include "MeasureDisplay.h"
#include "RequestManager.h"

namespace Ui {
class WndMain;
}

class WndMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit WndMain(QWidget *parent = 0);
    ~WndMain();

private slots:
    void configReplyFinished(QNetworkReply *reply);
    void currentReplyFinished(QNetworkReply *reply);
    void locationCoordinatesReplyFinished(QNetworkReply *reply);

    void on_pbtnSettings_clicked();

private:
    Ui::WndMain *ui;
    RequestManager *connManager;
    QSettings _settings;
    bool _flag;
};

#endif // WNDMAIN_H
