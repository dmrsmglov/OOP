#include <iostream>
#include <fstream>
#include "Task.h"


int main(int argc, char *argv[]) {
    if (argc == 3) {
        std::ifstream inStream(argv[1]);
        std::ofstream outStream(argv[2]);
        Task task(inStream, outStream);
        task.start();
    }
    return 0;
}