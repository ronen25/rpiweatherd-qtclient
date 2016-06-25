#include "DlgConfiguration.h"
#include "ui_DlgConfiguration.h"

DlgConfiguration::DlgConfiguration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConfiguration) {
    ui->setupUi(this);

    // Set version info
    ui->lblAbout->setText(ui->lblAbout->text().arg(APPLICATION_VERSION));

    // Load configuration information
    loadConfiguration();
}

DlgConfiguration::~DlgConfiguration() {
    delete ui;
}

void DlgConfiguration::showAboutTab() {
    ui->tabWidget->setCurrentIndex(1);
}

void DlgConfiguration::on_pbtnAbputQt_clicked() {
    QApplication::aboutQt();
}

void DlgConfiguration::loadConfiguration() {
    // QSettings instance
    QSettings &settings = ConfigurationManager::instance().settings();

    // Sync
    settings.sync();

    // Load server details
    ui->lnServerHostname->setText(settings.value(CONFIG_SERVER_IP, "").toString());
    ui->spnPort->setValue(settings.value(CONFIG_SERVER_PORT, RPIWEATHERD_DEFAULT_PORT).toInt());

    // Load measurement unit
    QString unit = settings.value(CONFIG_PREFERRED_UNIT,
                                  settings.value(CONFIG_SERVER_UNIT)).toString();
    QString serverUnit = settings.value(CONFIG_SERVER_UNIT).toString();

    if (serverUnit == unit)
        ui->radUnitsUseServer->setChecked(true);
    else if (unit == RPIWEATHERD_UNITS_METRIC)
        ui->radUnitsMetric->setChecked(true);
    else if (unit == RPIWEATHERD_UNITS_IMPERIAL)
        ui->radUnitsImperial->setChecked(true);

    // Show humidity
    ui->chkShowHumidity->setChecked(settings.value(CONFIG_SHOW_HUMIDITY, true).toBool());

    // Enable dynamic background
    ui->grpEnableDynamicBackground->setChecked(
                settings.value(CONFIG_ENABLE_DYNAMIC_BACKGROUND).toBool());
}

void DlgConfiguration::saveConfiguration() {
    // QSettings instance
    QSettings &settings = ConfigurationManager::instance().settings();

    // Set first-time configuration boolean to true, just in case.
    settings.setValue(CONFIG_FIRST_CONFIG, true);

    // Set server hostname
    if (!ui->lnServerHostname->text().isEmpty())
        settings.setValue(CONFIG_SERVER_IP, ui->lnServerHostname->text());
    else
        QMessageBox::critical(this, tr("Server Hostname Missing"),
                              tr("Server hostname must not be empty."),
                              QMessageBox::Ok);

    // Set server port
    settings.setValue(CONFIG_SERVER_PORT, ui->spnPort->value());

    // Set measurement unit
    if (ui->radUnitsUseServer->isChecked())
        settings.setValue(CONFIG_PREFERRED_UNIT, settings.value(CONFIG_SERVER_UNIT));
    else if (ui->radUnitsImperial->isChecked())
        settings.setValue(CONFIG_PREFERRED_UNIT, RPIWEATHERD_UNITS_IMPERIAL);
    else if (ui->radUnitsMetric->isChecked())
        settings.setValue(CONFIG_PREFERRED_UNIT, RPIWEATHERD_UNITS_METRIC);

    // Set show humidity
    settings.setValue(CONFIG_SHOW_HUMIDITY, ui->chkShowHumidity->isChecked());

    // Set dynamic background
    settings.setValue(CONFIG_ENABLE_DYNAMIC_BACKGROUND,
                      ui->grpEnableDynamicBackground->isChecked());

    // Sync
    settings.sync();
}

void DlgConfiguration::on_buttonBox_accepted() {
    saveConfiguration();
}

void DlgConfiguration::on_buttonBox_rejected() {
    close();
}
