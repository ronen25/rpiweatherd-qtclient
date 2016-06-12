#include "DlgConfiguration.h"
#include "ui_DlgConfiguration.h"

DlgConfiguration::DlgConfiguration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConfiguration)
{
    ui->setupUi(this);

    // Set version info
    ui->lblAbout->setText(ui->lblAbout->text().arg(APPLICATION_VERSION));

    // Load configuration information
    loadConfiguration();
}

DlgConfiguration::~DlgConfiguration()
{
    delete ui;
}

void DlgConfiguration::on_pbtnAbputQt_clicked()
{
    QApplication::aboutQt();
}

void DlgConfiguration::loadConfiguration() {
    // QSettings instance
    QSettings settings("RonenL", "rpiweatherd-qtclient");

    // Load server details
    ui->lnServerHostname->setText(settings.value("server_ip", ""));
    ui->spnPort->setValue(settings.value("server_port", RPIWEATHERD_DEFAULT_PORT));

    // Load measurement unit
    QString unit = settings.value("unit", "");
    if (unit == "")
        ui->radUnitsUseServer->setChecked(true);
    else if (unit == "metric")
        ui->radUnitsMetric->setChecked(true);
    else if (unit == "imperial")
        ui->radUnitsImperial->setChecked(true);

    // Show humidity
    ui->chkShowHumidity->setChecked(settings.value("show_humidity", true));
}

void DlgConfiguration::saveConfiguration() {
}
