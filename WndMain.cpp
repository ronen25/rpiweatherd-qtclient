#include "WndMain.h"
#include "ui_WndMain.h"

WndMain::WndMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WndMain)
{
    // TODO: Duplicate. Macro this somehow?
    QSettings settings("RonenL", "rpiweatherd-qtclient");

    ui->setupUi(this);

    // Load standard icons
    ui->pbtnRefresh->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    ui->pbtnSettings->setIcon(style()->standardIcon(QStyle::SP_FileDialogListView));

    // Check if we need first configuration
    if (!settings.value("first_config", false)) {
        int result = QMessageBox::question(this, tr("Configuration Needed"),
                              tr("The program requires configuration.\n " \
                                  "Would you like to configure it now?"),
                              QMessageBox::Yes, QMessageBox::No);

        if (result == QMessageBox::Yes)
            on_pbtnSettings_clicked();
        else
            QApplication::exit();
    }
    else {
        // Fetch data (TODO)
    }
}

WndMain::~WndMain()
{
    delete ui;
}

void WndMain::on_horizontalSlider_valueChanged(int value)
{
    ui->wgIndexDisplay->setValue(value);
}

void WndMain::on_pbtnSettings_clicked()
{
    // Show settings UI.
    // If accepted, save settings and reload view.
    DlgConfiguration *config = new DlgConfiguration(this);
    int result = config->exec();
}
