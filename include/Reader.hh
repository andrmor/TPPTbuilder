#ifndef reader_h
#define reader_h

#include "DepositionNodeRecord.hh"

#include <vector>
#include <string>

class Configuration;

class Reader
{
public:
    Reader();

    void read(const std::pair<double, double> & timeRange, std::vector<std::vector<DepositionNodeRecord>> & Nodes); //timeRange: <from,to> in ns

private:
    const Configuration & Config;

    bool bDebug = false;
};

#endif // reader_h
