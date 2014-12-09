/****************************************************************
 Thread.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_Thread_H
#define INCL_Thread_H
#include <iostream>
#include <pthread.h>
#include "String.H"

using namespace std;

namespace BOOM {


class Thread
{
    pthread_t pthread;
    static inline void *ff(void *t);
public:
    virtual void f()=0;   // the code to be executed by this thread
    void start();        // actually starts the thread executing f()
    void join();         // wait for this thread to terminate
};



void *Thread::ff(void*t)
{
  try
    {
      ((Thread*)t)->f();
    }
  catch(const char *p)
    {
      cout<<p<<endl;
      exit(-1);
    }
  catch(const BOOM::String &s)
    {
      cout<<s<<endl;
      exit(-1);
    }
  catch(const string &s)
    {
      cout<<s<<endl;
      exit(-1);
    }
  catch(...)
    {
      cout<<"unknown exception caught in Thread::ff()"<<endl;
      throw;
    }
    return (void*)0;
}

}

#endif

