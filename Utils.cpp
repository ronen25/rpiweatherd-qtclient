#include "Utils.h"

double Utils::calculateHumidex(float temp, float humid) {
    double e;

    // Vapor pressure
    e = (6.112 * pow(10, (7.5 * temp / (237.7 + temp))) * humid / 100);

    double humidex = temp + 0.55555555 * (e - 10.0); //humidex
    return humidex;
}

// Source: http://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml
double Utils::calculateHeatIndex(float temp, float humid) {
    double heatIndex = 0;
    double adjustment = 0;

    // Calculate first using the simple formula.
    heatIndex = 0.5 * (temp + 61.0 + ((temp - 68.0) * 1.2) + (humid * 0.094));

    // Check if the full equation is needed
    // If not, return average of this and the temperature.
    if (heatIndex < 80)
        return heatIndex + temp / 2;

    // Continue calculating.
    // Calculate heat index
    heatIndex = -42.379 + 2.04901523 * temp + 10.14333127 * humid - 0.22475541 * temp * humid -
            0.00683783 * temp * temp - 0.05481717 * humid * humid + 0.00122874 * temp * temp *
            humid + 0.00085282 * temp * humid * humid - 0.00000199 * temp * temp * humid * humid;

    // Check if an adjustment is needed
    if (humid < 13 && (temp >= 80 && temp <= 112))
        adjustment = ((13 - humid) / 4) * sqrt((17 - std::abs(temp - 95.0)) / 17);
    else if (humid > 85 && (temp >= 80 && temp <= 87))
        adjustment = ((humid - 85) / 10) * ((87 - temp) / 5);

    // Add adjustments if needed
    heatIndex += adjustment;

    // Return it
    return heatIndex;
}
