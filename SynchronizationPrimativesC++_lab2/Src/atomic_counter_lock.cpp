#include "atomic_counters.hpp"
#include <iostream>

atomic_counter_lock::atomic_counter_lock()
    : atomic_counter()
    , m_value(0)
    , m_lock() {
}

int atomic_counter_lock::increment() {
    // TODO: Add locks here
    int prev_value = m_value;
    std::lock_guard<std::mutex> lock(m_lock);
    //m_lock.lock();
    m_value = m_value + 1; 
    return prev_value;
}

int atomic_counter_lock::decrement() {
    // TODO: Add locks here
    int prev_value = m_value;
    std::lock_guard<std::mutex> lock(m_lock);
    //m_lock.lock();
    m_value = m_value - 1;
    //m_lock.unlock();
    return prev_value;
}

void atomic_counter_lock::set(int value) {
    // TODO: Add locks here
    std::lock_guard<std::mutex> lock(m_lock);
    //m_lock.lock();
    m_value = value;
    //std::cout<<"set value is "<<value<<"\n";
    //m_lock.unlock();

}

int atomic_counter_lock::get() {
    // TODO: Add locks here
    //std::cout<<"get value is "<<m_value<<"\n";
    std::lock_guard<std::mutex> lock(m_lock);
    return m_value;
    
}
