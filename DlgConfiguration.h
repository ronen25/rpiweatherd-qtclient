#ifndef DLGCONFIGURATION_H
#define DLGCONFIGURATION_H

#include <QDialog>
#include <QSettings>
#include <QMessageBox>

#include "ConfigurationConstants.h"
#include "ConfigurationManager.h"

namespace Ui {
class DlgConfiguration;
}

class DlgConfiguration : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConfiguration(QWidget *parent = 0);
    ~DlgConfiguration();

private slots:
    void on_pbtnAbputQt_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    // Properties
    Ui::DlgConfiguration *ui;

    // Methods
    void loadConfiguration(void);
    void saveConfiguration(void);
};

#endif // DLGCONFIGURATION_H
