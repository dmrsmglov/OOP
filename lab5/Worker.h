#pragma once

#include "Observer.h"

class Worker : public Observer{
public:
    Worker(std::ifstream &in, std::ofstream &out):
        inStream(in), outStream(out){};
    void update() const override;
    void go() const;
private:
    std::ifstream &inStream;
    std::ofstream &outStream;
};

