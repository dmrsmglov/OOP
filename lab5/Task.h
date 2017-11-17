#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include <utility>
#include <fstream>
#include "Observable.h"
#include "Writer.h"
#include "Reader.h"

class Task : public Observable{
public:
    Task(Reader &in, Writer &out, unsigned int threads):
        reader(in), writer(out), numberOfThreads(threads){};
    void start();
private:
    void consumer();
    void producer();
    void clearQueue();
    void naturalExit();
    void pauseExecution();
    void doTask(uint64_t);
    void printResult(std::string);

    Writer &writer;
    Reader &reader;
    std::mutex lockQueueMutex, pauseExecutionMutex, outStreamMutex, completeMutex;
    std::condition_variable condVarQueue, condVarPause;
    std::queue<uint64_t> numbers;
    unsigned int numberOfThreads;
    unsigned int completedThreads = 0;
    unsigned int pausedThreads = 0;
    bool pause = false;
    bool exit = false;
    bool notifiedConsumer = false;
    bool endOfFile = false;
    bool taskDone = false;
};
