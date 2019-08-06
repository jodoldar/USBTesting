/**
 * @file Observation.h
 * @author Josep Dols (jodoldar@gmail.com)
 * @brief Observation Class
 * @version 1.0
 * @date 2018-10-06
 * 
 * @copyright Copyright (c) 2019
 * 
 * The observation object is intended to store all the information related to
 * a specific measurement made by the Hideki physical device. This is the basic
 * object in the application.
 */

#include <list>

/**
 * @brief Observation Class
 * 
 * The observation object is intended to store all the information related to
 * a specific measurement made by the Hideki physical device. This is the basic
 * object in the application.
 */
class Observation 
{
    /* Observation attributes */
    protected:
        /// Timestamp of when the observation is made.
        unsigned int timestamp;
        /// Temperatures of the three different sensors.
        float temperature[3];
        /// Humidity of the three different sensors.
        float humidity[3];
        /// Pressure in mb.
        float pressure;
        /// Wind chill in kmh (calculated in devide)
        float wind_chill;
        /// Wind gust in kmh
        float wind_gust;
        /// Wind speed in kmh
        float wind_speed;
        /// Wind direction (only 16 possible options)
        float wind_dir;
        /// Accumulated rain (0 by now)
        float rainfall;

        /// Calculated dew point of an Observation.
        float dew_point;
        /// Calculated RealFeel© value of an Observation.
        float real_feel;

    /* Observation public methods */
    public:
		Observation();
		Observation(unsigned int newTimestamp);
		
        unsigned int getTimestamp();
        void setTimestamp(unsigned int newTimestamp);

        float getTemperature(int pos);
        void setTemperature(float newTemperature, int pos);
        //void setTemperature(std::list<float> new_temps);

        float getHumidity(int pos);
        void setHumidity(float newHumidity, int pos);
        //void setHumidity(std::list<float> new_temps);

        float getPressure();
        void setPressure(float newPressure);

        float getWindChill();
        void setWindChill(float newWindChill);

        float getWindGust();
        void setWindGust(float newWindGust);

        float getWindSpeed();
        void setWindSpeed(float newWindSpeed);

        float getWindDir();
        void setWindDir(float newWindDir);

        float getRainfall();
        void setRainfall(float newRainfall);

        float getDewPoint();
        void setDewPoint(float newDewPoint);

        float getRealFeel();
        void setRealFeel(float newRealFeel);

        short int calculateDewPoint();
        short int calculateRealFeel(int uv_index);
};