#include "Writer.hh"
#include "out.hh"

#include <fstream>
#include <iostream>
#include <sstream>

Writer::Writer(const std::string &FileName, bool Binary) :
    bBinary(Binary)
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
}

std::string Writer::write(std::vector<std::vector<EventRecord> > & Events,
                          std::vector<std::vector<double>> & ScintPos,
                          double energyMin, double energyMax)
{
    if (!outStream)                       return "Cannot open input file";
    if (Events.size() != ScintPos.size()) return "Missmatch in Events and ScintPos vectors";

    if (bDebug) out("->Writing events to file...");

    if (bBinary)
    {

    }
    else
    {
        for (int iEvent = 0; iEvent < Events.size(); iEvent++)
        {
            const std::vector<EventRecord> & evec = Events[iEvent];
            if (evec.empty()) continue;

            *outStream << "# " << iEvent << ' ' << ScintPos[iEvent][0] << ' ' << ScintPos[iEvent][1] << ' ' << ScintPos[iEvent][2] << "\n";

            for (const EventRecord & ev : evec)
            {
                if (ev.energy < energyMin || ev.energy > energyMax) continue;
                *outStream << ev.time << " " << ev.energy << '\n';
            }
        }
    }

    outStream->close();
    if (bDebug) out("\n<-Write completed\n");
    return "";
}
