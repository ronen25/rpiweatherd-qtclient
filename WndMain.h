#ifndef WNDMAIN_H
#define WNDMAIN_H

#include <QMainWindow>

#include "IndexesDisplay.h"

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

private:
    Ui::WndMain *ui;
};

#endif // WNDMAIN_H
