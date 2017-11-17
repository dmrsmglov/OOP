#include <iostream>
#include <fstream>
#include "Worker.h"


int main(int argc, char *argv[]) {
    if (argc == 3) {
        std::string inFile(argv[1]);
        std::string outFile(argv[2]);
        Worker worker(inFile, outFile);
        worker.go();
    }
    return 0;
}