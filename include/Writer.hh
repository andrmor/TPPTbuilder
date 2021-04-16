#ifndef writer_h
#define writer_h

#include "EventRecord.hh"

#include <string>
#include <vector>

namespace name {
class ofstream;
}

class Writer
{
public:
    Writer(const std::string & FileName, bool Binary);

    std::string write(std::vector<std::vector<EventRecord>> & Events,
                      std::vector<std::vector<double>> & ScintPos,
                      double energyMin, double energyMax); // returns error string, empty if success

    bool bDebug = true;

private:
    std::ofstream * outStream = nullptr;
    bool bBinary = false;
};

#endif // writer_h
