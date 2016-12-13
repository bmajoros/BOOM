/***********************************************************************
 Map.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Map_H
#define INCL_BOOM_Map_H
#include <map>
#include <iostream>
#include "Set.H"
using namespace std;

namespace BOOM {
template<class T>
struct DefaultMapComparator
{
  bool operator()(const T &a,const T &b) const {return a<b;}
};



template<class KeyType,class ValueType,
	 class Comp=DefaultMapComparator<KeyType> >
class Map : public map<KeyType,ValueType,Comp>
{
public:
  bool isDefined(const KeyType &key);
  void undefine(const KeyType &key) {erase(key);}
  inline int numElements();
  inline bool isEmpty() {return numElements()==0;}
  void printOn(ostream &);
  void readFrom(istream &);
  Set<KeyType> *getKeys();
  void getKeys(Set<KeyType> &); // doesn't clear the set first!
  Set<ValueType> *getValues();
  void getValues(Set<ValueType> &); // doesn't clear the set first!
};



template<class KeyType,class ValueType,class Comp>
ostream &operator<<(ostream &os,Map<KeyType,ValueType,Comp> &m)
{
  m.printOn(os);
  return os;
}



template<class KeyType,class ValueType,class Comp>
istream &operator>>(istream &is,Map<KeyType,ValueType,Comp> &m)
{
  m.readFrom(is);
  return is;
}
}



template<class KeyType,class ValueType,class Comp>
bool BOOM::Map<KeyType,ValueType,Comp>::isDefined(const KeyType &key)
{
  return this->find(key)!=this->end();
}



template<class KeyType,class ValueType,class Comp>
inline int BOOM::Map<KeyType,ValueType,Comp>::numElements() 
{
  return this->size();
}



template<class KeyType,class ValueType,class Comp>
void BOOM::Map<KeyType,ValueType,Comp>::printOn(ostream &os)
{
  int n=this->size();
  os<<n<<endl;
  typename Map<KeyType,ValueType,Comp>::iterator cur=this->begin(), 
    end=this->end();
  for(; cur!=end ; ++cur)
    {
      pair<const KeyType,ValueType> &p=*cur;
      os<<p.first<<'\t'<<p.second<<endl;
    }
}



template<class KeyType,class ValueType,class Comp>
void BOOM::Map<KeyType,ValueType,Comp>::readFrom(istream &is)
{
  this->clear();
  int n;
  is>>n;
  KeyType key;
  ValueType value;
  for(int i=0 ; i<n ; ++i)
    {
      is>>key>>value;
      (*this)[key]=value;
    }
}



template<class KeyType,class ValueType,class Comp>
Set<KeyType> *BOOM::Map<KeyType,ValueType,Comp>::getKeys()
{
  Set<KeyType> *keys=new Set<KeyType>;
  getKeys(*keys);
  return keys;
}



template<class KeyType,class ValueType,class Comp>
Set<ValueType> *BOOM::Map<KeyType,ValueType,Comp>::getValues()
{
  Set<ValueType> *values=new Set<ValueType>;
  getValues(*values);
  return values;
}



template<class KeyType,class ValueType,class Comp>
void BOOM::Map<KeyType,ValueType,Comp>::getKeys(Set<KeyType> &keys)
{
  typename Map<KeyType,ValueType,Comp>::iterator cur=this->begin(), 
    end=this->end();
  for(; cur!=end ; ++cur)
    {
      pair<const KeyType,ValueType> &p=*cur;
      keys.insert(p.first);
    }
}



template<class KeyType,class ValueType,class Comp>
void BOOM::Map<KeyType,ValueType,Comp>::getValues(Set<ValueType> &values)
{
  typename Map<KeyType,ValueType,Comp>::iterator cur=this->begin(), 
    end=this->end();
  for(; cur!=end ; ++cur)
    {
      pair<const KeyType,ValueType> &p=*cur;
      values.insert(p.first);
    }
}


#endif
