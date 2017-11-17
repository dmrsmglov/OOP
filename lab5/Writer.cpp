#include "Writer.h"

void Writer::print(std::string &note) {
    out << note;
}

void Writer::close() {
    out.close();
}

void Writer::appendOpen() {
    out.open(file, std::ofstream::app);
}