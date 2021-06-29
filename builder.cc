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

    std::string dir = "/home/andr/WORK/TPPT";
    //std::string dir = "/data/margarida/Data";

    bool bBinaryInput  = true;
    bool bBinaryOutput = true;

    /*
    std::vector<std::string> inputFileNames = {"SimOutput-NatRad-0-0m1m.bin",
                                               "SimOutput-NatRad-1-1m2m.bin",
                                               "SimOutput-NatRad-2-2m3m.bin",
                                               "SimOutput-NatRad-3-3m4m.bin",
                                               "SimOutput-NatRad-4-4m5m.bin"};
    */

    std::vector<std::string> inputFileNames = {"SimOutput-NatRad-0-0m1m.bin"};

    //std::vector<std::string> inputFileNames = {"SimOutput.bin"};
    //std::vector<std::string> inputFileNames = {"SimOutput.txt"};
    std::string outputFileName = "BuilderOutput.bin";
    //std::string outputFileName = "BuilderOutput.txt";

    double CTR             = 0.2; // coincidence timing resolution in ns!
    long   Seed            = 100;

    double clusterTime     = 0.1;
    double roughEnergyMin  = 0.311;
    double roughEnergyMax  = 0.711;
    double integrationTime = 40.0;
    double deadTime        = 100.0;

    //std::vector<std::pair<double,double>> TimeRanges = { {0, 1e50} }; // no filter and splitting
    std::vector<std::pair<double,double>> TimeRanges = { {0, 1e10}, {1e10, 2e10}, {2e10, 3e10}, {3e10, 4e10}, {4e10, 5e10}, {5e10, 6e10} };
    //std::vector<std::pair<double,double>> TimeRanges = { {0, 6e10}, {6e10, 12e10}, {12e10, 18e10}, {18e10, 24e10}, {24e10, 30e10} };

    // --- End of user inits

    Writer writer(dir, outputFileName, bBinaryOutput);
    writer.configure(roughEnergyMin, roughEnergyMax, CTR, Seed);
    if (!writer.isOK())
    {
        out("Failed to create output file", outputFileName, "in", dir);
        exit(1);
    }

    const int numScint = 6144;
    for (const std::pair<double,double> & timeRange : TimeRanges)
    {
        out("Processing time range from", timeRange.first, " ns to", timeRange.second, " ns");
        std::vector<std::vector<DepositionNodeRecord>> Nodes;  Nodes. resize(numScint);
        std::vector<std::vector<EventRecord>>          Events; Events.resize(numScint);

        Reader reader(dir, inputFileNames, bBinaryInput);
        reader.read(timeRange, Nodes);
        out("Reading completed");

        Clusterer clusterer(Nodes, clusterTime);
        clusterer.cluster();
        out("Clustering completed");

        EventBuilder builder(Nodes, integrationTime, deadTime);
        builder.buildEvents(Events);
        out("Event building completed");

        writer.write(Events);
    }

    writer.saveEnergyDist("Builder-Energy.txt");
    writer.saveTimeDist("Builder-Time.txt");
}
