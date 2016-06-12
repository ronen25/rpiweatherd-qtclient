#ifndef MEASUREDISPLAY_H
#define MEASUREDISPLAY_H

#include <QWidget>

namespace Ui {
class MeasureDisplay;
}

class MeasureDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit MeasureDisplay(QWidget *parent = 0);
    ~MeasureDisplay();

private:
    Ui::MeasureDisplay *ui;
};

#endif // MEASUREDISPLAY_H
