#include <iostream>
#include "Decomposition.h"
#include <fstream>

int main(int argc, char* argv[]) {
    if (argv[1] != nullptr && argv[2] != nullptr){
        std::string inputFile(argv[1]);
        std::string outputFile(argv[2]);

        std::ifstream inStream(inputFile);
        std::ofstream outStream(outputFile);
        if (inStream.good() && outStream.good()){
            uint64_t number;
            while (inStream >> number){
                Decomposition decomposition = Decomposition(number);
                if (decomposition.checkDecomposition()){
                    outStream << decomposition.getDecomposition();
                }
                else{
                    std::cerr << "Wrong decomposition for " + std::to_string(number) + "\n";
                }
            }
        }
        else{
            std::cerr << "Input/output error!\n";
        }
    }
    return 0;
}