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

std::string Reader::read(std::vector<std::vector<DepositionNodeRecord> > & Nodes)
{
    if (!inStream)                       return "Cannot open input file";

    if (bDebug) out("->Reading input file...");

    int iScint;

    if (bBinary)
    {
        char ch;
        double x,y,z;
        while (inStream->get(ch))
        {
            if (inStream->eof()) break;

            if (ch == (char)0xEE)
            {
                inStream->read((char*)&iScint, sizeof(int));
                if (iScint < 0 || iScint >= Nodes.size()) return "Bad scintillator index";
            }
            else if (ch == (char)0xFF)
            {
                DepositionNodeRecord node;
                inStream->read((char*)&node.time, sizeof(double));
                inStream->read((char*)&node.energy, sizeof(double));
                if (bDebug) out("Extracted values:", node.time, node.energy);
                Nodes[iScint].push_back(node);
            }
        }
    }
    else
    {
        std::string line;

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
                ss >> dummy >> iScint;
                if (iScint < 0 || iScint >= Nodes.size()) return "Bad scintillator index";
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
