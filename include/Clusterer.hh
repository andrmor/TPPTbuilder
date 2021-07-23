#ifndef clusterer_h
#define clusterer_h

#include "DepositionNodeRecord.hh"

#include <vector>

class Clusterer
{
public:
    Clusterer(std::vector< std::vector<DepositionNodeRecord>> & Nodes, double MaxTimeDelta);

    void cluster();

    bool bDebug = false;

private:
    std::vector< std::vector<DepositionNodeRecord>> & Nodes;

    double maxTimeDelta    = 1.0;

private:
    int  doPrecluster(std::vector<DepositionNodeRecord> & nvec);
    int  doCluster   (std::vector<DepositionNodeRecord> & nvec);

    void outNodes(const std::vector<DepositionNodeRecord> &nvec);
};

#endif // clusterer_h
