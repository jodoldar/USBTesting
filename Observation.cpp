/**
 * Observation Class
 * 
 * The observation object is intended to store all the information related to a specific
 * measurement made by the Hideki physical device. This is the basic object in the application
 * 
 * Josep Dols - 6/10/18
 */

#include "Observation.h"
#include <ctime>
#include <iostream>
#include <list>

using namespace std;

// Constructors for Observation class
Observation::Observation() {
	timestamp = std::time(nullptr);
	pressure = 1013;
	rainfall = 0;

    temperature[0] = 0.0; temperature[1] = 0.0; temperature[2] = 0.0;
    humidity[0] = 0.0; humidity[1] = 0.0; humidity[2] = 0.0;
    wind_chill = 0.0;
    wind_gust = 0.0;
    wind_dir = 0;

}
Observation::Observation(unsigned int newTimestamp){
	timestamp = newTimestamp;
	pressure = 1013;
	rainfall = 0;

    temperature[0] = 0.0; temperature[1] = 0.0; temperature[2] = 0.0;
    humidity[0] = 0.0; humidity[1] = 0.0; humidity[2] = 0.0;
    wind_chill = 0.0;
    wind_gust = 0.0;
    wind_dir = 0;
}

// Observation public methods implementation
unsigned int Observation::getTimestamp() {
    return timestamp;
}

void Observation::setTimestamp(unsigned int newTimestamp) {
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
    return wind_gust;
}

void Observation::setWindGust(float newWindGust, int pos) {
    wind_gust = newWindGust;
}


int Observation::getWindDir(int pos) {
    return wind_dir;
}

void Observation::setWindDir(int newWindDir, int pos) {
    wind_dir = newWindDir;
}


float Observation::getRainfall() {
    return rainfall;
}

void Observation::setRainfall(float newRainfall) {
    rainfall = newRainfall;
}
