#include "Worker.h"
#include "Task.h"
void Worker::update() const {
    std::cout << "Please, enter any symbol to finish the program\n";
}

void Worker::go() const{
    std::cout << "How many threads do you want use to calculate factorization?\nEnter a number from 1 to 8\n";
    unsigned int numberOfThreads;
    std::cin >> numberOfThreads;
    Task task(inStream, outStream, numberOfThreads);
    task.add(this);
    std::cout << "You can enter 'exit', 'pause' or 'resume'\n";
    task.start();
}