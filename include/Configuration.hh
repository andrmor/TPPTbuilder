#ifndef configuration_h
#define configuration_h

#include <string>
#include <vector>

class Configuration
{
public:
    static Configuration & getInstance();

private:
    Configuration(){}

public:
    Configuration(Configuration const&) = delete;
    void operator=(Configuration const&) = delete;

    void saveConfig(const std::string & fileName) const;
    void loadConfig(const std::string & fileName);

    std::string WorkingDirectory;

    std::vector<std::string> InputFileNames;
    bool BinaryInput  = false;

    std::string OutputFileName;
    bool BinaryOutput = false;

    double CTR             = 0.2;     // coincidence timing resolution in ns
    int    Seed            = 100;     // int due to json11

    double ClusterTime     = 0.1;     // in ns
    double RoughEnergyMin  = 0.311;   // in MeV
    double RoughEnergyMax  = 0.711;   // in MeV
    double IntegrationTime = 40.0;    // in ns
    double DeadTime        = 100.0;   // in ns

    std::vector<std::pair<double,double>> TimeRanges;

    // not saved / loaded:
    const int NumScint = 6144;

private:
    int isDirExist(const std::string & dirName);
    int isFileExist(const std::string & fileName);
};

#endif // configuration_h