#include "MeasureDisplay.h"
#include "ui_MeasureDisplay.h"

MeasureDisplay::MeasureDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeasureDisplay),
    _textColor(QApplication::palette().color(QPalette::WindowText))
{
    ui->setupUi(this);

    // Set size policies
    ui->lblMeasurement->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    // Check if we need to display humidity
    ui->lblHumidity->setVisible(
                ConfigurationManager::instance().value(CONFIG_SHOW_HUMIDITY).toBool());
}

MeasureDisplay::~MeasureDisplay()
{
    delete ui;
}

float MeasureDisplay::temperatureDisplayed() const {
    return _temp;
}

float MeasureDisplay::humidityDisplayed() const {
    return _humid;
}

QString MeasureDisplay::dateDisplayed() const {
    return _measureDate;
}

QColor MeasureDisplay::textColor() const {
    return _textColor;
}

void MeasureDisplay::resetState() {
    // Reset all texts
    ui->lblDate->setText("--");
    ui->lblFeelsLike->setText("--");
    ui->lblHumidity->setText("--");
    ui->lblMeasurement->setText("--");

    // Resize newly-created text
    resizeEvent(nullptr);
}

void MeasureDisplay::setMeasurementDetails(QDate date, float temperature, char unit,
                                           float humidity) {
    // Set all details
    _temp = temperature;
    _humid = humidity;
    _measureDate = date.toString("dddd, dd MMMM yyyy");
    _measureUnit = unit;

    // Set texts
    ui->lblDate->setText(_measureDate);
    ui->lblMeasurement->setText(MEASUREMENT_DISPLAY_TEMPLATE.arg(_temp).arg(
                                                             QChar(unit).toUpper().toLatin1()));
    ui->lblHumidity->setText(HUMIDITY_DISPLAY_TEMPLATE.arg(_humid));
    ui->lblFeelsLike->setText(FEELS_LIKE_DISPLAY_TEMPLATE.arg(
                                  Utils::calculateHeatIndex(temperature, humidity)).arg(
                                  QChar(unit).toUpper().toLatin1()));

    // Resize newly-created text
    resizeEvent(nullptr);
}

void MeasureDisplay::setTextColor(const QColor &color) {
    _textColor = color;

    // Set stylesheet
    QPalette palette = this->palette();
    palette.setColor(QPalette::WindowText, color);
    this->setPalette(palette);
}

void MeasureDisplay::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);

    // Get font and font matrics
    QFontMetrics metrics(ui->lblMeasurement->font());
    QFont labelFont = ui->lblMeasurement->font();

    // Calculate widths
    int stringWidth = metrics.width(ui->lblMeasurement->text());

    // Find out how much can the font grow
    float widthRatio = (float)ui->lblMeasurement->width() / (float)stringWidth;
    int newFontSize = (int)
            ((labelFont.pixelSize() == -1 ? labelFont.pointSize() : labelFont.pixelSize())
                                           * widthRatio);

    labelFont.setPointSize(std::min(newFontSize, ui->lblMeasurement->height()));
    ui->lblMeasurement->setFont(labelFont);
}
