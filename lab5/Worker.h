#pragma once

#include "Observer.h"

class Worker : public Observer{
public:
    Worker(std::string &in, std::string &out):
        inFileName(in), outFileName(out){};
    void update() const override;
    void go() const;
private:
    std::string &inFileName;
    std::string &outFileName;
};

