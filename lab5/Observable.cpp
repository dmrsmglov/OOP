#include "Observable.h"

void Observable::notify() {
    for (const auto &viewer : viewers) {
        viewer -> update();
    }
}

void Observable::add(const Observer *obs){
    viewers.push_back(obs);
}