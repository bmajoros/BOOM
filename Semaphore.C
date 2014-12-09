/****************************************************************
 Semaphore.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "Semaphore.H"
using namespace std;
using namespace BOOM;


Semaphore::Semaphore(int x)
{
  sem_init(&S,0,x);
}



Semaphore::~Semaphore()
{
  sem_destroy(&S);
}



void Semaphore::wait()
{
  sem_wait(&S);
}



void Semaphore::post()
{
  sem_post(&S);
}



int Semaphore::peek()
{
  int x;
  sem_getvalue(&S,&x);
  return x;
}



bool Semaphore::tryWait()
{
  return !sem_trywait(&S);
}


