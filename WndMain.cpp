#include "WndMain.h"
#include "ui_WndMain.h"

WndMain::WndMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WndMain)
{
    ui->setupUi(this);

    // Init indexes displays
    ui->wgIndexDisplay->setVisible(true);
}

WndMain::~WndMain()
{
    delete ui;
}

void WndMain::on_horizontalSlider_valueChanged(int value)
{
    ui->wgIndexDisplay->setValue(value);
}
