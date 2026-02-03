#ifndef BESTFIT_H
#define BESTFIT_H
#pragma once
#include "Algorithmn.h"
class bestfit:public Algorithmn
{
public:
    int findblock(const vector <block> &memory,int size)override;
};

#endif // BESTFIT_H
