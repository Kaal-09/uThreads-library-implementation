#include <iostream>
#include "uthread/uthread.h"

void worker1(void*){
    int counter = 0;

    while(counter < 5){
        std::cout << "Thread 1 counter: " << counter << std::endl;
        counter++;
        uthread::yield();
    }

    uthread::exit();
}

void worker2(void*){
    int counter = 100;

    while(counter < 105){
        std::cout << "Thread 2 counter: " << counter << std::endl;
        counter++;
        uthread::yield();
    }

    uthread::exit();
}

void two_thread_working_alive(){
    uthread::init();
    std::cout << "Creating threads...." << std:: endl;
    uthread::create(worker1, nullptr);
    uthread::create(worker2, nullptr);
    std::cout << "Threads created" << std::endl;

    while(true){
        uthread::yield();
    }
}

int main() {
    
    two_thread_working_alive();

    return 0;
}
