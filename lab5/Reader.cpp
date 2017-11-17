#include <cstdint>
#include "Reader.h"

uint64_t Reader::read() {
    uint64_t x;
    in >> x;
    return x;
}

bool Reader::isEnd() {
    return in.eof();
}