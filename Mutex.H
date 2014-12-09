/****************************************************************
 Mutex.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_Mutex_H
#define INCL_Mutex_H
#include <iostream>
#include <pthread.h>
using namespace std;

namespace BOOM {
class Mutex
{
    pthread_mutexattr_t attr;
    pthread_mutex_t mutex;
public:
    Mutex();
    virtual ~Mutex();
    void wait();
    void signal();
};
}

#endif

