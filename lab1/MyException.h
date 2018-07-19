#pragma once

class IniException : public std::runtime_error {
public:
    explicit IniException(const std::string &arg) : runtime_error(arg) {};
};

class IOException : public IniException {
public:
    explicit IOException(const std::string &fileName) : IniException("Can't open or create this file: " + fileName) {};
};

class ConfigurationNotInitialised : public IniException {
public:
    explicit ConfigurationNotInitialised() : IniException("Configuration wasn't initialised") {};
};

class SectionNotFound : public IniException {
public:
    explicit SectionNotFound(const std::string &sectionName) : IniException("This section wasn't found: " + sectionName) {};
};

class ParameterNotFound : public IniException {
public:
    explicit ParameterNotFound(const std::string &paramName) : IniException("This parameter wasn't found: " + paramName) {};
};