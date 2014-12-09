/****************************************************************
 Mutex.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "Mutex.H"
using namespace std;
using namespace BOOM;

Mutex::Mutex()
{
    // ctor

    pthread_mutexattr_init(&attr);
    pthread_mutex_init(&mutex,&attr);
}



Mutex::~Mutex()
{
    // dtor

    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&attr);
}



void Mutex::wait()
{
    pthread_mutex_lock(&mutex);
}



void Mutex::signal()
{
    pthread_mutex_unlock(&mutex);
}


