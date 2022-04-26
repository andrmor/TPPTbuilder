#ifndef gaussblur_h
#define gaussblur_h

#include "EventRecord.hh"

#include <vector>
#include <random>

class Configuration;

class GaussBlur
{
public:
    GaussBlur();

    void applyBlur(std::vector<std::vector<EventRecord>> & events) const;

    double blurValue(double val) const;

private:
    const Configuration & Config;

    std::mt19937_64 * RandEngine = nullptr;

};

#endif // gaussblur_h
