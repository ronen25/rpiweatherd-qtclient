#include "IndexesDisplay.h"

IndexesDisplay::IndexesDisplay(QWidget *parent) : QWidget(parent) {
    // Set default value
    _value = -1;
}

int IndexesDisplay::value() const {
    return _value;
}

void IndexesDisplay::setValue(int value) {
    // Set value
    this->_value = value;

    // Set tooltip to humidex description
    setToolTip(getHumidexDescription());

    // Force re-draw
    this->repaint();
}

void IndexesDisplay::paintEvent(QPaintEvent *event) {
    event->accept();

    // Create painter
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Create the font that will be used to draw the text
    QFont font = painter.font();
    font.setBold(true);
    font.setPixelSize(11);
    painter.setFont(font);

    // Fill color
    painter.fillRect(0, 0, width(), height(), getHumidexColor());

    // If no value is provided, use placeholder
    painter.setPen(QColor::fromRgb(255, 255, 255));
    if (_value == -1)
        painter.drawText(this->rect(), Qt::AlignCenter, HUMIDEX_PLACEHOLDER);
    else
        painter.drawText(this->rect(), Qt::AlignCenter, HUMIDEX_TEMPLATE.arg(_value));
}

QString IndexesDisplay::getHumidexDescription() const {
    if (_value <= 29)
        return "No discomfort";
    else if (30 <= _value && _value <= 39)
        return "Some discomfort";
    else if (40 <= _value && _value <= 45)
        return "Great discomfort; avoid exertion";
    else if (45 <= _value && _value <= 53)
        return "Dangerous";
    else if (_value >= 54)
        return "Heat Stroke Imminent";
    else
        return "Unknown";
}

QColor IndexesDisplay::getHumidexColor() const {
    if (_value == -1)
        return QColor::fromRgb(0, 0, 0); // No value (yet)
    else if (_value <= 29)
        return HUMIDEX_NO_DISCOMFORT_COLOR;
    else if (30 <= _value && _value <= 39)
        return HUMIDEX_SOME_DISCOMFORT_COLOR;
    else if (40 <= _value && _value <= 45)
        return HUMIDEX_GREAT_DISCOMFORT_COLOR;
    else if (45 <= _value && _value <= 53)
        return HUMIDEX_DANGEROUS_COLOR;
    else if (_value >= 54)
        return HUMIDEX_HEAT_STROKE_COLOR;
}
