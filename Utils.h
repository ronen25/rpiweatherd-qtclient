#ifndef INDEXESUTILS_H
#define INDEXESUTILS_H

#include <cmath>

#include <QString>
#include <QDebug>

#include "ConfigurationConstants.h"
#include "ConfigurationManager.h"

class Utils {
public:
    // Humidex calculation
    static double calculateHumidex(float temp, float humid);

    // Heat Index calculation
    static double calculateHeatIndex(float temp, float humid);

    // Unit/Temperature conversion helpers
    static double convertTemperatureIfNeeded(float temp);
    static double convertTemperatureIfNeeded(float temp, QString sourceUnit, QString targetUnit);

    static char unitDescriptionToUnitChar(QString desc);

    static QString buildCoordinatesString(QString location, double lat, double lng);
};

#endif // INDEXESUTILS_H
