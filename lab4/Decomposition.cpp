#include "Decomposition.h"

Decomposition::Decomposition(uint64_t x) {
    number = x;
    for (uint64_t div = 2; div * div <= x; div++){
        while (!(x % div)){
            divisors.push_back(div);
            x /= div;
        }
    }
    if (x != 1){
        divisors.push_back(x);
    }
}

bool Decomposition::checkDecomposition() const{
    uint64_t result = 1;
    for (auto div : divisors){
        result *= div;
    }
    return result == number;
}

std::string Decomposition::getDecomposition() const{
    std::string decomposition = std::to_string(number) + " = ";
    for (int i = 0; i < divisors.size() - 1; ++i){
        decomposition += std::to_string(divisors[i]) + " * ";
    }
    decomposition += std::to_string(divisors.back()) + "\n";
    return decomposition;
}