#include "MeasureDisplay.h"
#include "ui_MeasureDisplay.h"

MeasureDisplay::MeasureDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeasureDisplay)
{
    ui->setupUi(this);
}

MeasureDisplay::~MeasureDisplay()
{
    delete ui;
}
