#include <cstdint>
#include "Reader.h"

uint64_t Reader::read() const{
    uint64_t x;
    in >> x;
    return x;
}

bool Reader::isEnd() const {
    return in.eof();
}