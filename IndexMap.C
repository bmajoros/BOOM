/****************************************************************
 IndexMap.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "IndexMap.H"
#include "File.H"
#include "Exceptions.H"
using namespace std;
using namespace BOOM;

const int IndexMap::UNDEFINED = -1;
const unsigned char IndexMap::MATCH = 0;
const unsigned char IndexMap::INSERT = 1;
const unsigned char IndexMap::DELETE = 2;
const unsigned char IndexMap::END = 3;
const unsigned char IndexMap::MAX_RUNLEN=253;


IndexMap::IndexMap(int length)
  : array(length)
{
  // ctor
}



void IndexMap::clear()
{
  array.setAllTo(UNDEFINED);
}



void IndexMap::resize(int s,bool copyData)
{
  int t=array.size();
  if(t==0) {
    array.resize(s);
    return;
  }
  if(!copyData) return;
  Array1D<int> temp;
  //cout<<"a"<<endl;
  temp=array;
  //cout<<"b "<<temp.size()<<endl;
  array.resize(s);
  //cout<<"c "<<s<<" "<<t<<endl;
  int m=min(t,s);
  //cout<<"d "<<m<<endl;
  int i;
  for(i=0 ; i<m ; ++i) {
    //cout<<"i="<<i<<endl;
    array[i]=temp[i];
  }
  for(; i<s ; ++i)
    array[i]=UNDEFINED;
  //cout<<"/resize"<<endl;
}



int IndexMap::operator [](int i) const
{
  return array[i];
}



int &IndexMap::operator [](int i)
{
  return array[i];
}



void IndexMap::map(int from,int to)
{
  array[from]=to;
}



void IndexMap::setAllTo(int x)
{
  array.setAllTo(x);
}



int IndexMap::size() const
{
  return array.size();
}



void IndexMap::printOn(ostream &os) const
{
  int L=array.size();
  for(int i=0 ; i<L ; ++i)
    os<<i<<"->"<<array[i]<<" ";
}



ostream &BOOM::operator<<(ostream &os,const IndexMap &I)
{
  I.printOn(os);
  return os;
}



void IndexMap::save(const String &filename)
{
  File f(filename,"w");
  save(f);
}



void IndexMap::save(File &f)
{
  int L=array.size();
  f.write(L);
  int pos=-1, parentPos=-1;
  while(pos<L-1) { // ###
    unsigned char op, len;
    getChunk(pos,parentPos,op,len);
    //cout<<"pos="<<pos<<" parentPos="<<parentPos<<" L="<<L<<" "<<unsigned(op)<<" "<<unsigned(len)<<endl;
    if(int(len)==0) INTERNAL_ERROR;
    f.write(op);
    f.write(len);
    switch(op) {
    case MATCH:
      pos+=len;
      parentPos+=len;
      break;
    case INSERT:
      pos+=len;
      break;
    case DELETE:
      parentPos+=len;
      break;
    }
  }
  f.write(END);
}



void IndexMap::getChunk(int pos,int parentPos,unsigned char &op,
			unsigned char &runlen)
{
  ++pos;
  int nextParentPos=array[pos];
  int L=array.size();

  // First, figure out the operator:
  if(nextParentPos==UNDEFINED) op=INSERT;
  else if(nextParentPos==parentPos+1) op=MATCH;
  else op=DELETE;

  // Now figure out the run length:
  switch(op) {
  case MATCH:
    runlen=1;
    while(pos<L-1 && runlen<MAX_RUNLEN) {
      ++pos;
      parentPos=nextParentPos;
      nextParentPos=array[pos];
      if(nextParentPos!=parentPos+1) break;
      ++runlen;
    }
    break;
  case INSERT:
    runlen=1;
    while(pos<L-1 && runlen<MAX_RUNLEN) {
      ++pos;
      if(array[pos]!=UNDEFINED) break;
      ++runlen;
    }
    break;
  case DELETE: {
    //cout<<"cur="<<parentPos<<" next="<<nextParentPos<<endl;
    long rl=nextParentPos-parentPos-1;
    if(rl<0) {cout<<array<<endl;INTERNAL_ERROR;}
    runlen=(rl>MAX_RUNLEN ? MAX_RUNLEN : static_cast<unsigned char>(rl));
    //cout<<"runlen="<<unsigned(runlen)<<" rl="<<rl<<endl;
    }
    break;
  }
}



bool IndexMap::load(const String &filename)
{
  File f(filename);
  return load(f);
}



bool IndexMap::load(File &f)
{  
  int L=f.readInt();
  if(f.eof()) return false;
  array.resize(L);
  //cout<<"XXX L="<<L<<" "<<array.size()<<endl;
  int pos=-1, parentPos=-1;
  while(1) {
    unsigned char op=f.readByte();
    if(op==END) break;
    if(f.eof()) return false; // ### WHY IS THIS NECESSARY? ###########
    unsigned char runlen=f.readByte();
    switch(op) {
    case MATCH:
      for(int i=0 ; i<runlen ; ++i) {
	++pos;
	++parentPos;
	array[pos]=parentPos;
      }
      break;
    case INSERT:
      for(int i=0 ; i<runlen ; ++i) {
	++pos;
	array[pos]=UNDEFINED;
      }
      break;
    case DELETE:
      parentPos+=runlen;
      break;
    }
  }
  return true;
}



void IndexMap::invert(int length,IndexMap &into)
{
  Array1D<int> &other=into.array;
  other.resize(length);
  other.setAllTo(UNDEFINED);
  int L=array.size();
  for(int x=0 ; x<L ; ++x) {
    int y=array[x];
    if(y!=UNDEFINED) other[y]=x;
  }
}



void IndexMap::compose(const IndexMap &other,IndexMap &into)
{
  int L=size();
  if(into.size()!=L) into.resize(L);
  for(int x=0 ; x<L ; ++x) {
    int y=array[x];
    if(y==UNDEFINED) into[x]=UNDEFINED;
    else into[x]=other[y];
  }
}



List<int> *IndexMap::asList()
{
  List<int> *l=new List<int>;
  int L=array.size();
  int *a=&array[0];
  for(int i=0 ; i<L ; ++i) l->push_back(*a++);
  return l;
}



void IndexMap::copyFromList(List<int> &l)
{
  int L=l.size();
  array.resize(L);
  int *a=&array[0];
  List<int>::iterator cur=l.begin(), end=l.end();
  for(; cur!=end ; ++cur) *a++ = *cur;
}



bool IndexMap::sanityCheck(int targetLen)
{
  int L=array.size(), prev=UNDEFINED;
  for(int i=0 ; i<L ; ++i) {
    int x=array[i];
    if(x>=targetLen) {
      cout<<"pos="<<i<<" f(pos)="<<x<<" len="<<targetLen<<" mylen="<<L<<endl;
      return false;
    }
    if(x!=UNDEFINED && prev!=UNDEFINED && x<=prev) {
      cout<<"prev="<<prev<<" this="<<x<<" pos="<<i<<" mylen="<<L<<endl;
      return false;
    }
    if(x!=UNDEFINED) prev=x;
  }
  return true;
}



int IndexMap::countNonzero()
{
  int n=array.size(), count=0;
  for(int i=0 ; i<n ; ++i)
    if(array[i]!=UNDEFINED)
      ++count;
  return count;
}


