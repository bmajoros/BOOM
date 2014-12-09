/****************************************************************
 RouletteWheel.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/

#include "RouletteWheel.H"
#include "Random.H"
#include "Constants.H"
#include <math.h>
#include <iostream>
using namespace std;
using namespace BOOM;



// **************************************************
//               RouletteWheel methods
// **************************************************

void RouletteWheel::addSector(double probability)
{
  int n=wheel.size();
  if(n==0)
    wheel.push_back(probability);
  else
    {
      double prev=wheel[n-1];
      double next=prev+probability;
      if(prev>=1) next=POSITIVE_INFINITY;
      wheel.push_back(next);
    }
}



/*
int RouletteWheel::lookup(double f)
{
  int from=0, to=wheel.size()-1;
  while(from<to)
    {
      const int middle=(int) ceil((from+to)/2.0);
      if(f<wheel[middle])
	to=middle-1;
      else
	from=middle;
    }
  while(from>0 && f<wheel[from]) --from;
  while(f>wheel[from]) ++from;
  return from;
}
*/


int RouletteWheel::lookup(double f)
{
  int n=wheel.size();
  int from=0, to=n-1;
  while(from<to)
    {
      const int middle=int((from+to)/2.0);
      if(f>wheel[middle])
        from=middle+1;
      else
        to=middle;
    }
  while(from>0 && f<=wheel[from-1]) --from;
  while(f>wheel[from]) ++from;
  /*
  while(from>0 && wheel[from]>0 && 
        (wheel[from]==wheel[from-1] || wheel[from]==POSITIVE_INFINITY)) 
    --from;
  */
  while(from<n-1 && wheel[from]==wheel[from-1] && wheel[from]<1) ++from;
  //cout<<f<<" "<<from<<endl;
  return from;
}



int RouletteWheel::spin()
{
  return lookup(Random0to1());
}



void RouletteWheel::doneAddingSectors()
{
    int n=wheel.size();
    for(int i=0 ; i<n ; ++i) if(wheel[i]==0) wheel[i]=NEGATIVE_INFINITY;
    wheel[n-1]=POSITIVE_INFINITY;//wheel.push_back(POSITIVE_INFINITY);
}



void RouletteWheel::printOn(ostream &os) const
{
  int n=wheel.size();
  os<<'[';
  for(int i=0 ; i<n ; ++i)
    {
      os<<wheel[i];
      if(i<n-1) os<<',';
    }
  os<<']';
}



ostream &BOOM::operator<<(ostream &os,const BOOM::RouletteWheel &w)
{
  w.printOn(os);
  return os;
}


