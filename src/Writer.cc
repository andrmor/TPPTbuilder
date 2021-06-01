#include "Writer.hh"
#include "Hist1D.hh"
#include "out.hh"

#include <fstream>
#include <iostream>
#include <sstream>

Writer::Writer(const std::string &FileName, bool Binary, double EnergyMin, double EnergyMax, double CTR, long seed) :
    bBinary(Binary), energyMin(EnergyMin), energyMax(EnergyMax), ctr(CTR)
{
    if (bDebug)
    {
        out("Output file:", FileName);
        out("->Opening stream...");
    }

    outStream = new std::ofstream();

    if (bBinary) outStream->open(FileName, std::ios::out | std::ios::binary);
    else outStream->open(FileName);

    if (!outStream->is_open() || outStream->fail() || outStream->bad())
    {
        if (bDebug) out("<-Failed!");
        delete outStream; outStream = nullptr;
    }
    else
    {
        if (bDebug) out("<-Open");
    }

    if (ctr != 0)
    {
        //https://channel9.msdn.com/Events/GoingNative/2013/rand-Considered-Harmful
        //using mersenne twister engine
        randEngine = new std::mt19937_64(seed);
        gauss = new std::normal_distribution<double>(0, CTR / 2.355 / sqrt(2.0));
        //for (int i=0; i<10;i++) out( (*gauss)(*randEngine) );
    }
}

Writer::~Writer()
{
    delete gauss;
    delete randEngine;
}

std::string Writer::write(std::vector<std::vector<EventRecord> > & Events,
                          std::vector<std::vector<double>> & ScintPos)
{
    if (!outStream)                       return "Cannot open input file";
    if (Events.size() != ScintPos.size()) return "Missmatch in Events and ScintPos vectors";

    if (bDebug) out("->Writing events to file...");

    for (int iScint = 0; iScint < Events.size(); iScint++)
    {
        std::vector<EventRecord> & evec = Events[iScint];
        if (evec.empty()) continue;

        // scint header
        if (bBinary)
        {
            *outStream << char(0xEE);
            outStream->write((char*)&iScint,                     sizeof(int));
            outStream->write((char*)ScintPos[iScint].data(), 3 * sizeof(double));
        }
        else
            *outStream << "# " << iScint << ' ' << ScintPos[iScint][0] << ' ' << ScintPos[iScint][1] << ' ' << ScintPos[iScint][2] << "\n";

        // events
        for (EventRecord & ev : evec)
        {
            if (ev.energy < energyMin || ev.energy > energyMax) continue;

            if (ctr != 0) blurTime(ev.time);

            if (bBinary)
            {
                *outStream << char(0xFF);
                outStream->write((char*)&ev.time,   sizeof(double));
                outStream->write((char*)&ev.energy, sizeof(double));
            }
            else
                *outStream << ev.time << " " << ev.energy << '\n';
        }
    }

    outStream->close();
    if (bDebug) out("\n<-Write completed\n");

    if (bSaveEnergyDist) saveEnergyDist(Events);

    if (bSaveTimeDist) saveTimeDist(Events);

    return "";
}

void Writer::blurTime(double & time)
{
    time += (*gauss)(*randEngine);
}

void Writer::saveEnergyDist(std::vector<std::vector<EventRecord> > & Events)
{
    Hist1D Hist(1000, 0, 1.0);

    for (int iScint = 0; iScint < Events.size(); iScint++)
    {
        std::vector<EventRecord> & evec = Events[iScint];
        if (evec.empty()) continue;

        for (EventRecord & ev : evec) Hist.fill(ev.energy);
    }

    Hist.report();
    Hist.save(EnergyDistFileName);
}

void Writer::saveTimeDist(std::vector<std::vector<EventRecord> > & Events)
{
    Hist1D Hist(100, 0, 2e+12);

    for (int iScint = 0; iScint < Events.size(); iScint++)
    {
        std::vector<EventRecord> & evec = Events[iScint];
        if (evec.empty()) continue;

        for(EventRecord & ev : evec) Hist.fill(ev.time);
    }

    Hist.report();
    Hist.save(TimeDistFileName);
}
