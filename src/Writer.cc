#include "Writer.hh"
#include "Hist1D.hh"
#include "out.hh"

#include <fstream>
#include <iostream>
#include <sstream>

Writer::Writer(const std::string & dir, const std::string & fileName, bool Binary) :
    Dir(dir), FileName(dir + '/' + fileName), bBinary(Binary)
{
    if (bDebug)
    {
        out("Output file:", FileName);
        out("->Opening stream...");
    }

    outStream = new std::ofstream();

    if (bBinary) outStream->open(FileName, std::ios::out | std::ios::binary);
    else         outStream->open(FileName);

    if (!outStream->is_open() || outStream->fail() || outStream->bad())
    {
        if (bDebug) out("<-Failed!");
        delete outStream; outStream = nullptr;
    }
    else
        if (bDebug) out("<-Open");

    histEnergy = new Hist1D(1000, 0, 1.0);    // [MeV]
    histTime   = new Hist1D(100,  0, 2e+12);  // [ns]
}

void Writer::configure(double energyMin, double energyMax, double ctr, long seed)
{
     EnergyMin = energyMin;
     EnergyMax = energyMax;
     CTR       = ctr;

     if (CTR != 0)
     {
         //if (bDebug) out("Starting random engine");
         //https://channel9.msdn.com/Events/GoingNative/2013/rand-Considered-Harmful
         //using mersenne twister engine

         delete randEngine; randEngine = new std::mt19937_64(seed);
         delete gauss;      gauss      = new std::normal_distribution<double>(0, CTR / 2.355 / sqrt(2.0));

         //test random generator
         //for (int i=0; i<10;i++) out( (*gauss)(*randEngine) );
     }
}

Writer::~Writer()
{
    delete histTime;
    delete histEnergy;

    delete gauss;
    delete randEngine;

    outStream->close();
    delete outStream;
}

void Writer::write(std::vector<std::vector<EventRecord> > & Events)
{
    if (!outStream)
    {
        if (bDebug) "Output stream does not exist!";
        exit(1);
    }

    if (bDebug) out("->Writing events to file...");

    for (int iScint = 0; iScint < Events.size(); iScint++)
    {
        std::vector<EventRecord> & evec = Events[iScint];
        if (evec.empty()) continue;

        // scint header
        if (bBinary)
        {
            *outStream << char(0xEE);
            outStream->write((char*)&iScint, sizeof(int));
        }
        else
            *outStream << "# " << iScint << std::endl;

        // events
        for (EventRecord & ev : evec)
        {
            if (ev.energy < EnergyMin || ev.energy > EnergyMax) continue;

            if (CTR != 0) blurTime(ev.time);

            if (bBinary)
            {
                *outStream << char(0xFF);
                outStream->write((char*)&ev.time,   sizeof(double));
                outStream->write((char*)&ev.energy, sizeof(double));
            }
            else
                *outStream << ev.time << " " << ev.energy << '\n';

            histEnergy->fill(ev.energy);
            histTime->fill(ev.time);
        }
    }

    if (bDebug) out("\n<-Write completed\n");
}

void Writer::blurTime(double & time)
{
    time += (*gauss)(*randEngine);
}

void Writer::saveEnergyDist(const std::string & fileName)
{
    out("Energy distribution of saved events (axis from 0 to 1 MeV, 1000 bins):"); //TODO request data from hist
    histEnergy->report();
    histEnergy->save(Dir + '/' + fileName);
}

void Writer::saveTimeDist(const std::string & fileName)
{
    out("Time distribution (from 0 to 2e12 ns):");  //TODO request data from hist
    histTime->report();
    histTime->save(Dir + '/' + fileName);
}
