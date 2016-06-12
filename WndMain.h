#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QMainWindow>
#include <QStyle>
#include <QMessageBox>

#include "DlgConfiguration.h"
#include "IndexesDisplay.h"
#include "MeasureDisplay.h"

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
    void on_horizontalSlider_valueChanged(int value);

    void on_pbtnSettings_clicked();

private:
    Ui::WndMain *ui;
};

#endif // WNDMAIN_H
