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
#include <math.h>

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

float Observation::getWindChill()
{
    return wind_chill;
}

void Observation::setWindChill(float newWindChill)
{
    wind_chill = newWindChill;
}

float Observation::getWindGust() {
    return wind_gust;
}

void Observation::setWindGust(float newWindGust) {
    wind_gust = newWindGust;
}

float Observation::getWindSpeed()
{
    return wind_speed;
}

void Observation::setWindSpeed(float newWindSpeed)
{
    wind_speed = newWindSpeed;
}

float Observation::getWindDir() {
    return wind_dir;
}

void Observation::setWindDir(float newWindDir) {
    wind_dir = newWindDir;
}


float Observation::getRainfall() {
    return rainfall;
}

void Observation::setRainfall(float newRainfall) {
    rainfall = newRainfall;
}

float Observation::getDewPoint()
{
    return dew_point;
}

void Observation::setDewPoint(float newDewPoint)
{
    dew_point = newDewPoint;
}

float Observation::getRealFeel()
{
    return real_feel;
}

void Observation::setRealFeel(float newRealFeel)
{
    real_feel = newRealFeel;
}

short int Observation::calculateDewPoint()
{
    float f_dew_point;

    if (humidity[1] == 0.0 & temperature[1] == 0.0)
        return -1;

    f_dew_point = pow(humidity[1]/100, 0.125);
    f_dew_point *= (112 + (0.9*temperature[1]));
    f_dew_point -= 112;

    dew_point = f_dew_point;
    return 0;
}

short int Observation::calculateRealFeel()
{
    float temp1_f;
    float wind_speed_mph;
    float dew_point1_f;
    float MFT;

    if (humidity[1] == 0.0 & temperature[1] == 0.0)
        return -1;

    calculateDewPoint();

    // Initial conversions
    temp1_f = ((temperature[1]*9) / 5) + 32;
    wind_speed_mph = wind_speed / 1,609;
    dew_point1_f = ((dew_point*9) / 5) + 32;

    // Step 205. Separate ways depending on T1.
    
    if (temp1_f > 65)
    {
        // Step 210. Calculation of WSP2
        float WSP2, SI2, H2, P2;

        float w_a;
        if (wind_speed_mph < 4)
        {
            w_a = wind_speed_mph/2 + 2;
        }
        else if (wind_speed_mph > 56)
        {
            w_a = 56.0;
        }
        else
        {
            w_a = wind_speed_mph;
        }
        
        WSP2 = (80 - temp1_f) * (0.566 + 0.25*sqrt(w_a) - 0.0166 * w_a) * (sqrt(pressure/10)/10);

        // Step 220. Calculation of SI2
        SI2 = 2;

        // Step 230. Calculation of H2
        float d_a;
        if (dew_point1_f >= (55+sqrt(wind_speed_mph)))
        {
            d_a = dew_point1_f;
        }
        else
        {
            d_a = 55 + sqrt(wind_speed_mph);
        }
        
        H2 = pow((d_a - 55 - sqrt(wind_speed_mph)), 2) / 30;

        // Step 240. Calculation of P2
        P2 = 0;

        // Step 250. Calculation of MFT
        MFT = 80 - WSP2 + SI2 + H2 - P2;
    }
    else
    {
        // Step 215. Calculation of WSP1
        float WSP1, SI1, H1, P1;

        WSP1 = sqrt(wind_speed_mph) * (sqrt(pressure/10)/10);

        // Step 225. Calculation of SI1
        SI1 = 2;

        // Step 235. Calculation of H1
        float d_a;
        if (dew_point1_f >= (55+sqrt(wind_speed_mph)))
        {
            d_a = dew_point1_f;
        }
        else
        {
            d_a = 55 + sqrt(wind_speed_mph);
        }
        
        H1 = pow((d_a - 55 - sqrt(wind_speed_mph)), 2) / 30;

        // Step 245. Calculation of P1
        P1 = 0;

        // Step 255. Calculation of MFT
        MFT = temp1_f - WSP1 + SI1 + H1 - P1;
    }

    real_feel = ((MFT - 32) * 5) / 9;
}