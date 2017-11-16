#include <thread>
#include <iostream>
#include "Factorization.h"
#include "Task.h"

void Task::start() {
    std::vector<std::thread> threadPool(numberOfThreads);
    for (int i = 0; i < numberOfThreads; ++i) {
        threadPool[i] = std::thread(&Task::consumer, this);
    }
    std::thread prod(&Task::producer, this);
    while (!taskDone) {
        std::string signal;
        std::cin >> signal;
        if (signal == "exit") {
            exit = true;
            clearQueue();
            break;
        }
        if (signal == "pause") {
            pause = true;
        }
        if (signal == "resume") {
            if (pause) {
                pause = false;
                condVarPause.notify_all();
            }
        }
    }
    for (int i = 0; i < numberOfThreads; ++i) {
        threadPool[i].join();
    }
    prod.join();
}

void Task::producer() {
    if (inStream.good()) {
        uint64_t number;
        while (inStream >> number) {
            {
                std::unique_lock<std::mutex> ul(lockQueueMutex);
                if (exit) {
                    break;
                }
                numbers.push(number);
            }
            notifiedConsumer = true;
            condVarQueue.notify_all();
        }
        endOfFile = true;
    } else {
        std::cerr << "InputFile error\n";
    }
}

void Task::consumer() {
    if (outStream.good()) {
        std::unique_lock<std::mutex> ul(lockQueueMutex);
        while (!endOfFile || !numbers.empty()) {
            while (!notifiedConsumer) {
                condVarQueue.wait(ul);
            }
            while (!numbers.empty()) {
                uint64_t number = numbers.front();
                numbers.pop();
                ul.unlock();
                doTask(number);
                ul.lock();
                if (exit) {
                    taskDone = true;
                }
                if (pause) {
                    pauseExecution();
                }
            }
            notifiedConsumer = false;
        }
        taskDone = true;
        if (!exit) {
            naturalExit();
        }
    } else {
        std::cerr << "OutputFile error\n";
    }
}

void Task::clearQueue() {
    std::unique_lock<std::mutex> ul(lockQueueMutex);
    std::queue<uint64_t> empty;
    std::swap(empty, numbers);
}

void Task::naturalExit() {
    std::lock_guard<std::mutex> lg(outStreamMutex);
    if (!exit) {
        notify();
        exit = true;
    }
}

void Task::pauseExecution() {
    std::unique_lock<std::mutex> ul(pauseExecutionMutex);
    while (pause) {
        condVarPause.wait(ul);
    }
}

void Task::printResult(std::string result){
    std::lock_guard<std::mutex> lg(outStreamMutex);
    outStream << result;
}

void Task::doTask(uint64_t number) {
    Factorization factorization(number);
    if (factorization.checkFactorization()) {
        printResult(factorization.getFactorization());
    } else {
        std::cerr << "Wrong factorization for " + std::to_string(factorization.getNumber()) + "\n";
    }
}