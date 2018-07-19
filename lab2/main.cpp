#include <iostream>
#include "StatisticMultiset.h"

using namespace std;

int main() {
    StatisticMultiset<int> mySet;
    mySet.addNum(2e9);

    cout << mySet.getMin() << endl;

    StatisticMultiset<float> mySet2;

    mySet2.addNum(2.5f);
    cout << mySet2.getMax() << endl;

    StatisticMultiset<int> mySet3;
    mySet3.addNum(15);
    mySet3.addNum(mySet3);

    cout << mySet3.getAvg() << endl;

    return 0;
}