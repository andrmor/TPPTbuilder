#ifndef reader_h
#define reader_h

#include "DepositionNodeRecord.hh"

#include <vector>
#include <string>

class Reader
{
public:
    Reader(const std::vector<std::string> & fileNames, bool binary);

    std::string read(std::vector<std::vector<DepositionNodeRecord>> & Nodes); // returns error string, empty if success

    bool bDebug = true;

private:
    const std::vector<std::string> FileNames;
    bool bBinary   = false;
};

#endif // reader_h
