#include "user_locks.hpp"
#include <iostream>

user_lock_dekker::user_lock_dekker()
    : user_lock() {
    m_flag[0] = m_flag[1] = false;
    m_turn = 0;
}

void user_lock_dekker::lock(int thread_id) {
    // TODO: Implement the lock acquire part of the Dekker algorithm here
    //(void)thread_id; // TODO: Delete this line
    m_flag[thread_id].store(true);
    while(m_flag[(thread_id+1)%2].load()==true){
        if(m_turn.load()!=thread_id){
            m_flag[thread_id].store(false);
        }
        while (m_turn.load()!=thread_id)
        {
            // do nothing
        }
        m_flag[thread_id].store(true);
    }
        
    // m_turn.store(true);
    // if (m_flag[thread_id].load() ==true);
    //std::cout<<"in critical section \n";
}

void user_lock_dekker::unlock(int thread_id) {
    // TODO: Implement the lock release part of the Dekker algorithm here
    //(void)thread_id; // TODO: Delete this line
    // while (!m_flag[thread_id]);
    // std::cout<<m_turn<<"\n";
    m_turn.store((thread_id+1)%2);
    m_flag[thread_id].store(false);
    //fence
}
