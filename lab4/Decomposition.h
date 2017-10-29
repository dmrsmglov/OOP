#pragma once
#include <vector>
#include <cstdint>
#include <string>

class Decomposition{
public:
    explicit Decomposition(uint64_t);
    std::string getDecomposition();
    bool checkDecomposition();
private:
    uint64_t number;
    std::vector<uint64_t> divisors;
};
