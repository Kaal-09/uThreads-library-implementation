#include <iostream>
#include "uthread/uthread.h"

void worker(void*) {
    std::cout << "Hello from user thread\n";
    uthread::exit();
}

int main() {
    uthread::init();
    uthread::create(worker, nullptr);
    uthread::yield();
    return 0;
}
