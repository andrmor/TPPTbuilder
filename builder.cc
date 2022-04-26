#include "Configuration.hh"
#include "DepositionNodeRecord.hh"
#include "EventRecord.hh"
#include "Reader.hh"
#include "Clusterer.hh"
#include "EventBuilder.hh"
#include "GaussBlur.hh"
#include "Writer.hh"
#include "out.hh"

#include <string>
#include <vector>

int main(int argc, char** argv)
{
    Configuration & Config = Configuration::getInstance();

    std::string filename;
    if (argc > 1)
    {
        filename = std::string(argv[1]);
        out("\nLoading config from file:", filename);
    }
    else out("\nNo config file provided as argument, using configuration defined in the main of the builder");

        // warning: automatically saves config (if no errors) in working directory as BuilderConfig.json
        // beware of possible overright!

        //here you can directly provide the config file name
    //filename = "/home/andr/WORK/TPPT/BuilderConfig1.json";

    if (!filename.empty())
    {
        Config.loadConfig(filename);
    }
    else
    {
        // --- Start of user inits ---

        Config.WorkingDirectory = "/home/andr/WORK/TPPT";
        //Config.WorkingDirectory = "/data/margarida/Data";

        Config.BinaryInput    = true;  Config.InputFileNames = {"SimOutput.bin"};
        //Config.BinaryInput    = false; Config.InputFileNames = {"SimOutput.txt"};
        //Config.InputFileNames = {"SimOutput-NatRad-0-0m1m.bin"};
        /*
        Config.inputFileNames = {"SimOutput-NatRad-0-0m1m.bin",
                                  "SimOutput-NatRad-1-1m2m.bin",
                                  "SimOutput-NatRad-2-2m3m.bin",
                                  "SimOutput-NatRad-3-3m4m.bin",
                                  "SimOutput-NatRad-4-4m5m.bin"};
        */

        Config.BinaryOutput    = true; Config.OutputFileName = "BuilderOutput.bin";
        //Config.BinaryOutput   = false; Config.OutputFileName = "BuilderOutput.txt";

        Config.Seed             = 100;

        Config.ClusterTime      = 0.1;   // ns

        Config.IntegrationTime  = 40.0;  // ns
        Config.DeadTime         = 100.0; // ns

        Config.CTR              = 0.2;  // coincidence timing resolution in ns!
        Config.EnergyResolution = 0.13; // energy resolution (fraction, FWHM)

        Config.RoughEnergyMin   = 0.311;
        Config.RoughEnergyMax   = 0.711;

        Config.TimeRanges = { {0, 1e50} }; // no filter and splitting
        //Config.TimeRanges = { {0, 1e10}, {1e10, 2e10}, {2e10, 3e10}, {3e10, 4e10}, {4e10, 5e10}, {5e10, 6e10} };
        //Config.TimeRanges = { {0, 6e10}, {6e10, 12e10}, {12e10, 18e10}, {18e10, 24e10}, {24e10, 30e10} };

        // --- End of user inits
    }

    Writer writer;

    for (const std::pair<double,double> & timeRange : Config.TimeRanges)
    {
        out("Processing time range from", timeRange.first, " ns to", timeRange.second, " ns");
        std::vector<std::vector<DepositionNodeRecord>> Nodes;  Nodes. resize(Config.NumScint);
        std::vector<std::vector<EventRecord>>          Events; Events.resize(Config.NumScint);

        Reader reader;
        reader.read(timeRange, Nodes);
        out("Reading completed");

        Clusterer clusterer(Nodes);
        clusterer.cluster();
        out("Clustering completed");

        EventBuilder builder(Nodes);
        builder.buildEvents(Events);
        out("Event building completed");

        if (Config.EnergyResolution != 0)
        {
            GaussBlur Blur;
            Blur.applyBlur(Events);
            out("Energy blurring completed");
        }

        writer.write(Events);
    }

    writer.saveEnergyDist("Builder-Energy.txt");
    writer.saveTimeDist("Builder-Time.txt");

    Config.saveConfig("BuilderConfig.json");
}
