#ifndef DLGCONFIGURATION_H
#define DLGCONFIGURATION_H

#include <QDialog>

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

private:
    Ui::DlgConfiguration *ui;
};

#endif // DLGCONFIGURATION_H
