#include "EventBuilder.hh"
#include "out.hh"

#include <algorithm>

EventBuilder::EventBuilder(std::vector<std::vector<DepositionNodeRecord>> & clusters, double integrationTime, double deadTime) :
    Clusters(clusters), IntegrationTime(integrationTime), DeadTime(deadTime) {}

void EventBuilder::buildEvents(std::vector<std::vector<EventRecord>> & events)
{
    if (bDebug) out("->Starting event building (", IntegrationTime, DeadTime, ")...");

    for (int iScint = 0; iScint < Clusters.size(); iScint++)
    {
        //out("Event building for scint #:", iScint);

        std::vector<DepositionNodeRecord> & cvec = Clusters[iScint];
        if (cvec.empty()) continue;

        if (bDebug)
        {
            out("==> #", iScint, " clusters:", cvec.size());
            out(" --- input clusters:");
            for (const DepositionNodeRecord & c : cvec) out(c.time, c.energy);
        }

        std::sort(cvec.begin(), cvec.end());

        std::vector<EventRecord> evVec;
        evVec.reserve(cvec.size());

        evVec.push_back( EventRecord(cvec.front().time, cvec.front().energy) );
        for (int iClust = 1; iClust < cvec.size(); iClust++)
        {
            DepositionNodeRecord & clust     = cvec[iClust];
            EventRecord          & lastEvent = evVec.back();
            if      (clust.time < lastEvent.time + IntegrationTime)
                lastEvent.merge(clust);
            else if (clust.time < lastEvent.time + DeadTime)
                continue;
            else
                evVec.push_back( EventRecord(clust.time, clust.energy) );
        }

        if (bDebug)
        {
            out(" --- constructed hits:");
            for (const EventRecord & e : evVec) out(e.time, e.energy);
        }

        events[iScint] = evVec;
    }

    if (bDebug) out("\n<-Hit building done\n");
}
