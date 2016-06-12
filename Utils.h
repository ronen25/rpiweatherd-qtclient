#ifndef INDEXESUTILS_H
#define INDEXESUTILS_H

#include <cmath>

class Utils {
public:
    static double calculateHumidex(float temp, float humid);
    static double calculateHeatIndex(float temp, float humid);
};

#endif // INDEXESUTILS_H
