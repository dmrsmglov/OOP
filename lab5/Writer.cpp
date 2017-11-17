#include "Writer.h"

void Writer::print(std::string &note) const{
    out << note;
}

void Writer::close() {
    out.close();
}

void Writer::appendOpen() {
    out.open(file, std::ofstream::app);
}