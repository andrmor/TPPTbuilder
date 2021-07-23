#ifndef writer_h
#define writer_h

#include "EventRecord.hh"

#include <string>
#include <vector>
#include <random>

namespace name {
class ofstream;
}

class Hist1D;

class Writer
{
public:
    Writer(const std::string & dir, const std::string & fileName, bool Binary);
    ~Writer();

    bool isOK() const {return (bool)outStream;}
    void configure(double energyMin, double energyMax, double ctr, long seed);

    void write(std::vector<std::vector<EventRecord>> & Events);

    void saveEnergyDist(const std::string & fileName);
    void saveTimeDist(const std::string & fileName);

    bool bDebug = false;
    bool bSaveEnergyDist = true;
    bool bSaveTimeDist = true;
    std::string EnergyDistFileName = "/home/andr/WORK/TPPT/Builder-EnergyDist.txt";
    std::string TimeDistFileName   = "/home/andr/WORK/TPPT/Builder-TimeDist.txt";
    //std::string EnergyDistFileName = "/data/margarida/Data/EnergyDist.txt";
    //std::string TimeDistFileName = "/data/margarida/Data/TimeDist.txt";

private:
    std::string Dir;
    std::string FileName  = "UndefinedSaveName";
    bool        bBinary   = false;
    double      EnergyMin = 0;
    double      EnergyMax = 1000.0;
    double      CTR       = 0;

    std::ofstream                    * outStream  = nullptr;
    std::mt19937_64                  * randEngine = nullptr;
    std::normal_distribution<double> * gauss      = nullptr;
    Hist1D                           * histEnergy = nullptr;
    Hist1D                           * histTime   = nullptr;

    void blurTime(double & time);
};

#endif // writer_h
