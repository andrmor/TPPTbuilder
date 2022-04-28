#include "Reader.hh"
#include "Configuration.hh"
#include "out.hh"

#include <fstream>
#include <sstream>
#include <ios>

Reader::Reader() : Config(Configuration::getInstance()) {}

void Reader::read(const std::pair<double,double> & timeRange, std::vector<std::vector<DepositionNodeRecord> > & Nodes)
{
    //out("Reading input files...\n");
    int iScint;

    for (const std::string & FileName : Config.InputFileNames)
    {
        const std::string fn = Config.WorkingDirectory + '/' + FileName;
        out("    Input file:", fn);

        std::ifstream inStream;
        if (Config.BinaryInput) inStream.open(fn, std::ios::in | std::ios::binary);
        else                    inStream.open(fn);

        if (!inStream.is_open() || inStream.fail() || inStream.bad())
        {
            out("Cannot open input file:\n", fn);
            exit(2);
        }

        if (Config.BinaryInput)
        {
            char ch;
            double time, energy;
            while (inStream.get(ch))
            {
                if (inStream.eof()) break;

                if (ch == (char)0xEE)
                {
                    inStream.read((char*)&iScint, sizeof(int));
                    if (iScint < 0 || iScint >= Nodes.size())
                    {
                        out("Bad scintillator index:", iScint);
                        exit(10);
                    }
                }
                else if (ch == (char)0xFF)
                {
                    inStream.read((char*)&time,   sizeof(double));
                    inStream.read((char*)&energy, sizeof(double));
                    if (bDebug) out("Extracted values:", time, energy);

                    if (time > timeRange.first && time < timeRange.second)
                        Nodes[iScint].push_back(DepositionNodeRecord(time, energy));
                }
            }
        }
        else
        {
            std::string line;

            char dummy;
            double time, energy;

            while (!inStream.eof())
            {
                getline(inStream, line);
                if (bDebug) out(line);
                if (line.empty()) break;

                std::stringstream ss(line);

                if (line[0] == '#')
                {
                    //new scintillator
                    ss >> dummy >> iScint;
                    if (iScint < 0 || iScint >= Nodes.size())
                    {
                        out("Bad scintillator index:", iScint);
                        exit(10);
                    }
                }
                else
                {
                    //another node
                    ss >> time >> energy;
                    if (bDebug) out("Extracted values:", time, energy);

                    if (time > timeRange.first && time < timeRange.second)
                        Nodes[iScint].push_back(DepositionNodeRecord(time, energy));
                }
            }
        }

        inStream.close();
    }
}
