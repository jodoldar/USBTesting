/**
 * @file Observation.cpp
 * @author Josep Dols (jodoldar@gmail.com)
 * @brief Observation Class
 * @version 1.0.0
 * @date 2018-10-06
 * 
 * @copyright Copyright (c) 2019
 * 
 * The observation object is intended to store all the information related to
 * a specific measurement made by the Hideki physical device. This is the basic
 * object in the application.
 */

#include "Observation.h"
#include <ctime>
#include <iostream>
#include <list>
#include <math.h>

using namespace std;

/**
 * @brief Construct a new Observation object
 * 
 * Creates a new Observation object, with all the values initialized to 0,
 * except the pressure, initialized to 1013(STD) and the timestamp, wich is
 * established as the current UNIX timestamp.
 */
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

/**
 * @brief Construct a new Observation object
 * 
 * Creates a new Observation object, with all the values initialized to 0,
 * except the pressure, initialized to 1013(STD) and the timestamp, which is
 * indicated in a parameter.
 * 
 * @param newTimestamp Timestamp to assign to the Observation.
 */
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

/**
 * @brief Get the value of the timestamp attribute
 * 
 * @return unsigned int Current timestamp of the Observation
 */
unsigned int Observation::getTimestamp() {
    return timestamp;
}

/**
 * @brief Set the value of the timestamp attribute
 * 
 * @param newTimestamp New timestamp to assign to the Observation
 */
void Observation::setTimestamp(unsigned int newTimestamp) {
    timestamp = newTimestamp;
}

/**
 * @brief Get the value of a temperature attribute
 * 
 * @param pos Identifier of the temperature to obtain
 * @return float Value of the temperature selected
 */
float Observation::getTemperature(int pos) {
    return temperature[pos];
}

/**
 * @brief Set the value of a temperature attribute
 * 
 * @param newTemperature New value of the selected temperature
 * @param pos Identifier of the temperature to modify
 */
void Observation::setTemperature(float newTemperature, int pos) {
    temperature[pos] = newTemperature;
}

/**
 * @brief Get the value of a humidity attribute
 * 
 * @param pos Identifier of the humidity to obtain
 * @return float Value of the humidity selected
 */
float Observation::getHumidity(int pos) {
    return humidity[pos];
}

/**
 * @brief Set the value of a humidity attribute
 * 
 * @param newHumidity New value of the selected humidity
 * @param pos Identifier of the temperature to modify
 */
void Observation::setHumidity(float newHumidity, int pos) {
    humidity[pos] = newHumidity;
}

/**
 * @brief Get the value of the pressure attribute
 * 
 * @return float Value of the pressure
 */
float Observation::getPressure() {
    return pressure;
}

/**
 * @brief Set the value of the pressure attribute
 * 
 * @param newPressure New value of the pressure
 */
void Observation::setPressure(float newPressure) {
    pressure = newPressure;
}

/**
 * @brief Get the value of the wind chill attribute
 * 
 * @return float Value of the wind chill
 */
float Observation::getWindChill()
{
    return wind_chill;
}

/**
 * @brief Set the value of the wind chill attribute
 * 
 * @param newWindChill New value of the wind chill
 */
void Observation::setWindChill(float newWindChill)
{
    wind_chill = newWindChill;
}

/**
 * @brief Get the value of the wind gust attribute
 * 
 * @return float Value of the wind gust
 */
float Observation::getWindGust() {
    return wind_gust;
}

/**
 * @brief Set the value of the wind gust attribute
 * 
 * @param newWindGust New value of the wind gust
 */
void Observation::setWindGust(float newWindGust) {
    wind_gust = newWindGust;
}

/**
 * @brief Get the value of the wind speed attribute
 * 
 * @return float Value of the wind speed
 */
float Observation::getWindSpeed()
{
    return wind_speed;
}

/**
 * @brief Set the value of the wind speed attribute
 * 
 * @param newWindSpeed New value of the wind speed
 */
void Observation::setWindSpeed(float newWindSpeed)
{
    wind_speed = newWindSpeed;
}

/**
 * @brief Get the value of the wind direction attribute
 * 
 * @return float Value of the wind direction
 */
float Observation::getWindDir() {
    return wind_dir;
}

/**
 * @brief Set the value of the wind direction attribute
 * 
 * @param newWindDir New value of the wind direction
 */
void Observation::setWindDir(float newWindDir) {
    wind_dir = newWindDir;
}

/**
 * @brief Get the value of the rainfall attribute
 * 
 * @return float Value of the rainfall
 */
float Observation::getRainfall() {
    return rainfall;
}

/**
 * @brief Set the value of the rainfall attribute
 * 
 * @param newRainfall New value of the rainfall
 */
void Observation::setRainfall(float newRainfall) {
    rainfall = newRainfall;
}

/**
 * @brief Get the value of the dew point attribute
 * 
 * @return float Value of the dew point
 */
float Observation::getDewPoint()
{
    return dew_point;
}

/**
 * @brief Set the value of the dew point attribute
 * 
 * @param newDewPoint New value of the dew point
 */
void Observation::setDewPoint(float newDewPoint)
{
    dew_point = newDewPoint;
}

/**
 * @brief Get the value of the RealFeel attribute
 * 
 * @return float Value of the RealFeel
 */
float Observation::getRealFeel()
{
    return real_feel;
}

/**
 * @brief Set the value of the RealFeel attribute
 * 
 * @param newRealFeel New value of the RealFeel
 */
void Observation::setRealFeel(float newRealFeel)
{
    real_feel = newRealFeel;
}

/**
 * @brief Calculation of the dew point
 * 
 * From the current temperature and humidity attributes already present in the
 * Observation, the associated dew point is calculated.
 * 
 * @return short int If the calculated value is valid.
 */
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

/**
 * @brief Calculation of the RealFeel value
 * 
 * From the different attributes of an Observation, and following the original
 * patent function, calculates the RealFeel value associate to an Observation
 * 
 * Note: currently the Rain Index (RI1 & RI2) are 0.
 * 
 * TODO: Introduce an external UV value to the formulae.
 * 
 * @param uv_index UV index externally calculated
 * @return short int 
 */
short int Observation::calculateRealFeel(int uv_index)
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
        SI2 = uv_index;

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
        SI1 = uv_index;

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