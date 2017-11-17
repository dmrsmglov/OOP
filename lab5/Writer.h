#pragma once

#include <fstream>

class Writer{
public:
    Writer(std::string &fileName, std::ofstream &streamName):
            out(streamName), file(fileName){};
    void print(std::string &note);
    void close();
    void appendOpen();
private:
    std::ofstream &out;
    std::string &file;
};

