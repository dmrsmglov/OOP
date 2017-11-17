#pragma once

#include <fstream>

class Reader{
public:
    Reader(std::string &fileName, std::ifstream &streamName):
            file(fileName), in(streamName) {};
    uint64_t read() const;
    bool isEnd() const;
private:
    std::string &file;
    std::ifstream &in;
};