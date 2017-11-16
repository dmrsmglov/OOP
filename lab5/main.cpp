#include <iostream>
#include <fstream>
#include "Worker.h"


int main(int argc, char *argv[]) {
    if (argc == 3) {
        std::ifstream inStream(argv[1]);
        std::ofstream outStream(argv[2]);
        Worker concObs(inStream, outStream);
        concObs.go();
    }
    return 0;
}