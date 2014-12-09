/***********************************************************************
 ElasVector.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_ElasVector_H
#define INCL_BOOM_ElasVector_H
#include "Vector.H"
using namespace std;
namespace BOOM {


/***********************************************************************
                          template ElasVector
 ***********************************************************************/
template<class T>
class ElasVector
{
public:
  ElasVector(const T &nullInitializer);
  T &operator[](int);
  int size();
protected:
  const T &nullInitializer;
  Vector<T> theVector;
};


/***********************************************************************
                          class ElasPtrVector
 ***********************************************************************/
template<class T>
class ElasPtrVector : public ElasVector<T> {
public:
  ElasPtrVector(const T &nullInitializer=NULL) {}
};

/***********************************************************************
                          class ElasPtrVector
 ***********************************************************************/
class ElasDblVect : public ElasVector<double>
{
public:
  ElasDblVect();
};

/***********************************************************************
                          class ElasFloatVector
 ***********************************************************************/
class ElasFloatVect : public ElasVector<float>
{
public:
  ElasFloatVect();
};

/***********************************************************************
                          class ElasIntVector
 ***********************************************************************/
class ElasIntVect : public ElasVector<int>
{
public:
  ElasIntVect();
};

/***********************************************************************
                          class ElasCharVector
 ***********************************************************************/
class ElasCharVect : public ElasVector<char>
{
public:
  ElasCharVect();
};

/***********************************************************************
                          class ElasLongVector
 ***********************************************************************/
class ElasLongVect : public ElasVector<long>
{
public:
  ElasLongVect();
};
}









/***********************************************************************
                                methods
 ***********************************************************************/
using namespace BOOM;


template<class T>
T &ElasVector<T>::operator[](int index)
{
  int sz=theVector.size();
  if(index>=sz)
    for(int i=sz ; i<=index ; ++i)
      theVector.push_back(nullInitializer);
  return theVector[index];
}



template<class T>
ElasVector<T>::ElasVector(const T &nullInitializer)
  : nullInitializer(nullInitializer)
{
}



template<class T>
int ElasVector<T>::size()
{
  return theVector.size();
}

#endif
