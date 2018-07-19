#pragma once
#include <string>
#include <map>
#include "MyException.h"

class IniParser {
public:
    IniParser();
    ~IniParser();

    void initialise(const std::string &fileName)
    throw (IOException);
    bool isHaveSection(const std::string &sectionName) const
    throw(ConfigurationNotInitialised);
    bool isHaveParam(const std::string &sectionName, const std::string &paramName) const
    throw(ConfigurationNotInitialised, SectionNotFound);
    template <typename T>
    T getValue(const std::string &sectionName, const std::string &paramName) const
    throw(ConfigurationNotInitialised, SectionNotFound, ParameterNotFound);

private:
    std::map<std::string, std::map<std::string, std::string>> myDatabase;
    void checkRequest(const std::string &sectionName, const std::string &paramName) const;
    void parseToParNameAndParValue(const std::string &currentString, std::string &paramName, std::string &paramValue);
    bool configurationIsInitialised;
};