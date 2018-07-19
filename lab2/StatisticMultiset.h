#pragma once

#include <set>
#include <vector>
#include <fstream>
#include <list>

template<typename T>
class StatisticMultiset {
public:
    StatisticMultiset() {
        configurationIsInitialised = false;
        lastGetAvg = 0;
    }

    ~StatisticMultiset() {
        statisticMultiset.clear();
    }

    void addNum(const T x) {
        statisticMultiset.insert(x);
        if (x > lastGetMax || !configurationIsInitialised) {
            lastGetMax = x;
        }
        if (x < lastGetMin || !configurationIsInitialised) {
            lastGetMin = x;
        }
        if (x > lastAboveParametr && configurationIsInitialised) {
            lastGetCountAbove++;
        }
        if (x < lastUnderParametr && configurationIsInitialised) {
            lastGetCountUnder++;
        }
        if (configurationIsInitialised) {
            lastGetAvg = (lastGetAvg + x) / 2;
        } else {
            lastGetAvg = x;
        }
        configurationIsInitialised = true;
    }

    void addNum(const std::multiset<T> &other) {
        for (auto el : other) {
            addNum(el);
        }
    }

    void addNum(const std::vector<T> &other) {
        for (auto el : other) {
            addNum(el);
        }
    }

    void addNum(const std::list<T> &other) {
        for (auto el : other) {
            addNum(el);
        }
    }

    void addNum(const StatisticMultiset &otherSet) {
        if (&otherSet != this) {
            for (auto el : otherSet.statisticMultiset) {
                addNum(el);
            }
        }
    }

    void addNumFromFile(const std::string &nameOfFile) {
        std::ifstream in(nameOfFile);
        T newElement;
        while (!in.eof) {
            in >> newElement;
            addNum(newElement);
        }
    }

    T getMax() const {
        return lastGetMax;
    }

    T getMin() const {
        return lastGetMin;
    }

    double getAvg() const {
        return lastGetAvg;
    }

    std::multiset<T> &getMultiset() {
        return statisticMultiset;
    }

    int getCountUnder(const T x) const {
        if (x != lastUnderParametr) {
            lastUnderParametr = x;
            int result = 0;
            for (auto el : statisticMultiset) {
                if (el < x) {
                    result++;
                }
            }
            lastGetCountUnder = result;
        }
        return lastGetCountUnder;
    }

    int getCountAbove(const T x) const {
        if (x != lastAboveParametr) {
            lastAboveParametr = x;
            int result = 0;
            for (auto el : statisticMultiset) {
                if (el > x) {
                    result++;
                }
            }
            lastGetCountUnder = result;
        }
        return lastGetCountUnder;
    }

private:
    std::multiset<T> statisticMultiset;
    bool configurationIsInitialised;
    mutable T lastAboveParametr, lastUnderParametr;
    mutable int lastGetCountAbove, lastGetCountUnder;
    mutable T lastGetMax, lastGetMin;
    mutable double lastGetAvg;
};