#include <queue>
#include "scheduler.h"
#include "mutex.h"

static Schedular schedular;

template <typename T>
class MessageQueue {
    private:
        std::queue<T> q;
        Mutex m;

        std::queue<TCB*> waiting_receivers;

    public:
        void send(const T& msg);
        T receive();
};

template <typename T>
void MessageQueue<T>::send(const T& msg) {
    schedular.disable_preemption();

    q.push(msg);

    if (!waiting_receivers.empty()) {
        TCB* t = waiting_receivers.front();
        waiting_receivers.pop();
        schedular.unblock(t);
    }

    schedular.enable_preemption();
}

template <typename T>
T MessageQueue<T>::receive() {

    while(m.locked){
        schedular.yield();
    }

    while (q.empty()) {
        m.lock();
        TCB* current = schedular.get_current_tcb();
        waiting_receivers.push(current);
        m.unlock();

        schedular.block_current();
    }

    T val = q.front();
    q.pop();


    return val;
}