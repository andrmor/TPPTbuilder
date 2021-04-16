#ifndef eventrecord_h
#define eventrecord_h

#include "DepositionNodeRecord.hh"

struct EventRecord
{
    EventRecord(double Time, double Energy);

    void merge(const DepositionNodeRecord & cluster);

    bool operator<(const EventRecord & other) const;

    double time;
    double energy;
};

#endif // eventrecord_h
