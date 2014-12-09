/****************************************************************
 Semaphore.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_Semaphore_H
#define INCL_Semaphore_H
#include <iostream>
#include "semaphore.h"
using namespace std;


namespace BOOM {

class Semaphore {
public:
  Semaphore(int);
  virtual ~Semaphore();
  void wait();
  void post();
  int peek();
  bool tryWait(); // if true, then you must call post()!  (otherwise, don't!)
protected:
  sem_t S;
};

}
#endif

