#include "Configuration.hh"
#include "out.hh"

Configuration & Configuration::getInstance()
{
    static Configuration instance; // Guaranteed to be destroyed, instantiated on first use.
    return instance;
}

void Configuration::saveConfig(const std::string & filename) const
{

}

void Configuration::loadConfig(const std::string & filename)
{
    out("Not implemented yet!");
    exit(100);
}

