#include "Clusterer.hh"
#include "Configuration.hh"
#include "out.hh"

#include <algorithm>

Clusterer::Clusterer(std::vector<std::vector<DepositionNodeRecord>> & Nodes) :
    Config(Configuration::getInstance()),
    Nodes(Nodes) {}

void Clusterer::cluster()
{
    out("Starting clustering (", Config.ClusterTime, "ns)");

    int numMergesPreclusterGlobal = 0;
    int numMergesClusterGlobal = 0;
    for (int iScint = 0; iScint < Nodes.size(); iScint++)
    {
        //out("Clustering for scint #:", iScint);
        std::vector<DepositionNodeRecord> & nvec = Nodes[iScint];
        if (nvec.empty()) continue;

        if (bDebug)
        {
            out("==> #", iScint, " nodes:", nvec.size());
            out(" --- before clustering:");
            outNodes(nvec);
        }

        int numMerges = doPrecluster(nvec);
        if (bDebug) out("Number of merges in preclustering:", numMerges);
        numMergesPreclusterGlobal += numMerges;

        std::sort(nvec.begin(), nvec.end());

        do
        {
            numMerges = doCluster(nvec);
            numMergesClusterGlobal += numMerges;
            if (bDebug) out("Number of merges in clustering:", numMerges);
        }
        while (numMerges != 0);

        if (bDebug)
        {
            out(" --- after clustering:");
            outNodes(Nodes[iScint]);
        }
    }
    out("-- Num precluster merges:", numMergesPreclusterGlobal);
    out("-- Num clustering merges:", numMergesClusterGlobal);

    if (bDebug) out("\n<-Clustering done\n");
}

int Clusterer::doPrecluster(std::vector<DepositionNodeRecord> & nvec)
{
    int iMergeCounter = 0;

    std::vector<DepositionNodeRecord> nvecClustered;
    nvecClustered.reserve(nvec.size());
    nvecClustered.push_back(nvec.front());

    for (int iNode = 1; iNode < nvec.size(); iNode++)
    {
        const DepositionNodeRecord & newNode = nvec[iNode];

        if (nvecClustered.back().isCluster(newNode, Config.ClusterTime))
        {
            nvecClustered.back().merge(newNode);
            iMergeCounter++;
        }
        else
            nvecClustered.push_back(newNode);
    }

    nvec = nvecClustered;
    return iMergeCounter;
}

int Clusterer::doCluster(std::vector<DepositionNodeRecord> & nvec)
{
    int iMergeCounter = 0;

    std::vector<DepositionNodeRecord> nvecClustered;
    nvecClustered.reserve(nvec.size());
    nvecClustered.push_back(nvec.front());

    for (int iNode = 1; iNode < nvec.size(); iNode++)
    {
        const DepositionNodeRecord & newNode = nvec[iNode];

        bool bFoundOldCluster = false;
        for (DepositionNodeRecord & oldCluster : nvecClustered)
        {
            if (oldCluster.isCluster(newNode, Config.ClusterTime))
            {
                oldCluster.merge(newNode);
                bFoundOldCluster = true;
                iMergeCounter++;
                break;
            }
        }
        if (!bFoundOldCluster) nvecClustered.push_back(newNode);
    }

    nvec = nvecClustered;
    return iMergeCounter;
}

void Clusterer::outNodes(const std::vector<DepositionNodeRecord> & nvec)
{
    for (const DepositionNodeRecord & n : nvec)
        out(n.time, n.energy);
}
