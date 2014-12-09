/****************************************************************
 Thread.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "Thread.H"
using namespace std;
using namespace BOOM;


void Thread::start()
{
    int success=pthread_create(&pthread,NULL,&ff,(void*)this);
}



void Thread::join()
{
    pthread_join(pthread,NULL);
}

