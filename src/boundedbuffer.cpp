#include "mutex.h"
#include "conditionvariable.h"
#include "boundedbuffer.h"

template <typename T>
void BoundedBuffer<T>::produce(const T& item) {
    m.lock();

    while (buffer.size() == capacity) {
        not_full.wait(m);
    }

    buffer.push(item);

    not_empty.signal();

    m.unlock();
}

template <typename T>
T BoundedBuffer<T>::consume() {
    m.lock();

    while (buffer.empty()) {
        not_empty.wait(m);
    }

    T item = buffer.front();
    buffer.pop();
 
    not_full.signal();

    m.unlock();

    return item;
}