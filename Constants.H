/***********************************************************************
 Constants.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_Constants_H
#define INCL_Constants_H
using namespace std;
#include <iostream>
#include <math.h>
#include <fenv.h>
#ifdef __APPLE__
#include <float.h>
#else
#include <values.h>
#endif
#include <limits.h>

//==================================================================
// see /usr/include/limits.h and /usr/include/values.h for others!!!
//==================================================================

namespace BOOM {
const double POSITIVE_INFINITY    =-log(0.0);
const double NEGATIVE_INFINITY    =log(0.0);
const int    LARGEST_INTEGER      =INT_MAX;
const int    SMALLEST_INTEGER     =INT_MIN;
const float  LARGEST_FLOAT        =FLT_MAX;
const float  SMALLEST_FLOAT       =FLT_MIN;
const double LARGEST_DOUBLE       =DBL_MAX;
const double SMALLEST_DOUBLE      =DBL_MIN;
const long   LARGEST_LONG_INT     =LONG_MAX;
const long   SMALLEST_LONG_INT    =LONG_MIN;
const unsigned long LARGEST_UNSIGNED_INT =UINT_MAX;
const unsigned long SMALLEST_UNSIGNED_INT =0;
const double PI                   =acos(-1.0);
const double LOG_0                =log(0.0);
const double LOG_1                =0.0;

#define Endl endl

inline bool isInfinity(double d)
{
  return d==NEGATIVE_INFINITY || d==POSITIVE_INFINITY;
}

inline bool isNaN(double d)
{
  return isnan(d);
}

inline bool isFinite(double d)
{
  return !(isinf(d) || isnan(d));
}

inline double safeAdd(double a,double b)
{
  if(isinf(a) || isinf(b)) return NEGATIVE_INFINITY;
  return a+b;
}

inline double safeAdd(double a,double b,double c)
{
  if(isinf(a) || isinf(b) || isinf(c)) return NEGATIVE_INFINITY;
  return a+b+c;
}

inline double safeAdd(double a,double b,double c,double d)
{
  if(isinf(a) || isinf(b) || isinf(c) || isinf(d)) return NEGATIVE_INFINITY;
  return a+b+c+d;
}

inline void catchFloatOverflow()
{
  feenableexcept(FE_OVERFLOW);
}

inline bool didOverflow()
{
  return fetestexcept(FE_OVERFLOW);
}

inline setPrecision(ostream &os,int d)
{
  os.setf(ios::fixed);
  os.precision(d);
}


}

#endif
