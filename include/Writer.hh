#ifndef writer_h
#define writer_h

#include "EventRecord.hh"

#include <string>
#include <vector>
#include <random>

namespace name {
class ofstream;
}

class Writer
{
public:
    Writer(const std::string & FileName, bool Binary, double EnergyMin, double EnergyMax, double CTR, long seed);
    ~Writer();

    std::string write(std::vector<std::vector<EventRecord>> & Events); // returns error string, empty if success

    bool bDebug = true;
    bool bSaveEnergyDist = true;
    bool bSaveTimeDist = true;
    std::string EnergyDistFileName = "/home/andr/WORK/TPPT/Builder-EnergyDist.txt";
    std::string TimeDistFileName   = "/home/andr/WORK/TPPT/Builder-TimeDist.txt";
    //std::string EnergyDistFileName = "/data/margarida/Data/EnergyDist.txt";
    //std::string TimeDistFileName = "/data/margarida/Data/TimeDist.txt";

private:
    std::ofstream * outStream = nullptr;
    bool bBinary = false;
    double energyMin = 0;
    double energyMax = 1000.0;
    double ctr = 0;

    std::mt19937_64                  * randEngine = nullptr;
    std::normal_distribution<double> * gauss      = nullptr;

    void blurTime(double & time);
    void saveEnergyDist(std::vector<std::vector<EventRecord>> & Events);
    void saveTimeDist(std::vector<std::vector<EventRecord>> & Events);
};

#endif // writer_h
