#include "Utils.h"

double Utils::calculateHumidex(float temp, float humid) {
    double e;

    // Vapor pressure
    e = (6.112 * pow(10, (7.5 * temp / (237.7 + temp))) * humid / 100);

    double humidex = temp + 0.55555555 * (e - 10.0); //humidex
    return humidex;
}

// Source: http://www.engineeringtoolbox.com/heat-index-d_935.html
double Utils::calculateHeatIndex(float temp, float humid) {
    // Check if we need conversion to farenheit
    float tempF = Utils::convertTemperatureIfNeeded(temp,
        ConfigurationManager::instance().value(CONFIG_PREFERRED_UNIT).toString(),
        QString(RPIWEATHERD_UNITS_IMPERIAL));

    // Calculation only applies for tempF > 57 F
    if (tempF < 57)
        return temp; // No conversion

    // Calculation constants
    const double c1 = -42.739;
    const double c2 = 2.04901523;
    const double c3 = 10.14333127;
    const double c4 = 0.22475541;
    const double c5 = 0.00683783;
    const double c6 = 0.05481717;
    const double c7 = 0.00122874;
    const double c8 = 0.00085282;
    const double c9 = 0.00000199;

    // Calculate and round to two digits after the decimal point.
    double result = (c1 + c2 * tempF + c3 * humid - c4 * tempF * humid - c5 * tempF * tempF
                    - c6 * humid * humid + c7 * tempF * tempF * humid + c8 * tempF * humid * humid -
                    c9 * tempF * tempF * humid * humid);
    result = round(result * 100) / 100;

    // Convert back and return
    return Utils::convertTemperatureIfNeeded(result, QString(RPIWEATHERD_UNITS_IMPERIAL),
        ConfigurationManager::instance().value(CONFIG_PREFERRED_UNIT).toString());
}

double Utils::convertTemperatureIfNeeded(float temp) {
    // Get source and destination units from configuration
    QString sourceUnit = ConfigurationManager::instance().value(CONFIG_SERVER_UNIT).toString();
    QString targetUnit = ConfigurationManager::instance().value(CONFIG_PREFERRED_UNIT).toString();

    return Utils::convertTemperatureIfNeeded(temp, sourceUnit, targetUnit);
}

double Utils::convertTemperatureIfNeeded(float temp, QString sourceUnit, QString targetUnit) {
    // If the source and the destination unit are the same, do no conversion.
    if (sourceUnit == targetUnit)
        return temp;

    // Now perform conversion.
    if (sourceUnit == RPIWEATHERD_UNITS_METRIC && targetUnit == RPIWEATHERD_UNITS_IMPERIAL)
        return temp * 9.0 / 5.0 + 32.0;
    else if (sourceUnit == RPIWEATHERD_UNITS_IMPERIAL && targetUnit == RPIWEATHERD_UNITS_METRIC)
        return (temp - 32.0) / 1.8;
    else {
        qDebug() << "Unknown conversion from scale '" << sourceUnit << "' to '"
                 << targetUnit << "'";
        return temp;
    }
}

char Utils::unitDescriptionToUnitChar(QString desc) {
    if (desc == RPIWEATHERD_UNITS_METRIC)
        return 'c';
    else if (desc == RPIWEATHERD_UNITS_IMPERIAL)
        return 'f';
    else
        return '?';
}

QString Utils::buildCoordinatesString(QString location, double lat, double lng) {
    QString ret = "%1;%2;%3";

    return ret.arg(location).arg(lat).arg(lng);
}
