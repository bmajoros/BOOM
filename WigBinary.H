/****************************************************************
 WigBinary.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_WigBinary_H
#define INCL_WigBinary_H
#include <iostream>
#include "File.H"
#include "String.H"
#include "GffFeature.H"
#include "Array1D.H"
using namespace std;
using namespace BOOM;

struct WigInterval {
  int begin, end;
  WigInterval(int b,int e) : begin(b), end(e) {}
  bool overlaps(GffFeature *g) 
    {return begin<g->getEnd() && g->getBegin()<end;}
  bool isEmpty() {return begin>end;}
};

class WigBinary {
public:
  WigBinary(const String &filename,bool allowWrite=false);
  virtual float read(int pos); // pos is in residue units (not bytes!)
  void write(int pos,float);
  void regionsAbove(float threshold,Vector<WigInterval> &into);
  void regionsAbove(float threshold,Vector<WigInterval> &into,
		    int from,int to);
  void regionsAbove(float threshold,Vector<WigInterval> &into,
		    const Vector<WigInterval> &lowerThreshRegions);
  int getLength(); // in residues, not bytes
  virtual void getExtrema(float &min,float &max);
  void close();
  virtual float ave();
  virtual float ave(int begin,int end);
  virtual void buildPSA(Array1D<float> &);
protected:
  File F;
};

class ScaledWigBinary : public WigBinary {
public:
  ScaledWigBinary(const String &filename,float scale=1.0);
  void changeScale(float);
  virtual void getExtrema(float &min,float &max);
  virtual float read(int pos);
  virtual void buildPSA(Array1D<float> &);
  virtual float ave();
  virtual void useLog();
protected:
  float scale;
  bool shouldLog;
};

#endif

