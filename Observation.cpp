
using namespace std;

class Observation 
{
    protected:
        double timestamp;
        float temperature[];
        float humidity[];
        float pressure;
        float wind_gust[];
        int wind_dir[];
        float rainfall;

    public:
        double getTimestamp() {
            return timestamp;
        }

        void setTimestamp(double newTimestamp) {
            timestamp = newTimestamp;
        }


        float getTemperature(int pos) {
            return temperature[pos];
        }
        
        void setTemperature(float newTemperature, int pos) {
            temperature[pos] = newTemperature;
        }


        float getHumidity(int pos) {
            return humidity[pos];
        }

        void setHumidity(float newHumidity, int pos) {
            humidity[pos] = newHumidity;
        }


        float getPressure() {
            return pressure;
        }

        void setPressure(float newPressure) {
            pressure = newPressure;
        }


        float getWindGust(int pos) {
            return wind_gust[pos];
        }

        void setWindGust(float newWindGust, int pos) {
            wind_gust[pos] = newWindGust;
        }


        int getWindDir(int pos) {
            return wind_dir[pos];
        }

        void setWindDir(int newWindDir, int pos) {
            wind_dir[pos] = newWindDir;
        }


        float getRainfall() {
            return rainfall;
        }

        void setRainfall(float newRainfall) {
            rainfall = newRainfall;
        }
};