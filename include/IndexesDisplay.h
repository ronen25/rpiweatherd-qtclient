#ifndef HUMIDEXDISPLAY_H
#define HUMIDEXDISPLAY_H

#include <QObject>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

// Constants
#define NO_HUMIDEX_VALUE -1

class IndexesDisplay : public QWidget {
    Q_OBJECT
public:
    explicit IndexesDisplay(QWidget *parent = 0);

    // Getters
    int value(void) const;

public slots:
    void setValue(int value);
    void resetState(void);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    int _value;

private:
    // Constant colors for the humidex value
    const QColor HUMIDEX_NO_DISCOMFORT_COLOR = QColor::fromRgb(86, 228, 228);
    const QColor HUMIDEX_SOME_DISCOMFORT_COLOR = QColor::fromRgb(51, 255, 153);
    const QColor HUMIDEX_GREAT_DISCOMFORT_COLOR = QColor::fromRgb(219, 219, 49);
    const QColor HUMIDEX_DANGEROUS_COLOR = QColor::fromRgb(255, 153, 51);
    const QColor HUMIDEX_HEAT_STROKE_COLOR = QColor::fromRgb(255, 51, 51);

    // Constants
    const QString HUMIDEX_PLACEHOLDER = "HUMIDEX --";
    const QString HUMIDEX_TEMPLATE = "HUMIDEX %1";

    // Calculation Methods (Humidex)
    QString getHumidexDescription(void) const;
    QColor getHumidexColor(void) const;
};

#endif // HUMIDEXDISPLAY_H
