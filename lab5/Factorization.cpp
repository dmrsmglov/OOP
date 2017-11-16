#include "Factorization.h"
#include <iostream>
#include <mutex>

Factorization::Factorization(uint64_t x) {
    number = x;
    for (uint64_t div = 2; div * div <= x; div++) {
        while (!(x % div)) {
            divisors.push_back(div);
            x /= div;
        }
    }
    if (x != 1) {
        divisors.push_back(x);
    }
}

uint64_t Factorization::getNumber() const {
    return number;
}

bool Factorization::checkFactorization() const {
    uint64_t result = 1;
    for (auto div : divisors) {
        result *= div;
    }
    return result == number;
}

std::string Factorization::getFactorization() const {
    std::string factorization = std::to_string(number) + " = ";
    for (int i = 0; i < divisors.size() - 1; ++i) {
        factorization += std::to_string(divisors[i]) + " * ";
    }
    factorization += std::to_string(divisors.back()) + "\n";
    return factorization;
}