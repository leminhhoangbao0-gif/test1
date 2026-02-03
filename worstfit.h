#ifndef WORSTFIT_H
#define WORSTFIT_H
#pragma once
#include "Algorithmn.h"
class worstfit:public Algorithmn
{
public:
    int findblock(const vector <block>&memory,int size);
};

#endif // WORSTFIT_H
