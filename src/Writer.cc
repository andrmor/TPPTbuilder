#include "Writer.hh"
#include "Configuration.hh"
#include "Hist1D.hh"
#include "out.hh"

#include <fstream>
#include <iostream>
#include <sstream>

Writer::Writer() : Config(Configuration::getInstance())
{
    const std::string FileName = Config.WorkingDirectory + '/' + Config.OutputFileName;
    out("Opening output stream, file:", FileName);

    outStream = new std::ofstream();
    if (Config.BinaryOutput) outStream->open(FileName, std::ios::out | std::ios::binary);
    else                     outStream->open(FileName);

    if (!outStream->is_open() || outStream->fail() || outStream->bad())
    {
        out("Failed!");
        //delete outStream; outStream = nullptr;
        exit(1);
    }

    if (Config.CTR != 0)
    {
        out("Starting random engine");
        //https://channel9.msdn.com/Events/GoingNative/2013/rand-Considered-Harmful
        //using mersenne twister engine

        randEngine = new std::mt19937_64(Config.Seed);
        gauss      = new std::normal_distribution<double>(0, Config.CTR / 2.355 / sqrt(2.0));

        //test random generator
        //for (int i=0; i<10;i++) out( (*gauss)(*randEngine) );
    }

    histEnergy = new Hist1D(1000, 0, 1.0);    // [MeV]
    histTime   = new Hist1D(100,  0, 2e+12);  // [ns]
}

Writer::~Writer()
{
    delete histTime;
    delete histEnergy;

    delete gauss;
    delete randEngine;

    if (outStream) outStream->close();
    delete outStream;
}

void Writer::write(std::vector<std::vector<EventRecord> > & Events)
{
    if (!outStream)
    {
        if (bDebug) out("Output stream does not exist!");
        exit(1);
    }

    if (bDebug) out("->Writing events to file...");

    for (int iScint = 0; iScint < Events.size(); iScint++)
    {
        std::vector<EventRecord> & evec = Events[iScint];
        if (evec.empty()) continue;

        // scint header
        if (Config.BinaryOutput)
        {
            *outStream << char(0xEE);
            outStream->write((char*)&iScint, sizeof(int));
        }
        else
            *outStream << "# " << iScint << std::endl;

        // events
        for (EventRecord & ev : evec)
        {
            if (ev.energy < Config.RoughEnergyMin || ev.energy > Config.RoughEnergyMax) continue;

            if (Config.CTR != 0) blurTime(ev.time);

            if (Config.BinaryOutput)
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
    histEnergy->save(Config.WorkingDirectory + '/' + fileName);
}

void Writer::saveTimeDist(const std::string & fileName)
{
    out("Time distribution (from 0 to 2e12 ns):");  //TODO request data from hist
    histTime->report();
    histTime->save(Config.WorkingDirectory + '/' + fileName);
}
