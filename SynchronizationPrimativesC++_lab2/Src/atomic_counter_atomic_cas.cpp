#include "atomic_counters.hpp"

atomic_counter_atomic_cas::atomic_counter_atomic_cas()
    : atomic_counter()
    , m_value(0) {
}

int atomic_counter_atomic_cas::increment() {
    // TODO: Modify this code using atomic compare and exchange (CAS) operations
    int prev_value = m_value.load(std::memory_order_relaxed);
    while(!m_value.compare_exchange_weak(prev_value, prev_value + 1,std::memory_order_acquire,std::memory_order_relaxed));
    //m_value = m_value + 1;
    return prev_value;
}

int atomic_counter_atomic_cas::decrement() {
    // TODO: Modify this code using atomic compare and exchange (CAS) operations
    int prev_value = m_value.load(std::memory_order_relaxed);
    while(!m_value.compare_exchange_weak(prev_value, prev_value - 1,std::memory_order_acquire,std::memory_order_relaxed));
    // int prev_value = m_value;
    // m_value = m_value - 1;
    return prev_value;
}

void atomic_counter_atomic_cas::set(int value) {
    // TODO: Modify this code using atomic store operations
    m_value.store(value,std::memory_order_relaxed);  
    //m_value = value;
}
int atomic_counter_atomic_cas::get() {
    // TODO: Modify this code using atomic load operations
    m_value.load(std::memory_order_relaxed);
    return m_value;
}
