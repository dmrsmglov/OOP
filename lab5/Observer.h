#pragma once

#include <iostream>
#include <fstream>

class Observer{
public:
    virtual void update() const = 0;
};