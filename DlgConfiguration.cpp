#include "DlgConfiguration.h"
#include "ui_DlgConfiguration.h"

DlgConfiguration::DlgConfiguration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConfiguration)
{
    ui->setupUi(this);
}

DlgConfiguration::~DlgConfiguration()
{
    delete ui;
}