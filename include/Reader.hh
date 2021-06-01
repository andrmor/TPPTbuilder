#ifndef reader_h
#define reader_h

#include "DepositionNodeRecord.hh"

#include <string>
#include <vector>

namespace name {
class ifstream;
}

class Reader
{
public:
    Reader(const std::string & FileName, bool Binary, bool bInputScintPos);

    std::string read(std::vector<std::vector<DepositionNodeRecord>> & Nodes, std::vector<std::vector<double>> & ScintPos); // returns error string, empty if success

    bool bDebug = true;

private:
    std::ifstream * inStream = nullptr;
    bool bBinary   = false;
    bool bInputPos = false;
};

#endif // reader_h
