#include "memorymanager.h"

void memorymanager::setmemory(const vector<block>&mem){
    m_memory = mem;
    m_original = mem;
}
bool memorymanager::allocate(process&p, Algorithmn& algo){
    int idx = algo.findblock(m_memory,p.size);
    if (idx ==-1){
        return false;
    }
    p.base = m_memory[idx].base;

    if (m_memory[idx].base==0){
        m_memory[idx].free = false;
    }
    return true;
}
const vector<block>&memorymanager::memory() const{
    return m_memory;
}
void memorymanager::reset()
{
    m_memory = m_original;
}
