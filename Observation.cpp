/**
 * Observation Class
 * 
 * The observation object is intended to store all the information related to a specific
 * measurement made by the Hideki physical device. This is the basic object in the application
 * 
 * Josep Dols - 6/10/18
 */

#include "Observation.h";

using namespace std;

// Observation public methods implementation
double Observation::getTimestamp() {
    return timestamp;
}

void Observation::setTimestamp(double newTimestamp) {
    timestamp = newTimestamp;
}


float Observation::getTemperature(int pos) {
    return temperature[pos];
}

void Observation::setTemperature(float newTemperature, int pos) {
    temperature[pos] = newTemperature;
}


float Observation::getHumidity(int pos) {
    return humidity[pos];
}

void Observation::setHumidity(float newHumidity, int pos) {
    humidity[pos] = newHumidity;
}


float Observation::getPressure() {
    return pressure;
}

void Observation::setPressure(float newPressure) {
    pressure = newPressure;
}


float Observation::getWindGust(int pos) {
    return wind_gust[pos];
}

void Observation::setWindGust(float newWindGust, int pos) {
    wind_gust[pos] = newWindGust;
}


int Observation::getWindDir(int pos) {
    return wind_dir[pos];
}

void Observation::setWindDir(int newWindDir, int pos) {
    wind_dir[pos] = newWindDir;
}


float Observation::getRainfall() {
    return rainfall;
}

void Observation::setRainfall(float newRainfall) {
    rainfall = newRainfall;
}