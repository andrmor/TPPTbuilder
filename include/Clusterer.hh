#ifndef clusterer_h
#define clusterer_h

#include "DepositionNodeRecord.hh"

#include <vector>

class Configuration;

class Clusterer
{
public:
    Clusterer(std::vector< std::vector<DepositionNodeRecord>> & Nodes);

    void cluster();

    bool bDebug = false;

private:
    const Configuration & Config;
    std::vector< std::vector<DepositionNodeRecord>> & Nodes;

private:
    int  doPrecluster(std::vector<DepositionNodeRecord> & nvec);
    int  doCluster   (std::vector<DepositionNodeRecord> & nvec);

    void outNodes(const std::vector<DepositionNodeRecord> &nvec);
};

#endif // clusterer_h
