#ifndef eventbuilder_h
#define eventbuilder_h

#include "DepositionNodeRecord.hh"
#include "EventRecord.hh"

#include <vector>

class Configuration;

class EventBuilder
{
public:
    EventBuilder(std::vector<std::vector<DepositionNodeRecord>> & clusters);

    void buildEvents(std::vector<std::vector<EventRecord>> & events);

    bool bDebug = false;

private:
    const Configuration & Config;
    std::vector< std::vector<DepositionNodeRecord>> & Clusters;

};

#endif // eventbuilder_h
