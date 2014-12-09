/****************************************************************
 HashTable.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_HashTable_H
#define INCL_HashTable_H
#include <iostream>
#include "List.H"
#include "Array1D.H"
#include "Exceptions.H"
using namespace std;
using namespace BOOM;


/*****************************************************************
                           class HashTable
 *****************************************************************/

// type T must have an unsigned hash() function and an operator==
template<class T>
class HashTable {
public:
  typedef List<T> Bucket;
  HashTable(int tableSize=0); // use a prime!  (see below)
  void insert(const T &);
  bool isMember(const T &) const;
  Bucket &operator[](unsigned);
  void resize(int s) {table.resize(s);}
  void clear();
protected:
  Array1D<Bucket> table;
};



/*****************************************************************
                           class HashMap
 *****************************************************************/
template<class S,class T>
class HashMap {
public:
  typedef pair<S,T> MapElem;
  typedef List<MapElem> Bucket;
  HashMap(int tableSize=0); // use a prime!  (see below)
  bool isDefined(const S &) const;
  T &operator[](const S &);
  const T &operator[](const S &) const;
  T *lookup(const S &);
  void resize(int s) {table.resize(s);}
  void clear();
  unsigned countCollisions() const;
protected:
  Array1D<Bucket> table;
};



/*****************************************************************
  Some primes to use for table sizes:

  tens:
    13 19 23 31 41 53 61 71 83 89

  hundreds:
    101 199 293 401 499 601 701 797 887 

  thousands:
    997 1097 1201 1301 1399 1499 1601 1699 1801 1901 1999 2099 2179 
    2297 2399 2477 2593 2699 2801 2897 3001 3089 3191 3301 3391 3499 
    3593 3701 3797 3889 4001 4099 4201 4297 4397 4493 4597 4691 4801 
    4889 4999 5101 5197 5297 5399 5501 5591 5701 5801 5897 5987 6101 
    6199 6301 6397 6491 6599 6701 6793 6899 7001 7079 7193 7297 7393 
    7499 7591 7699 7793 7901 7993 8101 8191 8297 8389 8501 8599 8699 
    8783 8893 9001 9091 9199 9293 9397 9497 9601 9697 9791 9901 

  ten-thousands:
    9973 10993 11987 12983 13999 14983 15991 16993 17989 18979 19997
    20983 21997 22993 23993 24989 25999 26993 27997 28979 29989 30983
    31991 32999 33997 34981 35999 36997 37997 38993 39989 40993 41999
    42989 43997 44987 45989 46997 47981 48991 49999 50993 51991 52999
    53993 54983 55997 56999 57991 58997 59999 60961 61991 62989 63997
    64997 65993 66977 67993 68993 69997 70999 71999 72997 73999 74959
    75997 76991 77999 78989 79999 80989 81973 82997 83987 84991 85999
    86993 87991 88997 89989 90997 91997 92993 93997 94999 95989 96997 
    97987 98999 

  hundred-thousands:
    99961 199999 299993 399989 499979 599999 700001 799999 900001 

  millions:
    999959 1999957 
 *****************************************************************/


/*****************************************************************
                         HashTable methods
 *****************************************************************/

template<class T>
HashTable<T>::HashTable(int tableSize)
  : table(tableSize)
{
  // ctor
}



template<class T>
void HashTable<T>::insert(const T &t)
{
  if(isMember(t)) return;
  const unsigned index=t.hash()%table.size();
  List<T> &bucket=table[index];
  bucket.insert(t);
}



template<class T>
bool HashTable<T>::isMember(const T &t) const
{
  const unsigned index=t.hash()%table.size();
  List<T> &bucket=table[index];
  typename List<T>::const_iterator cur=bucket.begin(), end=bucket.end();
  for(; cur!=end ; ++cur) if(*cur==t) return true;
  return false;
}



template<class T>
typename HashTable<T>::Bucket &HashTable<T>::operator[](unsigned h)
{
  return table[h%table.size()];
}



template<class T>
void HashTable<T>::clear()
{
  const unsigned n=table.size();
  for(int i=0 ; i<n ; ++i) table[i].clear();
}



/*****************************************************************
                          HashMap methods
 *****************************************************************/

template<class S,class T>
HashMap<S,T>::HashMap(int tableSize)
  : table(tableSize)
{
  // ctor
}



template<class S,class T>
bool HashMap<S,T>::isDefined(const S &s) const
{
  const Bucket &bucket=table[s.hash()%table.size()];
  typename Bucket::const_iterator cur=bucket.begin(), end=bucket.end();
  for(; cur!=end ; ++cur) if((*cur).first==s) return true;
  return false;
}



template<class S,class T>
T &HashMap<S,T>::operator[](const S &s)
{
  table.size();
   s.hash()%table.size();
   table[s.hash()%table.size()];
  Bucket &bucket=table[s.hash()%table.size()];
  typename Bucket::iterator cur=bucket.begin(), end=bucket.end();
  for(; cur!=end ; ++cur) if((*cur).first==s) return (*cur).second;
  pair<S,T> p;
  p.first=s;
  bucket.push_front(p);
  return bucket.front().second;
}



template<class S,class T>
const T &HashMap<S,T>::operator[](const S &s) const
{
  Bucket &bucket=table[s.hash()%table.size()];
  typename Bucket::const_iterator cur=bucket.begin(), end=bucket.end();
  for(; cur!=end ; ++cur) if((*cur).first==s) return (*cur).second;
  throw "Element not found in BOOM::HashMap";
}



template<class S,class T>
T *HashMap<S,T>::lookup(const S &s)
{
  Bucket &bucket=table[s.hash()%table.size()];
  typename Bucket::iterator cur=bucket.begin(), end=bucket.end();
  for(; cur!=end ; ++cur) if((*cur).first==s) return &(*cur).second;
  return NULL;
}



template<class S,class T>
void HashMap<S,T>::clear()
{
  const unsigned n=table.size();
  for(int i=0 ; i<n ; ++i) table[i].clear();
}



template<class S,class T>
unsigned HashMap<S,T>::countCollisions() const
{
  unsigned collisions=0;
  const unsigned n=table.size();
  for(int i=0 ; i<n ; ++i) collisions+=max(0U,(unsigned)table[i].size()-1);
  return collisions;
}



#endif

