
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
};