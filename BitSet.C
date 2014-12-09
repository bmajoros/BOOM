/***********************************************************************
 BitSet.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
using namespace std;
#include "Exceptions.H"
#include "BitSet.H"
#include "String.H"
using namespace BOOM;


BOOM::BitSet::BitSet(unsigned long Size) 
  : TheBitMap(NULL)
{
  setSize(Size);
}



BOOM::BitSet::BitSet(const BitSet &other)
  : TheBitMap(NULL)
{
  *this=other;
}



BOOM::BitSet::~BitSet() 
{ 
  delete [] TheBitMap; 
}



void BOOM::BitSet::operator=(const BOOM::BitSet &RValue)
{  
  MaxSize=RValue.MaxSize;
  NumBytes=RValue.NumBytes;
  
  delete [] TheBitMap;
  TheBitMap=new unsigned char[NumBytes];

  unsigned long f;
  for(f=0 ; f<NumBytes ; f++)
    TheBitMap[f]=RValue.TheBitMap[f];
}



void BOOM::BitSet::operator -=(BOOM::BitSet &OtherSet) 
{
  unsigned long f;
  for(f=NumBytes ; f>0 ; f--)
    TheBitMap[f-1] &= (~(OtherSet.TheBitMap[f-1]));
}



void BOOM::BitSet::operator +=(BOOM::BitSet &OtherSet) 
{
  unsigned long f;
  for(f=NumBytes ; f>0 ; f--)
    TheBitMap[f-1] |= OtherSet.TheBitMap[f-1];
}



void BOOM::BitSet::operator *=(BOOM::BitSet &OtherSet) 
{
  unsigned long f;
  for(f=NumBytes ; f>0 ; f--)
    TheBitMap[f-1] &= OtherSet.TheBitMap[f-1];
}



bool BOOM::BitSet::operator !=(BitSet &otherSet) {
    return !(*this==otherSet);
}



bool BOOM::BitSet::operator ==(BOOM::BitSet &OtherSet) 
{
  unsigned long f;
  for(f=NumBytes-1 ; f ; f--)
    if(TheBitMap[f]!=OtherSet.TheBitMap[f]) 
      return false;
  
  unsigned char BitMask=1;
  unsigned long RelevantBits=MaxSize % 8;
  for(f=1; f<=RelevantBits; f++) 
    {
      if((BitMask & TheBitMap[0])!=
	 (BitMask & OtherSet.TheBitMap[0])) 
	return false;
      BitMask <<=1;
    }
  
  return true;
}



BOOM::BitSet *BOOM::BitSet::operator -(BOOM::BitSet &OtherSet) 
{
  BOOM::BitSet *retval=new BOOM::BitSet(MaxSize);
  *retval=*this;
  *retval-=OtherSet;
  
  return retval;
}



BOOM::BitSet *BOOM::BitSet::operator +(BOOM::BitSet &OtherSet) 
{
  BOOM::BitSet *retval=new BOOM::BitSet(MaxSize);
  *retval+=OtherSet;
  *retval+=*this;
  
  return retval;
}



void BOOM::BitSet::intersect(BOOM::BitSet &otherSet,BOOM::BitSet &retval)
{
  unsigned char *r=retval.TheBitMap, *t=TheBitMap, *o=otherSet.TheBitMap;
  for(unsigned long f=0 ; f<NumBytes ; ++f)
    *r++ = *t++ & *o++;
}



void BOOM::BitSet::unionWith(BitSet &otherSet,BitSet &retval) {
  unsigned char *r=retval.TheBitMap, *t=TheBitMap, *o=otherSet.TheBitMap;
  for(unsigned long f=0 ; f<NumBytes ; ++f)
    *r++ = *t++ | *o++;
}



bool BOOM::BitSet::isEmpty() {
    return cardinality()==0;
}



BOOM::BitSet *BOOM::BitSet::operator *(BOOM::BitSet &otherSet) 
{
  BOOM::BitSet *retval=new BOOM::BitSet(MaxSize);
  intersect(otherSet,*retval);
  return retval;
}



bool BOOM::BitSet::isMember(unsigned long BitNumber) const
{
  if(BitNumber>=MaxSize) 
    throw ArrayIndexException(BitNumber,
       "in BOOM::BitSet::IsMember()");
  
  unsigned long ByteFromRight=BitNumber/8;
  unsigned long BitInByte=BitNumber%8;

  unsigned char ByteMask=1;
  ByteMask <<= BitInByte;
  
  return (TheBitMap[NumBytes-1-ByteFromRight] & ByteMask) ? 
    true : false;
}



unsigned long BOOM::BitSet::cardinality() const
{
  unsigned long TotalElements=0, f;
  for(f=NumBytes-1 ; f ; f--) 
    {
      unsigned char BitMask=1;
      unsigned long ElementsInByte=0;
      unsigned long g;
      for(g=1 ; g<=8 ; g++) 
	{
	  if(BitMask & TheBitMap[f]) ElementsInByte++;
	  BitMask <<=1;
	}
      TotalElements+=ElementsInByte;
    }
   
  unsigned long RelevantBits=MaxSize % 8;
  if(RelevantBits==0) RelevantBits=8;
  unsigned char BitMask=1;
  unsigned long ElementsInByte=0;
  for(f=1 ; f<=RelevantBits ; f++) 
    {
      if(BitMask & TheBitMap[0]) ElementsInByte++;
      BitMask <<=1;
    }
  TotalElements+=ElementsInByte;

  int debug=0;
  for(int i=0 ; i<MaxSize ; ++i) if(isMember(i)) ++debug;
  
  return TotalElements;
}



unsigned long BOOM::BitSet::getMaxSize() const
{ 
  return MaxSize; 
}



void BOOM::BitSet::addAll()
{
  unsigned long i;
  for(i=1 ; i<NumBytes ; i++)
    TheBitMap[i]=char(0xFF); 
  TheBitMap[0]='\0';
  unsigned long RelevantBits=MaxSize % 8;
  unsigned char BitMask=1;
  unsigned long ElementsInByte=0;
  for(int f=1 ; f<=RelevantBits ; f++) 
    {
      TheBitMap[0] |= BitMask;
      BitMask <<=1;
    }
}



void BOOM::BitSet::addMember(unsigned long BitNumber) 
{
  if(BitNumber>=MaxSize) 
    throw ArrayIndexException(BitNumber,
			      "in BOOM::BitSet::AddMember()");
  
  unsigned long ByteFromRight=BitNumber/8;
  unsigned long BitInByte=BitNumber%8;

  unsigned char ByteMask=1;
  ByteMask <<= BitInByte;
  
  TheBitMap[NumBytes-1-ByteFromRight] =
    TheBitMap[NumBytes-1-ByteFromRight] | ByteMask;
}



void BOOM::BitSet::complement()
{
  unsigned long i;
  for(i=0 ; i<NumBytes ; i++)
    TheBitMap[i]=~TheBitMap[i];  
}



void BOOM::BitSet::getRawBytes(unsigned char *&bytes,
			    unsigned long &maxSize,
			    unsigned long &numBytes)
{
  bytes=TheBitMap;
  maxSize=this->MaxSize;
  numBytes=this->NumBytes;
}



void BOOM::BitSet::load(FILE *fp)
{
   
  if(fread(&MaxSize,1,sizeof(MaxSize),fp) != sizeof(MaxSize)) 
    throw FileErrorException("<filename unknown>",
			     "Cannot load BOOM::BitSet");
  setSize(MaxSize);
  
   
  if(fread(TheBitMap,1,NumBytes,fp) != NumBytes) 
    throw FileErrorException("<filename unknown>",
			     "Cannot load BOOM::BitSet");
}



void BOOM::BitSet::purge() 
{
  unsigned long i;
  
  for(i=0 ; i<NumBytes ; i++)
    TheBitMap[i]='\0';
}



void BOOM::BitSet::removeMember(unsigned long BitNumber) 
{
  if(BitNumber>=MaxSize) 
    throw ArrayIndexException(BitNumber,
			      "in BOOM::BitSet::RemoveMember()");
  
  unsigned long ByteFromRight=BitNumber/8;
  unsigned long BitInByte=BitNumber%8;

  unsigned char ByteMask=1;
  ByteMask <<= BitInByte;
  
  TheBitMap[NumBytes-1-ByteFromRight] =
    TheBitMap[NumBytes-1-ByteFromRight] & (~ByteMask);
}



void BOOM::BitSet::replaceRawBytes(unsigned char *rawBytes,
				unsigned long maxSize,
				unsigned long numBytes)
{
  delete [] this->TheBitMap;

  this->TheBitMap=rawBytes;
  this->MaxSize=maxSize;
  this->NumBytes=numBytes;
}



void BOOM::BitSet::save(FILE *fp)
{
   
  if(fwrite(&MaxSize,1,sizeof(MaxSize),fp)!=sizeof(MaxSize)) 
    throw FileErrorException("<filename unknown>",
			     "Cannot save BOOM::BitSet");
  
  if(fwrite(TheBitMap,1,NumBytes,fp) != NumBytes) 
    throw FileErrorException("<filename unknown>",
			     "Cannot save BOOM::BitSet");
}



void BOOM::BitSet::setSize(unsigned long Size) 
{
  delete [] TheBitMap;
  
  MaxSize=Size;
  NumBytes=MaxSize/8;
  if(MaxSize%8) NumBytes++;
  
  TheBitMap=new unsigned char[NumBytes];
  
  purge();
}



unsigned long BOOM::BitSet::getIthMember(unsigned long index) {
    int n=cardinality(), seen=0;
    for(int i=0 ; i<MaxSize ; ++i) {
        if(isMember(i)) {
            if(seen==index) return i;
            ++seen;
        }
    }
    throw BOOM::String("BitSet::getIthMember(")+index+")";
}



void BOOM::BitSet::swapBits(unsigned long bitNumber1,unsigned long bitNumber2)
{
  bool b1=isMember(bitNumber1), b2=isMember(bitNumber2);
  if(b1)
    if(b2)
      return;
    else
      {removeMember(bitNumber1); addMember(bitNumber2);}
  else
    if(b2)
      {addMember(bitNumber1); removeMember(bitNumber2);}
    else
      return;
}



void BOOM::BitSet::getMemberList(Vector<unsigned long> &V)
{
  V.clear();
  for(unsigned long i=0 ; i<MaxSize ; ++i)
    if(isMember(i))
      V.push_back(i);
}



void BOOM::BitSet::printOn(ostream &os) const
{
  unsigned long n=getMaxSize(), c=cardinality(), found=0;
  os<<"[";
  for(unsigned long i=0 ; i<n ; ++i)
    if(isMember(i)) {
      ++found;
      os<<i;
      if(found<c) os<<",";
    }
  os<<"]";
}



ostream &BOOM::operator<<(ostream &os,const BOOM::BitSet &b)
{
  b.printOn(os);
  return os;
}


