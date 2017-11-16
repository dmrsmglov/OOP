#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include <fstream>
#include <utility>

class Task{
public:
    Task() = delete;
    Task(Task &&) = delete;
    Task(const Task &) = delete;
    Task(std::ifstream &inStream, std::ofstream &outStream):
        inStream(inStream), outStream(outStream){};
    void start();
private:
    void consumer();
    void producer();
    void clearQueue();
    void naturalExit();
    void pauseExecution();
    void doTask(uint64_t);
    std::mutex lockQueueMutex, pauseExecutionMutex, outStreamMutex;
    std::condition_variable condVarQueue, condVarPause;
    std::queue<uint64_t> numbers;
    std::ofstream &outStream;
    std::ifstream &inStream;
    bool pause = false;
    bool exit = false;
    bool notifiedConsumer = false;
    bool endOfFile = false;
    bool taskDone = false;
};
