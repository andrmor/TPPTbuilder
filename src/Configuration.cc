#include "Configuration.hh"
#include "out.hh"
#include "json11.hh"
#include "jstools.hh"

#include <iostream>
#include <fstream>

Configuration & Configuration::getInstance()
{
    static Configuration instance; // Guaranteed to be destroyed, instantiated on first use.
    return instance;
}

void Configuration::saveConfig(const std::string & fileName) const
{
    json11::Json::object json;

    json["WorkingDirectory"] = WorkingDirectory;

    json11::Json::array ar;
    for (const std::string & el : InputFileNames) ar.push_back(el);
    json["InputFileNames"]  = ar;
    json["BinaryInput"]     = BinaryInput;

    json["OutputFileName"]  = OutputFileName;
    json["BinaryOutput"]    = BinaryOutput;

    json["CTR"]             = CTR;
    json["EnergyResolution"]= EnergyResolution;
    json["Seed"]            = Seed;

    json["ClusterTime"]     = ClusterTime;
    json["EnergyThreshold"] = EnergyThreshold;
    json["IntegrationTime"] = IntegrationTime;
    json["DeadTime"]        = DeadTime;

    json11::Json::array arTR;
    for (const std::pair<double,double> & p : TimeRanges)
    {
        json11::Json::array arP;
            arP.push_back(p.first);
            arP.push_back(p.second);
        arTR.push_back(arP);
    }
    json["TimeRanges"]      = arTR;

    std::string json_str = json11::Json(json).dump();
    std::ofstream confStream;
    confStream.open(WorkingDirectory + '/' + fileName);
    if (confStream.is_open())
        confStream << json_str << std::endl;
    confStream.close();
}

#include <sstream>
void Configuration::loadConfig(const std::string & fileName)
{
    if (!isFileExist(fileName))
    {
        out("Config file", fileName, "does not exist or cannot be open!");
        exit(1);
    }

    out("\nReading config file:", fileName);
    std::ifstream in(fileName);
    std::stringstream sstr;
    sstr << in.rdbuf();
    in.close();
    std::string cs = sstr.str();

    std::string err;
    json11::Json json = json11::Json::parse(cs, err);
    if (!err.empty())
    {
        out(err);
        exit(2);
    }

    jstools::readString(json, "WorkingDirectory", WorkingDirectory);
    if (!isDirExist(WorkingDirectory))
    {
        out("Directory does not exist:", WorkingDirectory);
        exit(3);
    }

    InputFileNames.clear();
    json11::Json::array ar;
    jstools::readArray(json, "InputFileNames", ar);
    for (size_t i = 0; i < ar.size(); i++)
    {
        const json11::Json & arEl = ar[i];
        const std::string ci = arEl.string_value();
        InputFileNames.push_back(ci);
    }
    jstools::readBool  (json, "BinaryInput",     BinaryInput);

    jstools::readString(json, "OutputFileName",  OutputFileName);
    jstools::readBool  (json, "BinaryOutput",    BinaryOutput);

    jstools::readDouble(json, "CTR",             CTR);
    jstools::readDouble(json, "EnergyResolution",EnergyResolution);
    jstools::readInt   (json, "Seed",            Seed);

    jstools::readDouble(json, "ClusterTime",     ClusterTime);
    jstools::readDouble(json, "EnergyThreshold", EnergyThreshold);
    jstools::readDouble(json, "IntegrationTime", IntegrationTime);
    jstools::readDouble(json, "DeadTime",        DeadTime);

    TimeRanges.clear();
    json11::Json::array arTR;
    jstools::readArray(json, "TimeRanges", arTR);
    for (size_t i = 0; i < arTR.size(); i++)
    {
        const json11::Json & arEl = arTR[i];
        const json11::Json::array arP = arEl.array_items();
        if (arP.size() != 2)
        {
            out("Config file: bad format of TimeRanges array");
            exit(3);
        }
        double from = arP[0].number_value();
        double to   = arP[1].number_value();
        TimeRanges.push_back({from, to});
    }
}

#include <sys/types.h>
#include <sys/stat.h>
int Configuration::isDirExist(const std::string & dirName)
{
    struct stat info;

    if (stat(dirName.data(), &info) != 0) return false;
    else if (info.st_mode & S_IFDIR)      return true;
    else                                  return false;
}

int Configuration::isFileExist(const std::string &fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

