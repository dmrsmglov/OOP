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
            std::lock_guard<std::mutex> lg(pauseExecutionMutex);
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
    while (!reader.isEnd()) {
        {
            uint64_t number = reader.read();
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
}

void Task::consumer() {
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
            if (exit) {
                taskDone = true;
            }
            if (pause) {
                pauseExecution();
            }
            ul.lock();
        }
        notifiedConsumer = false;
    }
    if (!exit) {
        std::lock_guard<std::mutex> lg(completeMutex);
        ++completedThreads;
        if (completedThreads == numberOfThreads) {
            taskDone = true;
            naturalExit();
        }
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
    ++pausedThreads;
    if (pausedThreads == numberOfThreads) {
        writer.close();
    }
    while (pause) {
        condVarPause.wait(ul);
    }
    if (pausedThreads == numberOfThreads) {
        writer.appendOpen();
    }
    --pausedThreads;
}

void Task::printResult(std::string result) {
    std::lock_guard<std::mutex> lg(outStreamMutex);
    writer.print(result);
}

void Task::doTask(uint64_t number) {
    Factorization factorization(number);
    if (factorization.checkFactorization()) {
        printResult(factorization.getFactorization());
    } else {
        std::cerr << "Wrong factorization for " + std::to_string(factorization.getNumber()) + "\n";
    }
}