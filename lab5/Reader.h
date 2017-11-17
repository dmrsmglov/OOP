#pragma once

#include <fstream>

class Reader{
public:
    Reader(std::string &fileName, std::ifstream &streamName):
            file(fileName), in(streamName) {};
    uint64_t read();
    bool isEnd();
private:
    std::string &file;
    std::ifstream &in;
};