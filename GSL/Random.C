/****************************************************************
 Random.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include <time.h>
#include "Random.H"
using namespace std;


namespace GSL {

gsl_rng *Random::rng;
const gsl_rng_type *Random::rngType=gsl_rng_default;
Random Random::initializer;

    
Random::Random()
{
    // ctor
    //gsl_rng_env_setup();
    rngType=gsl_rng_mt19937;
    rng=gsl_rng_alloc(rngType);
}



void Random::randomize() {
    unsigned seed=(unsigned) time(0);
    gsl_rng_set(rng,seed);
}


void Random::seed(unsigned int seed) {
    gsl_rng_set(rng,seed);
}

    
gsl_rng *Random::getGenerator() {
    return rng;
}
    


unsigned long int Random::randomUnsignedLong(unsigned long int min,
                                             unsigned long int max) {
    return min+gsl_rng_uniform_int(rng,max-min+1);
}


    
unsigned int Random::randomUnsigned(unsigned min,unsigned max) {
    return (unsigned) randomUnsignedLong(min,max);
}


    
int Random::randomInt(int min,int max) { // inclusive
    return min+gsl_rng_uniform_int(rng,max-min+1);
}


    
float Random::randomFloat(float min,float max) {
    return float(min+gsl_rng_uniform(rng)*(max-min));
}


    
double Random::randomDouble(double min,double max) {
    return min+gsl_rng_uniform(rng)*(max-min);
}


    
}
