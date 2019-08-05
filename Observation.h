/**
 * Observation Class
 * 
 * The observation object is intended to store all the information related to a specific
 * measurement made by the Hideki physical device. This is the basic object in the application
 * 
 * Josep Dols - 6/10/18
 */

#include <list>

class Observation 
{
    // Observation attributes
    protected:
        unsigned int timestamp;
        float temperature[3];
        float humidity[3];
        float pressure;
        float wind_chill;
        float wind_gust;
        float wind_speed;
        float wind_dir;
        float rainfall;

        float dew_point;
        float real_feel;

    // Observation public methods
    public:
		Observation();
		Observation(unsigned int newTimestamp);
		
        unsigned int getTimestamp();
        void setTimestamp(unsigned int newTimestamp);

        float getTemperature(int pos);
        void setTemperature(float newTemperature, int pos);
        void setTemperature(std::list<float> new_temps);

        float getHumidity(int pos);
        void setHumidity(float newHumidity, int pos);
        void setHumidity(std::list<float> new_temps);

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
        short int calculateRealFeel();
};

