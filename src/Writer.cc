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

    for (int iScint = 0; iScint < Events.size(); iScint++)
    {
        const std::vector<EventRecord> & evec = Events[iScint];
        if (evec.empty()) continue;

        if (bBinary)
        {
            *outStream << char(0xEE);
            outStream->write((char*)&iScint,                     sizeof(int));
            outStream->write((char*)ScintPos[iScint].data(), 3 * sizeof(double));

            for (const EventRecord & ev : evec)
            {
                if (ev.energy < energyMin || ev.energy > energyMax) continue;
                *outStream << char(0xFF);
                outStream->write((char*)&ev.time,   sizeof(double));
                outStream->write((char*)&ev.energy, sizeof(double));
            }
        }
        else
        {
            *outStream << "# " << iScint << ' ' << ScintPos[iScint][0] << ' ' << ScintPos[iScint][1] << ' ' << ScintPos[iScint][2] << "\n";

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
