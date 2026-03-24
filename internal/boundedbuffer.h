#include <queue>
#include "mutex.h"
#include "conditionvariable.h"

template <typename T>
class BoundedBuffer {
    private:
        std::queue<T> buffer;
        size_t capacity;

        Mutex m;
        ConditionVariable not_empty;
        ConditionVariable not_full;

    public:
        BoundedBuffer(size_t cap) : capacity(cap) {}

        void produce(const T& item);
        T consume();
};