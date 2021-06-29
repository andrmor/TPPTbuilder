#ifndef eventbuilder_h
#define eventbuilder_h

#include "DepositionNodeRecord.hh"
#include "EventRecord.hh"

#include <vector>

class EventBuilder
{
public:
    EventBuilder(std::vector<std::vector<DepositionNodeRecord>> & clusters, double integrationTime, double deadTime);

    void buildEvents(std::vector<std::vector<EventRecord>> & events);

    bool bDebug = false;

private:
    std::vector< std::vector<DepositionNodeRecord>> & Clusters;

    double IntegrationTime = 1.0;
    double DeadTime = 100.0;
};

#endif // eventbuilder_h
