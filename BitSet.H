/***********************************************************************
 BitSet.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_BitSet_H
#define INCL_BOOM_BitSet_H
using namespace std;
#include <stdio.h>
#include <iostream>
#include "Vector.H"

namespace BOOM {
class BitSet {
public:
  BitSet(unsigned long Size=0);  
  BitSet(const BitSet &);
  virtual ~BitSet();
  void operator =(const BitSet &RValue);   
  void operator -=(BitSet &otherSet);  
  void operator +=(BitSet &otherSet);  
  void operator *=(BitSet &otherSet);  
  bool operator ==(BitSet &otherSet);
  bool operator !=(BitSet &otherSet);
  BitSet *operator -(BitSet &otherSet);  
  BitSet *operator +(BitSet &otherSet);  
  BitSet *operator *(BitSet &otherSet);  
  void intersect(BitSet &otherSet,BitSet &into);
  void unionWith(BitSet &otherSet,BitSet &into);
  bool isMember(unsigned long BitNumber) const;
  unsigned long getIthMember(unsigned long i);
  unsigned long cardinality() const;
  bool isEmpty();
  unsigned long getMaxSize() const;
  void addAll();  
  void addMember(unsigned long BitNumber);  
  void complement();
  void getRawBytes(unsigned char *&,unsigned long &maxSize,
		   unsigned long &numBytes);
  void load(FILE *fp);
  void purge();                
  void removeMember(unsigned long BitNumber);  
  void replaceRawBytes(unsigned char *,unsigned long maxSize,
		       unsigned long numBytes);
  void save(FILE *fp);  
  void setSize(unsigned long Size);
  void swapBits(unsigned long bitNumber1,unsigned long bitNumber2);
  void getMemberList(Vector<unsigned long> &); // ### could be made faster

  void printOn(ostream &) const;
protected:
  unsigned long NumBytes;  
  unsigned long MaxSize;   
  unsigned char *TheBitMap;
};


ostream &operator<<(ostream &,const BOOM::BitSet &);


}
#endif
