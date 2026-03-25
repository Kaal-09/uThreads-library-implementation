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

void worker3(void* arg) {
    int id = *(int*)arg;

    for (int i = 0; i < 5; i++) {
        for (volatile int j = 0; j < 100000000; j++);
        uthread::check_preemption();
        std::cout << "Thread " << id << " iteration " << i << std::endl;
    }
}

void premeptive_test(){
    uthread::init();
    int id1 = 1, id2 = 2, id3 = 3;
    uthread::create(worker3, &id1);
    uthread::create(worker3, &id2);
    uthread::create(worker3, &id3);

    while(true){
        uthread::yield();
    }
}

int main() {
    
    // two_thread_working_alive();
    premeptive_test();

    return 0;
}
