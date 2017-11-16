#pragma once
#include "Observer.h"
#include <vector>

class Observable{
public:
    void add(const Observer *obs);
protected:
    void notify();
    std::vector<const Observer *> viewers;
};
