#include <fstream>
#include <iostream>
#include "IniParser.h"

using namespace std;

int main() {
    string nameOfFile;
    cin >> nameOfFile;
    IniParser iniParser;
    try {
        iniParser.initialise(nameOfFile);
    }
    catch (IniException e) {
        cout << e.what() << endl;
    }
    try {
        cout << iniParser.isHaveParam("COMMON", "StatisterTimeMs");
    }
    catch (IniException e) {
        cout << e.what() << endl;
    }
    return 0;
}