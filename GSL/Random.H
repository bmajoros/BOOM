/****************************************************************
 Random.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_GSLRandom_H
#define INCL_GSLRandom_H
#include <iostream>
#include <gsl/gsl_rng.h>
using namespace std;


namespace GSL {

    
class Random {
public:
  // Initialization
  static void randomize();
  static void seed(unsigned int seed);
  
  // Uniformly distributed random numbers:
  static unsigned long int randomUnsignedLong(unsigned long int min,
					      unsigned long int max);
  static unsigned int randomUnsigned(unsigned min,unsigned max);
  static int randomInt(int min,int max); // interval [min,max] inclusive!
  static float randomFloat(float min,float max); // interval [min,max)
  static double randomDouble(double min,double max);//interval [min,max)
  static inline double random0to1() {return randomDouble(0.0,1.0);}
  
  // SEMI-PRIVATE (for use by specific distribution classes):
  static gsl_rng *getGenerator();
private:
  Random();
  static Random initializer;
  static gsl_rng *rng;
  static const gsl_rng_type *rngType;
};

}
#endif

