/**
 * Observation Class
 * 
 * The observation object is intended to store all the information related to a specific
 * measurement made by the Hideki physical device. This is the basic object in the application
 * 
 * Josep Dols - 6/10/18
 */

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
        int wind_dir;
        float rainfall;

    // Observation public methods
    public:
		Observation();
		Observation(unsigned int newTimestamp);
		
        unsigned int getTimestamp();
        void setTimestamp(unsigned int newTimestamp);
        float getTemperature(int pos);
        void setTemperature(float newTemperature, int pos);
        float getHumidity(int pos);
        void setHumidity(float newHumidity, int pos);
        float getPressure();
        void setPressure(float newPressure);
        float getWindGust(int pos);
        void setWindGust(float newWindGust, int pos);
        int getWindDir(int pos);
        void setWindDir(int newWindDir, int pos);
        float getRainfall();
        void setRainfall(float newRainfall);
};
