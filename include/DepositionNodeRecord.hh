#ifndef depositionnoderecord_h
#define depositionnoderecord_h

struct DepositionNodeRecord
{
    DepositionNodeRecord(double Time, double Energy);
    DepositionNodeRecord(){}

    void merge(const DepositionNodeRecord & other);
    bool isCluster(const DepositionNodeRecord & other, double maxTimeDelta) const;

    bool operator<(const DepositionNodeRecord & other) const;

    double time;
    double energy = 0;
};

#endif // depositionnoderecord_h
