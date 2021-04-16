#include "EventRecord.hh"

#include <math.h>
#include <algorithm>

EventRecord::EventRecord(double Time, double Energy) :
    time(Time), energy(Energy) {}

void EventRecord::merge(const DepositionNodeRecord & cluster)
{
    if (cluster.energy <= 0) return;
    energy = energy + cluster.energy;
    time = std::min(time, cluster.time);
}

bool EventRecord::operator<(const EventRecord & other) const
{
    return (time < other.time);
}
