#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QMainWindow>
#include <QStyle>
#include <QMessageBox>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>
#include <QDateTime>
#include <QPainter>
#include <QRadialGradient>
#include <QPalette>
#include <QShortcut>

#include "ConfigurationManager.h"
#include "DlgConfiguration.h"
#include "IndexesDisplay.h"
#include "MeasureDisplay.h"
#include "RequestManager.h"
#include "PhaseImageMap.h"

namespace Ui {
class WndMain;
}

class WndMain : public QMainWindow {
    Q_OBJECT
public:
    // Cnstr.
    explicit WndMain(QWidget *parent = 0);

    // Dstr.
    ~WndMain();

    // Utility methods
    void fetchProcedure(void);

    // Helper methods
    void showServerErrorMessage(int retcode, QString msg);

private slots:
    void resetState(void);

    // PhaseMap callback
    void changeTextColor(void);

    // Slots for network replies
    void configReplyFinished(QNetworkReply *reply);
    void currentReplyFinished(QNetworkReply *reply);
    void locationCoordinatesReplyFinished(QNetworkReply *reply);
    void sunsetSunriseReplyFinished(QNetworkReply *reply);

    // UI Callbacks
    void on_pbtnSettings_clicked();

    void on_pbtnRefresh_clicked();

    void on_f1AboutShortcut();

protected:
    void paintEvent(QPaintEvent *ev);

private:
    // Init Methods
    void initConnectionManager(void);

    // Properties
    Ui::WndMain *ui;
    RequestManager *connManager;
    PhaseImageMap *skyColorMap, *sunColorMap;

    // Internal properties
    bool _locationChanged;
};

#endif // WNDMAIN_H
