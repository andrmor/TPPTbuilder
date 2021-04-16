#include "DepositionNodeRecord.hh"

#include <math.h>

DepositionNodeRecord::DepositionNodeRecord(double Time, double Energy) :
    time(Time), energy(Energy) {}

void DepositionNodeRecord::merge(const DepositionNodeRecord & other)
{
    if (other.energy <= 0) return;

    const double newEnergy = energy + other.energy;
    time = (time * energy  +  other.time * other.energy) / newEnergy;
    energy = newEnergy;
}

bool DepositionNodeRecord::isCluster(const DepositionNodeRecord &other, double maxTimeDelta) const
{
    if ( fabs(time - other.time) > maxTimeDelta ) return false;
    return true;
}

bool DepositionNodeRecord::operator<(const DepositionNodeRecord &other) const
{
    return (time < other.time);
}
