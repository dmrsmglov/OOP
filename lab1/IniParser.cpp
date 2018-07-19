#include <fstream>
#include <iostream>
#include "IniParser.h"

using namespace std;

IniParser::IniParser() {
    configurationIsInitialised = false;
}

IniParser::~IniParser() {
    myDatabase.clear();
}

void IniParser::parseToParNameAndParValue(const std::string &currentString, std::string &paramName,
                                          std::string &paramValue) {
    int it = 0;
    while (it < currentString.length() && currentString[it] == ' ') {
        it++;
    }
    while (it < currentString.length() && currentString[it] != ' ') {
        paramName += currentString[it++];
    }
    while (it < currentString.length() && currentString[it] == ' ') {
        it++;
    }
    it++;
    while (it < currentString.length() && currentString[it] == ' ') {
        it++;
    }
    while (it < currentString.length() && currentString[it] != ' ') {
        paramValue += currentString[it++];
    }
}

void IniParser::initialise(const string &fileName) {
    ifstream in(fileName);
    string lastSection, currentString;
    if (in.good()) {
        configurationIsInitialised = true;
        while (!in.eof()) {
            getline(in, currentString);
            if (currentString.find(';') != string::npos) {
                currentString = currentString.substr(0, currentString.find(';'));
            }
            if (currentString.find('[') != string::npos) {
                int begin = static_cast<int>(currentString.find('['));
                int end = static_cast<int>(currentString.find(']') - 1);
                lastSection = currentString.substr(begin + 1, end - begin);
            } else if (currentString.find('=') != string::npos) {
                string paramName = "", paramValue = "";
                parseToParNameAndParValue(currentString, paramName, paramValue);
                myDatabase[lastSection][paramName] = paramValue;
            }
        }
    } else {
        throw IOException(fileName);
    }
}

bool IniParser::isHaveSection(const string &sectionName) const {
    if (configurationIsInitialised) {
        return static_cast<bool>(myDatabase.count(sectionName));
    } else {
        throw ConfigurationNotInitialised();
    }
}

bool IniParser::isHaveParam(const string &sectionName, const string &paramName) const {
    if (configurationIsInitialised) {
        if (myDatabase.count(sectionName)) {
            return static_cast<bool>(myDatabase.at(sectionName).count(paramName));
        } else {
            throw SectionNotFound(sectionName);
        }
    } else {
        throw ConfigurationNotInitialised();
    }
}

template<>
string IniParser::getValue<typename string>(const string &sectionName, const string &paramName) const {
    try {
        checkRequest(sectionName, paramName);
    }
    catch (IniException e) {
        throw e;
    }
    return myDatabase.at(sectionName).at(paramName);
}

template<>
int IniParser::getValue(const string &sectionName, const string &paramName) const {
    try {
        checkRequest(sectionName, paramName);
    }
    catch (IniException e) {
        throw e;
    }
    return stoi(myDatabase.at(sectionName).at(paramName));
}

template<>
double IniParser::getValue(const string &sectionName, const string &paramName) const {
    try {
        checkRequest(sectionName, paramName);
    }
    catch (IniException e) {
        throw e;
    }
    return stod(myDatabase.at(sectionName).at(paramName));
}


void IniParser::checkRequest(const std::string &sectionName, const std::string &paramName) const {
    if (!configurationIsInitialised) {
        throw ConfigurationNotInitialised();
    }
    if (!myDatabase.count(sectionName)) {
        throw SectionNotFound(sectionName);
    }
    if (!myDatabase.at(sectionName).count(paramName)) {
        throw ParameterNotFound(paramName);
    }
}