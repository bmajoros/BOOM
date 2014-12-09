/***********************************************************************
 Random.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Random_H
#define INCL_BOOM_Random_H
#include <limits.h>
using namespace std;
#define INCL_RANDOM_H

namespace BOOM {
const int LARGEST_RANDOM_NUMBER=SHRT_MAX;  
unsigned GetRandomSeed();
void SeedRandomizer(unsigned); // alternative to randomize()
void randomize(); // seeds randomizer with timer
int RandomNumber(int n);
int RandomNumber(int smallest,int largest);
float Random0to1();
float RandomFloat(float range);
float RandomFloat(float from,float to);
float RandomGaussian(float min,float max,int n);
}

#endif
