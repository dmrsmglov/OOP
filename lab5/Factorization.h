#pragma once

#include <vector>
#include <cstdint>
#include <string>

class Factorization {
public:
    explicit Factorization(uint64_t);

    std::string getFactorization() const;

    bool checkFactorization() const;
    std::uint64_t getNumber() const;

private:
    uint64_t number;
    std::vector<uint64_t> divisors;
};