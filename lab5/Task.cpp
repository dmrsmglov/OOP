#include <thread>
#include <iostream>
#include "Decomposition.h"
#include "Task.h"

void Task::start() {
    std::cout << "How many threads do you want use to calculate factorization?\nEnter a number from 1 to 8\n";
    unsigned long numberOfThreads;
    std::cin >> numberOfThreads;
    std::vector<std::thread> myLittleThreads(numberOfThreads);
    std::cout << "You can enter 'exit', 'pause' or 'resume'\n";
    for (int i = 0; i < numberOfThreads; ++i) {
        myLittleThreads[i] = std::thread(&Task::consumer, this);
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
        myLittleThreads[i].join();
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
        std::cout << "Please, enter any symbol to finish the program\n";
        exit = true;
    }
}

void Task::pauseExecution() {
    std::unique_lock<std::mutex> ul(pauseExecutionMutex);
    while (pause) {
        condVarPause.wait(ul);
    }
}

void Task::doTask(uint64_t number) {
    Decomposition decomposition(number);
    if (decomposition.checkDecomposition()) {
        std::lock_guard<std::mutex> lg(outStreamMutex);
        outStream << decomposition.getDecomposition();
        outStream.flush();
    } else {
        std::cerr << "Wrong decomposition for " + std::to_string(decomposition.getNumber()) + "\n";
    }
    if (exit) {
        taskDone = true;
    }
    if (pause) {
        pauseExecution();
    }
}