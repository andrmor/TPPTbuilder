#include "GaussBlur.hh"
#include "Configuration.hh"
#include "out.hh"

#include <string>
#include <fstream>

GaussBlur::GaussBlur() :
    Config(Configuration::getInstance())
{
        out("Starting random engine for energy blurring");
        //https://channel9.msdn.com/Events/GoingNative/2013/rand-Considered-Harmful
        //using mersenne twister engine

        RandEngine = new std::mt19937_64(Config.Seed + 1);

        //test random generator
        /*
        std::string fn = Config.WorkingDirectory + "/blurTest.txt";
        std::ofstream outStream;
        outStream.open(fn);
        for (int i=0; i<10000; i++)
        {
            outStream << blurValue(0.511) << '\n';
            outStream << blurValue(0.200) << '\n';
        }
        outStream.close();
        exit(111);
        */
}

void GaussBlur::applyBlur(std::vector<std::vector<EventRecord>> & events) const
{
    for (std::vector<EventRecord> & evec : events)
        for (EventRecord & ev : evec)
            ev.energy = blurValue(ev.energy);
}

double GaussBlur::blurValue(double val) const
{
    std::normal_distribution<double> ND{val, val * Config.EnergyResolution / 2.355};
    return ND(*RandEngine);
}
