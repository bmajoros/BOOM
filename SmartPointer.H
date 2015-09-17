/***********************************************************************
 SmartPointer.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_SmartPointer_H
#define INCL_BOOM_SmartPointer_H
#include <iostream>
using namespace std;
namespace BOOM {

/****************************************************************
                   HOW TO USE SMART POINTERS

 Very simple: 

   (1) The class that will be pointed to must either be
       derived from class PointedTo (below) or have a 
       "referenceCount" integer attribute.  

   (2) Be sure to #define USE_SMART_POINTERS before including this
       header file.  (Then you can easily disable smart-pointing 
       simply by commenting out that #define)

   (3) Declare your pointers as:
            BOOM::SmartPointer<SomeClassName> ptr;
       rather than:
            SomeClassName *ptr;

*****************************************************************/




/****************************************************************
                         base class PointedTo
 */
struct PointedTo
{
#ifdef USE_SMART_POINTERS
  int referenceCount;
  PointedTo() : referenceCount(0) {}
#endif
};





/****************************************************************
                       class BOOM::SmartPointer
 */
template<class T>
class SmartPointer
{
  T *p;
public:
  //-----------------------------------------------------
  SmartPointer(T *p=NULL) 
    : p(p) 
  {
#ifdef USE_SMART_POINTERS
    if(p) ++p->referenceCount;
#endif
  }
  //-----------------------------------------------------
  SmartPointer(const BOOM::SmartPointer<T> &o) 
    : p(o.p) 
  {
#ifdef USE_SMART_POINTERS
    if(p) ++p->referenceCount;
#endif
  }
  //-----------------------------------------------------
#ifdef USE_SMART_POINTERS
  ~SmartPointer() 
  {
    if(p)
      if(--p->referenceCount==0) delete p;
  }
#endif
  //-----------------------------------------------------
  BOOM::SmartPointer<T> &operator=(const BOOM::SmartPointer<T> &o)
  {
#ifdef USE_SMART_POINTERS
    if(p==o.p) return *this;
    if(p) 
      if(--p->referenceCount==0) delete p;
#endif
    p=o.p;
#ifdef USE_SMART_POINTERS
    if(p) ++p->referenceCount; 
#endif
    return *this;
  }
  //-----------------------------------------------------
  BOOM::SmartPointer<T> &operator=(T *t)
  {
#ifdef USE_SMART_POINTERS
    if(p==t) return *this;
    if(p) 
      if(--p->referenceCount==0) delete p;
#endif
    p=t;
#ifdef USE_SMART_POINTERS
    if(p) ++p->referenceCount; 
#endif
    return *this;
  }
  //-----------------------------------------------------
  T *operator ->() {return p;}
  //-----------------------------------------------------
  const T *operator ->() const {return p;}
  //-----------------------------------------------------
//#ifndef USE_SMART_POINTERS
  T &operator*() {return *p;}
//#endif
  //-----------------------------------------------------
//#ifndef USE_SMART_POINTERS
  const T &operator*() const {return *p;}
//#endif
  //-----------------------------------------------------
#ifndef USE_SMART_POINTERS
  operator T*() const {return p;}
#endif
  //-----------------------------------------------------
#ifndef USE_SMART_POINTERS
  //operator const T*() const {return p;}
#endif
  //-----------------------------------------------------
  bool operator<(const BOOM::SmartPointer<T> &t) const {return p<t.p;}
  //-----------------------------------------------------
  bool operator==(const BOOM::SmartPointer<T> &t) const {return p==t.p;}
  //-----------------------------------------------------
  bool operator!=(const BOOM::SmartPointer<T> &t) const {return p!=t.p;}
  //-----------------------------------------------------
  bool operator>(const BOOM::SmartPointer<T> &t) const {return p>t.p;}
  //-----------------------------------------------------
  operator bool() {return p!=NULL;}
  //-----------------------------------------------------
  void printOn(ostream &os) const {os<<p;}
};
}

template<class T>
ostream &operator<<(ostream &os,const BOOM::SmartPointer<T> &p)
{
  p.printOn(os);
  return os;
}

#endif
