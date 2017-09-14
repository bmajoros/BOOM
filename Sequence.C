/***********************************************************************
 Sequence.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
using namespace std;
#include <math.h>
#include "Sequence.H"
#include "FastaReader.H"
using namespace BOOM;


BOOM::Regex Sequence::deflineRegex("^\\s*>\\s*(\\S+).*");


Sequence::Sequence(const BOOM::String &s,const Alphabet &alphabet)
  : phase(-1)
{
  append(alphabet,s.c_str());
}



Sequence::Sequence() : phase(-1)
{
}



Sequence::Sequence(const char *str,const Alphabet &alphabet)
  : phase(-1)
{
  append(alphabet,str);
}



Sequence::Sequence(Symbol s)
    : phase(-1)
{
    symbols.push_back(s);
}



Sequence::Sequence(Symbol s,int repetitions) {
    symbols.resize(repetitions);
    for(int i=0 ; i<repetitions ; ++i)
        symbols[i]=s;
}



void Sequence::copyFrom(const BOOM::String &source,const Alphabet &alphabet)
{
  phase=-1;
  symbols.clear();
  append(alphabet,source.c_str());
}



void Sequence::append(const Alphabet &alphabet,const char *ptr)
{
  while(*ptr)
    if(!isspace(*ptr))
      {
	const char c=*ptr++;
	Symbol s=alphabet.lookup(c);
	if(s==INVALID_SYMBOL) {
	  cerr<<"ERROR: Sequence::append() found invalid char ("
	      <<int(c);
	  if(isprint(c)) cerr<<"="<<c;
	  cerr<<")"<<endl;
	  throw "Exiting";
	}	
	append(s);
      }
    else
      ptr++;
}



ostream &operator<<(ostream &os,const Sequence &s)
{
  Sequence &ncs=const_cast<Sequence&>(s);
  ncs.printOn(os);
  return os;
}



void Sequence::printOn(ostream &os) const
{
  int numSymbols=symbols.size();
  for(int i=0 ; i<numSymbols ; ++i) os<<symbols[i]<<' ';
}



int Sequence::getLength() const
{
  return symbols.size();
}



void Sequence::printOn(ostream &os,const Alphabet &alphabet) const
{
  BOOM::String *s=toString(alphabet);
  os << *s;
  delete s;
}



void Sequence::toString(const Alphabet &alphabet,int startingAt,int length,
			BOOM::String &s) const
{
  s.clear();
  int end=startingAt+length;
  for(int i=startingAt ; i<end ; ++i) 
    s+=alphabet.lookup(symbols[i]);
}



BOOM::String *Sequence::toString(const Alphabet &alphabet,int startingAt) const
{
  BOOM::String &str=*new BOOM::String;
  int numSymbols=symbols.size();
  for(int i=startingAt ; i<numSymbols ; ++i) 
    str+=alphabet.lookup(symbols[i]);
  return &str;
}



void Sequence::clear()
{
  symbols.clear();
}



void Sequence::resize(int length)
{
  symbols.resize(length);
}



void Sequence::getSubsequence(int begin,int len,Sequence &seq) const
{
  const Sequence &self=*this;
  if(seq.getLength()!=len) seq.symbols.resize(len);
  if(len==0) return;
  int last=begin+len;
  int myLength=getLength();
  if(begin>=myLength || last>myLength) 
    throw String("Sequence::getSubsequence(")+begin+","+len+") : "+myLength;
  int j=0;
  for(int i=begin ; i<last ; ++i)
    seq[j++]=self[i];
}



bool Sequence::subsequenceOccursAt(const Sequence &subsequence,int at) const
{
  const Sequence &self=*this;
  int end=at+subsequence.getLength();
  if(end>=getLength()) return false;
  for(int i=at, j=0 ; i<end ; ++i, ++j)
    if(self[i]!=subsequence[j])
      return false;
  return true;
}



void Sequence::setPhase(int phase)
{
  this->phase=phase;
}



Symbol &Sequence::operator[](int i)
{
  return symbols[i];
}



void Sequence::prepend(Symbol s)
{
  symbols.push_front(s);
}



void Sequence::append(Symbol s)
{
  symbols.push_back(s);
}



void Sequence::append(const Sequence &other)
{
  symbols.append(other.symbols);
}



int Sequence::getPhase() const
{
  return phase;
}



Symbol Sequence::operator [](int i) const
{
  return symbols[i];
}



Sequence *Sequence::load(const BOOM::String &filename,const Alphabet &alphabet,
			 BOOM::String &substrateId)
{
  BOOM::FastaReader reader(filename,alphabet);
  BOOM::String sequence, defline;
  reader.nextSequence(defline,sequence);
  if(!deflineRegex.match(defline))
    throw BOOM::String("Can't parse defline: \"")+defline+"\"";
  substrateId=deflineRegex[1];
  return new Sequence(sequence,alphabet);
}



Sequence *Sequence::load(const BOOM::String &filename,const Alphabet &alphabet)
{
  BOOM::String substrateId;
  return load(filename,alphabet,substrateId);
}



const Sequence &Sequence::operator=(const Sequence &s)
{
  clear();
  append(s);
  return s;
}




void Sequence::reverseComplement(const Alphabet &alphabet,Sequence &rc) const
{
  rc.phase=(phase+getLength()-1) % 3;

  BOOM::Vector<Symbol>::const_reverse_iterator cur=symbols.rbegin(), 
    end=symbols.rend();
  for(; cur!=end ; ++cur)
    rc.append(alphabet.complement(*cur));
}



Sequence *Sequence::reverseComplement(const Alphabet &alphabet) const
{
  Sequence *rc=new Sequence;
  reverseComplement(alphabet,*rc);
  return rc;
}



bool Sequence::containsSymbol(Symbol s) 
{
  BOOM::Vector<Symbol>::iterator cur=symbols.begin(), end=symbols.end();
  for(; cur!=end ; ++cur)
    if(*cur==s)
      return true;
  return false;
}



int Sequence::countOccurrences(Symbol s)
{
  int c=0, n=symbols.size();
  for(int i=0 ; i<n ; ++i)
    if(symbols[i]==s)
      ++c;
  return c;
}



void Sequence::replaceAll(Symbol replaceThis,Symbol withThis)
{
  int n=symbols.size();
  for(int i=0 ; i<n ; ++i)
    if(symbols[i]==replaceThis)
      symbols[i]=withThis;
}



bool Sequence::operator<(const Sequence &other) const
{
  int n=getLength(), otherN=other.getLength();
  const BOOM::Vector<Symbol> &otherSymbols=other.symbols;
  for(int i=0 ; i<n ; ++i)
    {
      if(i>otherN) return false;
      Symbol thisSymbol=(*this)[i];
      Symbol thatSymbol=other[i];
      if(thisSymbol<thatSymbol) return true;
      if(thisSymbol>thatSymbol) return false;
    }
  return n<otherN;
}



void Sequence::insert(Symbol s,int pos)
{
  symbols.insertByIndex(s,pos);
}



void Sequence::insert(const Sequence &seq,int pos)
{
  symbols.insertByIndex(seq.symbols,pos);
}



void Sequence::cut(int begin,int len)
{
  symbols.cut(begin,len);
}



bool Sequence::operator==(const Sequence &other) const
{
  const Sequence &self=*this;
  int n=getLength(), m=other.getLength();
  if(n!=m) return false;
  for(int i=0 ; i<n ; ++i)
    if(self[i]!=other[i]) return false;
  return true;
}



unsigned Sequence::asInt(const AlphabetMap &alphabetMap,int begin,int len)
  const
{
  const Sequence &self=*this;
  if(len<0) len=getLength();
  unsigned code=0;
  unsigned base=alphabetMap.getRangeSize();
  unsigned end=begin+len;
  unsigned factor=1;
  for(int i=begin ; i<end ; ++i) {
    Symbol s=alphabetMap(self[i]);
    if(s==INVALID_SYMBOL) {
      cerr<<"Invalid symbol: "<<s<<endl;
      throw BOOM::String("Invalid symbol encountered in Sequence::asInt()");
    }
    unsigned x=(int)s;
    code+=x*factor;
    factor*=base;
  }
  return code;
}



unsigned Sequence::asInt(const Alphabet &alphabet,int begin,int len)
  const
{
  const Sequence &self=*this;
  if(len<0) len=getLength();
  unsigned code=0;
  unsigned base=alphabet.size();
  unsigned end=begin+len;
  unsigned factor=1;
  for(int i=begin ; i<end ; ++i) {
    Symbol s=self[i];
    unsigned x=(int)s;
    code+=x*factor;
    factor*=base;
  }
  return code;
}



void Sequence::fromInt(unsigned x,int seqLength,const Alphabet &alphabet)
{
  symbols.resize(seqLength);
  unsigned base=alphabet.size();
  for(int i=0 ; i<seqLength ; ++i) {
    unsigned digit=x%base;
    Symbol s=(int)digit;
    symbols[i]=s;
    x/=base;
  }	
}



void Sequence::fromInt(unsigned x,int seqLength,const AlphabetMap &alphabetMap)
{
  symbols.resize(seqLength);
  unsigned base=alphabetMap.getRangeSize();
  for(int i=0 ; i<seqLength ; ++i) {
    unsigned digit=x%base;
    Symbol s=(int)digit;
    s=alphabetMap.unmap(s);
    symbols[i]=s;
    x/=base;
  }	
}



Sequence Sequence::operator+(const Sequence &other)
{
  Sequence S=*this;
  S.append(other);
  return S;
}



Sequence Sequence::operator+(Symbol s)
{
  Sequence S=*this;
  S.append(s);
  return S;
}



void Sequence::operator+=(const Sequence &other)
{
  append(other);
}



void Sequence::operator+=(Symbol s)
{
  append(s);
}





String Sequence::operator()(const Alphabet &alphabet) const
{
  String s;
  toString(alphabet,0,getLength(),s);
  return s;
}



void Sequence::translate(const AlphabetMap &M,Sequence &into)
{
  int n=getLength();
  into.resize(n);
  for(int i=0 ; i<n ; ++i) 
    into[i]=M(symbols[i]);
}


#include "DnaDashAlphabet.H";
#include "Array1D.H"
void Sequence::useOnlyTheseSymbols(Array1D<bool> &indexBySymbol)
{
  Symbol d=INVALID_SYMBOL;
  int m=indexBySymbol.size();
  //cout<<"a "<<m<<" "<<indexBySymbol<<endl;
  bool *p=&indexBySymbol[0];
  for(int j=0 ; j<m ; ++j, ++p) if(*p) {d=Symbol(j); break;}

  int n=symbols.size();
  for(int i=0 ; i<n ; ++i) {
    //cout<<"i="<<i<<" s="<<symbols[i]<<endl;
    Symbol &s=symbols[i];
    DnaDashAlphabet alphabet; // ###
    //if(s==(Symbol)alphabet.lookup('-')) cout<<"found a dash"<<endl;
    //if(s==INVALID_SYMBOL) cout<<"Sequence::useOnlyTheseSymbols() found an invalid symbol"<<endl;
    if(s==INVALID_SYMBOL || !indexBySymbol[s]) {
      //cout<<"replacing "<<alphabet.lookup(s)<<" with "<<alphabet.lookup(i>0 ? symbols[i-1] : d)<<endl;
      s=i>0 ? symbols[i-1] : d;
    }
  }
}



int Sequence::asInt(int base)
{
  const int L=getLength();
  int x=0;
  for(int i=L-1 ; i>=0 ; --i) {
    Symbol s=symbols[i];
    x+=int(s*pow(base,(L-1-i)));
  }
  return x;
}

