#include "DepositionNodeRecord.hh"
#include "EventRecord.hh"
#include "Reader.hh"
#include "Clusterer.hh"
#include "EventBuilder.hh"
#include "Writer.hh"
#include "out.hh"

#include <string>
#include <vector>

int main(int argc, char** argv)
{
    // --- Start of user inits ---

    //std::string inputFileName  = "/home/andr/WORK/TPPT/SimOutput.bin";     bool bBinaryInput = true;
    //std::string inputFileName  = "/home/andr/WORK/TPPT/SimOutput.txt";     bool bBinaryInput = false;
    //std::string outputFileName = "/home/andr/WORK/TPPT/BuilderOutput.bin"; bool bBinaryOutput = true;
    //std::string outputFileName = "/home/andr/WORK/TPPT/BuilderOutput.txt"; bool bBinaryOutput = false;

    std::string inputFileName  = "/data/margarida/Data/SimOutput.bin";     bool bBinaryInput = true;
    //std::string inputFileName  = "/data/margarida/Data/SimOutput.txt";     bool bBinaryInput = false;
    std::string outputFileName = "/data/margarida/Data/BuilderOutput.bin"; bool bBinaryOutput = true;
    //std::string outputFileName = "/data/margarida/Data/BuilderOutput.txt"; bool bBinaryOutput = false;

    double CTR             = 0.2; // coincidence timing resolution in ns!
    long   Seed            = 100;

    double clusterTime     = 0.1;
    double roughEnergyMin  = 0.311;
    double roughEnergyMax  = 0.711;
    double integrationTime = 40.0;
    double deadTime        = 100.0;

    bool bDebug = false;

    // --- End of user inits

    const int numScint = 6144;

    std::vector<std::vector<DepositionNodeRecord>> Nodes;
    Nodes.resize(numScint);
    std::vector<std::vector<double>> ScintPos;
    ScintPos.resize(numScint);
    for (int iScint = 0; iScint < numScint; iScint++) ScintPos[iScint].resize(3);
    std::vector<std::vector<EventRecord>> Events;
    Events.resize(Nodes.size());

    Reader reader(inputFileName, bBinaryInput);
    reader.bDebug = bDebug;
    std::string error = reader.read(Nodes, ScintPos);
    if (!error.empty())
    {
        out(error);
        exit(2);
    }

    Clusterer clusterer(Nodes, clusterTime);
    clusterer.bDebug = bDebug;
    clusterer.cluster();

    EventBuilder builder(Nodes, integrationTime, deadTime);
    builder.bDebug = bDebug;
    builder.buildEvents(Events);

    Writer writer(outputFileName, bBinaryOutput, roughEnergyMin, roughEnergyMax, CTR, Seed);
    writer.bDebug = bDebug;
    error = writer.write(Events, ScintPos);
    if (!error.empty())
    {
        out(error);
        exit(3);
    }
}
