/***********************************************************************
 Random.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include "Random.H"
#include "BOOM/GSL/Random.H"
#include "BOOM/Constants.H"
using namespace std;
using namespace BOOM;


float BOOM::Random0to1()
{
  /*
  const int randInt=RandomNumber(LARGEST_RANDOM_NUMBER);
  return randInt/(float)(LARGEST_RANDOM_NUMBER-1);
  */
  return GSL::Random::random0to1();
}



float BOOM::RandomFloat(float from,float to)
{
  //return RandomFloat(to-from)+from;
  return GSL::Random::randomFloat(from,to);
}



float BOOM::RandomFloat(float range)
{
  //return Random0to1()*range;
  return GSL::Random::randomFloat(0,range);
}



float BOOM::RandomGaussian(float min,float max,int n)
{
  float sum=0.0;
  int i;
  for(i=0 ; i<n ; ++i)
    sum+=RandomFloat(min,max);
  return sum/n;
}



int BOOM::RandomNumber(int smallest,int largest)
{
  return RandomNumber(largest-smallest+1)+smallest;
}



int BOOM::RandomNumber(int n)
{
  //return n>1 ? rand() % n : 0;
  return n>1 ? GSL::Random::randomInt(0,n-1) : 0;
}



unsigned BOOM::GetRandomSeed()
{
  /*
  randomize();  
  return unsigned(RandomNumber(INT_MAX));
  */
  randomize();
  unsigned x=GSL::Random::randomUnsigned(0,LARGEST_UNSIGNED_INT-1);
  return x;
}



void BOOM::SeedRandomizer(unsigned s)
{
  GSL::Random::seed(s);
  /*
  srand(s);
  */
}



void BOOM::randomize()
{
  unsigned seed=(unsigned) time(0);
  SeedRandomizer(seed);
}


