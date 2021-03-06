#ifndef MEASUREDISPLAY_H
#define MEASUREDISPLAY_H

#include <QWidget>
#include <QDebug>
#include <QDate>

#include "Utils.h"

namespace Ui {
class MeasureDisplay;
}

class MeasureDisplay : public QWidget {
    Q_OBJECT
public:
    explicit MeasureDisplay(QWidget *parent = 0);
    ~MeasureDisplay();

    // Getters for the measurement details
    float temperatureDisplayed(void) const;
    float humidityDisplayed(void) const;
    QString dateDisplayed(void) const;
    QColor textColor(void) const;

    void resetState(void);

public slots:
    // Setter for all properties
    void setMeasurementDetails(QDate date, float temperature, char unit, float humidity, float serverVersion);

    // Color setter
    void setTextColor(const QColor &color);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::MeasureDisplay *ui;
    float _temp, _humid;
    QString _measureDate;
    char _measureUnit;
    QColor _textColor;

    // Constants
    const QString MEASUREMENT_DISPLAY_TEMPLATE = tr("%1 °%2");
    const QString HUMIDITY_DISPLAY_TEMPLATE = tr("Humidity: %1% ");
    const QString FEELS_LIKE_DISPLAY_TEMPLATE = tr("(Feels like %1 °%2)");
};

#endif // MEASUREDISPLAY_H
