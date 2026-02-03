#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
#pragma once
#include <vector>
#include "process.h"
#include "block.h"
#include "Algorithmn.h"

class memorymanager
{
public:
    void setmemory(const vector<block>& mem);
    bool allocate(process& p, Algorithmn& algo);
    const vector<block>& memory() const;
    void reset();
private:
    vector<block> m_memory;
    vector<block> m_original;
};

#endif // MEMORYMANAGER_H
