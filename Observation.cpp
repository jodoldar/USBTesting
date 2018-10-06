
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

        float getTemperature(int pos) {
            return temperature[pos];
        }

        float getHumidity(int pos) {
            return humidity[pos];
        }

        float getPressure() {
            return pressure;
        }

        float getWindGust(int pos) {
            return wind_gust[pos];
        }

        int getWindDir(int pos) {
            return wind_dir[pos];
        }

        float getRainfall() {
            return rainfall;
        }
};