#ifndef writer_h
#define writer_h

#include "EventRecord.hh"

#include <string>
#include <vector>
#include <random>

//namespace name {
//class ofstream;
//}

class Configuration;
class Hist1D;

class Writer
{
public:
    Writer();
    ~Writer();

    void write(std::vector<std::vector<EventRecord>> & Events);

    void saveEnergyDist(const std::string & fileName);
    void saveTimeDist  (const std::string & fileName);

    bool bSaveEnergyDist = true;
    bool bSaveTimeDist = true;
    std::string EnergyDistFileName = "Builder-EnergyDist.txt";
    std::string TimeDistFileName   = "Builder-TimeDist.txt";

private:
    const Configuration & Config;

    std::ofstream                    * outStream  = nullptr;
    std::mt19937_64                  * randEngine = nullptr;
    std::normal_distribution<double> * gauss      = nullptr;
    Hist1D                           * histEnergy = nullptr;
    Hist1D                           * histTime   = nullptr;

    bool bDebug = false;

    void blurTime(double & time);
};

#endif // writer_h
