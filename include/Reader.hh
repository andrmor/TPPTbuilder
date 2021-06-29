#ifndef reader_h
#define reader_h

#include "DepositionNodeRecord.hh"

#include <vector>
#include <string>

class Reader
{
public:
    Reader(const std::string & dir, const std::vector<std::string> & fileNames, bool binary);

    void read(const std::pair<double, double> & timeRange, std::vector<std::vector<DepositionNodeRecord>> & Nodes); //timeRange: <from,to> in ns

    bool bDebug = false;

private:
    std::string Dir;
    std::vector<std::string> FileNames;
    bool bBinary = false;
};

#endif // reader_h
