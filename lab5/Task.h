#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include "Observable.h"
#include <fstream>
#include <utility>

class Task : public Observable{
public:
    Task(std::ifstream &in, std::ofstream &out, unsigned int threads):
        inStream(in), outStream(out), numberOfThreads(threads){};
    void start();
private:
    void consumer();
    void producer();
    void clearQueue();
    void naturalExit();
    void pauseExecution();
    void doTask(uint64_t);
    void printResult(std::string);

    std::mutex lockQueueMutex, pauseExecutionMutex, outStreamMutex, completeMutex;
    std::condition_variable condVarQueue, condVarPause;
    std::queue<uint64_t> numbers;
    std::ofstream &outStream;
    std::ifstream &inStream;
    unsigned int numberOfThreads;
    unsigned int completedThreads = 0;
    bool pause = false;
    bool exit = false;
    bool notifiedConsumer = false;
    bool endOfFile = false;
    bool taskDone = false;
};
