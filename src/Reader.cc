#include "Reader.hh"
#include "out.hh"

#include <fstream>
#include <sstream>

Reader::Reader(const std::string & FileName, bool Binary) :
    bBinary(Binary)
{
    if (bDebug)
    {
        out("Input file:", FileName);
        out("->Opening stream...");
    }

    if (bBinary) inStream = new std::ifstream(FileName, std::ios::in | std::ios::binary);
    else         inStream = new std::ifstream(FileName);

    if (!inStream->is_open() || inStream->fail() || inStream->bad())
    {
        if (bDebug) out("<-Failed!");
        delete inStream; inStream = nullptr;
    }
    else
    {
        if (bDebug) out("<-Open");
    }
}

std::string Reader::read(std::vector<std::vector<DepositionNodeRecord> > & Nodes, std::vector<std::vector<double>> & ScintPos)
{
    if (!inStream)                       return "Cannot open input file";
    if (Nodes.size() != ScintPos.size()) return "Missmatch in Nodes and ScintPos vectors";

    if (bDebug) out("->Reading input file...");

    if (bBinary)
    {

    }
    else
    {
        std::string line;

        int iScint;
        char dummy;
        double x, y, z;
        double time, depo;

        while (!inStream->eof())
        {
            getline(*inStream, line);
            if (bDebug) out(line);
            if (line.empty()) break;

            std::stringstream ss(line);

            if (line[0] == '#')
            {
                //new scintillator
                ss >> dummy >> iScint >> x >> y >> z;
                if (iScint < 0 || iScint >= Nodes.size()) return "Bad scintillator index";
                ScintPos[iScint][0] = x;
                ScintPos[iScint][1] = y;
                ScintPos[iScint][2] = z;
                if (bDebug) out("Scint #", iScint, "pos:",x,y,z);
            }
            else
            {
                //another node
                ss >> time >> depo;
                Nodes[iScint].push_back( DepositionNodeRecord(time, depo) );
                if (bDebug) out("Extracted values:", time, depo);
            }
        }
    }

    inStream->close();
    if (bDebug) out("\n<-Read completed\n");
    return "";
}
