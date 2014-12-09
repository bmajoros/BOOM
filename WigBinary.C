/****************************************************************
 WigBinary.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include <math.h>
#include "WigBinary.H"
using namespace std;
using namespace BOOM;

WigBinary::WigBinary(const String &filename,bool allowWrite)
{
  if(!F.open(filename, allowWrite ? "r+" : "r"))
    throw String("Error: cannot open file: ")+filename;
}



float WigBinary::read(int pos)
{
  F.seek(pos*sizeof(float));
  return F.readFloat();
}



void WigBinary::buildPSA(Array1D<float> &A)
{
  int L=getLength();
  A.resize(L);
  F.seek(0);
  A[0]=F.readFloat();
  for(int i=1 ; i<L ; ++i) A[i]=A[i-1]+F.readFloat();
}



void WigBinary::write(int pos,float f)
{
  F.seek(pos*sizeof(float));
  F.write(f);
}



void WigBinary::close()
{
  F.close();
}



int WigBinary::getLength()
{
  return F.getSize()/sizeof(float);
}



void WigBinary::regionsAbove(float threshold,Vector<WigInterval> &into)
{
  regionsAbove(threshold,into,0,getLength()-1);
}



void WigBinary::regionsAbove(float threshold,Vector<WigInterval> &into,
			     const Vector<WigInterval> &lowerThreshRegions)
{
  Vector<WigInterval>::const_iterator cur=lowerThreshRegions.begin(),
    end=lowerThreshRegions.end();
  for(; cur!=end ; ++cur) {
    const WigInterval &I=*cur;
    regionsAbove(threshold,into,I.begin,I.end);
  }
}



void WigBinary::regionsAbove(float threshold,Vector<WigInterval> &into,
			     int from,int to)
{
  int begin=-1;
  F.seek(from*sizeof(float));
  for(int pos=from ; pos<=to ; ++pos) {
    float y=F.readFloat();
    //cout<<pos<<" "<<threshold<<" "<<y<<" "<<begin<<" "<<from<<" "<<to<<endl;
    if(begin<0) {
      if(y>=threshold) begin=pos;
    }
    else {
      if(y<threshold) {
	into.push_back(WigInterval(begin,pos));
	begin=-1;
      }
    }
  }
  if(begin>=0) into.push_back(WigInterval(begin,to));
}



void WigBinary::getExtrema(float &min,float &max)
{
  min=max=read(0);
  int L=getLength();
  F.seek(0);
  for(int i=0 ; i<L ; ++i){
    float y=F.readFloat();
    if(y<min) min=y;
    else if(y>max) max=y;
  }
}



float WigBinary::ave()
{
  float sum=0;
  const int L=getLength();
  int N=0;
  for(int i=0 ; i<L ; ++i) {
    float x=read(i);
    if(x>0) { sum+=x; ++N; }
  }
  return sum/N;
}



float WigBinary::ave(int begin,int end)
{
  float sum=0;
  const int L=end-begin;
  int N=0;
  F.seek(begin*sizeof(float));
  for(int i=0 ; i<L ; ++i) {
    float x=F.readFloat();//read(i);
    if(x>0) { sum+=x; ++N; }
  }
  return sum/N;
}



ScaledWigBinary::ScaledWigBinary(const String &filename,float scale)
  : WigBinary(filename,false), scale(scale), shouldLog(false)
{
}



float ScaledWigBinary::read(int pos)
{
  F.seek(pos*sizeof(float));
  float f=F.readFloat();
  if(shouldLog) f=log(f+1);
  return f*scale;
}



void ScaledWigBinary::buildPSA(Array1D<float> &A)
{
  int L=getLength();
  A.resize(L);
  F.seek(0);
  float f=F.readFloat();
  if(shouldLog) f=log(f+1);
  A[0]=f*scale;
  for(int i=1 ; i<L ; ++i) {
    float f=F.readFloat();
    if(shouldLog) f=log(f+1);
    A[i]=A[i-1]+f*scale;
  }
}



float ScaledWigBinary::ave()
{
  float sum=0;
  const int L=getLength();
  int N=0;
  for(int i=0 ; i<L ; ++i) {
    float x=read(i);
    if(x>0) { sum+=x; ++N; }
  }
  return sum/N*scale;
}



void ScaledWigBinary::changeScale(float s)
{
  scale=s;
}



void ScaledWigBinary::useLog()
{
  shouldLog=true;
}



void ScaledWigBinary::getExtrema(float &min,float &max)
{
  min=max=read(0);
  if(shouldLog) { min=max=log(min+1); }
  int L=getLength();
  F.seek(0);
  for(int i=0 ; i<L ; ++i){
    float y=F.readFloat();
    if(shouldLog) y=log(y+1);
    if(y<min) min=y;
    else if(y>max) max=y;
  }
}



