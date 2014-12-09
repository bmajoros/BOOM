/***********************************************************************
 swap.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_swap_H
#define INCL_swap_H

namespace BOOM {
template<class T>
inline void swap(T &a,T &b)
{
  T c=a;
  a=b;
  b=c;
}
}

#endif
