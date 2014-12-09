/***********************************************************************
 SubstitutionMatrix.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_SubstitutionMatrix_H
#define INCL_SubstitutionMatrix_H
#include "String.H"
#include "Alphabet.H"
#include "Array2D.H"
#include "Regex.H"
#include <iostream>
#include "File.H"
#include "Constants.H"
#include <math.h>
#include "Vector.H"

namespace BOOM {

template<class ENTRY_TYPE>
class SubstitutionMatrix
{
public:
    SubstitutionMatrix(const String &filename,Alphabet &);
    ENTRY_TYPE operator()(Symbol row,Symbol column) const;
    inline ENTRY_TYPE getMeanValue() const { return meanValue; }
    inline float percentageGreaterThanZero() const {return percentOverZero;}
    inline float smallestEntry() const {return smallestValue;}
    inline float largestEntry() const {return largestValue;}
private:
    static Regex commentRegex, headerRegex, tableRegex, errorRegex;
    Array2D<ENTRY_TYPE> matrix;
    ENTRY_TYPE meanValue, smallestValue, largestValue;
    float percentOverZero;
    
    void load(const String &);
    friend ostream &operator<<(ostream &,
                     const SubstitutionMatrix<ENTRY_TYPE> &);
    void printOn(ostream &) const;
protected:
    Alphabet &alphabet;
    SubstitutionMatrix(Alphabet &);
    void computeMeanValue(int ignoreIndex);
    void computePercentOverZero(int);
    void set(int i,int j,ENTRY_TYPE value) {matrix[i][j]=value;}
};
}
using namespace BOOM;

template<class T>
ostream &operator<<(ostream &,const SubstitutionMatrix<T> &);

template<class T>
Regex SubstitutionMatrix<T>::commentRegex("^\\s*#");

template<class T>
Regex SubstitutionMatrix<T>::headerRegex("^[A-Z \\t\\n\\r*]+$");

template<class T>
Regex SubstitutionMatrix<T>::tableRegex("^\\s*[A-Z*]\\s+[. \\t\\r\\n01-9e\\+-]+$");

template<class T>
Regex SubstitutionMatrix<T>::errorRegex("\\S");



template<class ENTRY_TYPE>
SubstitutionMatrix<ENTRY_TYPE>::SubstitutionMatrix(
        const String &filename,
	Alphabet &alphabet)
  : alphabet(alphabet), 
    matrix(alphabet.getNumElements(),alphabet.getNumElements())
{
  matrix.setAllTo(NEGATIVE_INFINITY);
  load(filename);
  
  int ignoreIndex=alphabet.isDefined('*') ? alphabet.lookup('*') : -1;
  computeMeanValue(ignoreIndex);
  computePercentOverZero(ignoreIndex);
}



template<class ENTRY_TYPE>
SubstitutionMatrix<ENTRY_TYPE>::SubstitutionMatrix(Alphabet &alphabet)
  : alphabet(alphabet), 
    matrix(alphabet.getNumElements(),alphabet.getNumElements())
{
  matrix.setAllTo(NEGATIVE_INFINITY);
}



template<class ENTRY_TYPE>
ENTRY_TYPE SubstitutionMatrix<ENTRY_TYPE>::operator()(Symbol a,Symbol b) 
  const
{
  return matrix[int(a)][int(b)];
}



template<class ENTRY_TYPE>
void SubstitutionMatrix<ENTRY_TYPE>::load(const String &filename)
{
  Vector<Symbol> headerFields;
  File file(filename);
  while(!file.eof())
    {
      String line=file.getline();
      if(file.eof()) break;
      if(commentRegex.search(line)) continue;
      
      if(headerRegex.search(line))
	{
	  Vector<String> &fields=*line.getFields();
	  int n=fields.size();
	  for(int i=0 ; i<n ; ++i)
	    {
	      char headerChar=fields[i][0];
	      Symbol s=INVALID_SYMBOL;
	      if(alphabet.isDefined(headerChar))
		 s=alphabet.lookup(headerChar);
	      headerFields.push_back(s);
	    }
	  delete &fields;
	}
      else if(tableRegex.search(line))
	{
	  Vector<String> &fields=*line.getFields();
	  char cRowSymbol=fields[0][0];
	  if(alphabet.isDefined(cRowSymbol))
	    {
	      Symbol rowSymbol=alphabet.lookup(cRowSymbol);
	      int n=fields.size();
	      if(n!=headerFields.size()+1)
		throw String("Wrong number of entries on row \"")+line+
		  "\" of matrix "+filename;
	      for(int i=1 ; i<n ; ++i)
		{
		  Symbol columnSymbol=headerFields[i-1];
		  if(columnSymbol!=INVALID_SYMBOL)
		    matrix[rowSymbol][columnSymbol]=fields[i].asDouble();
		}
	    }
	  delete &fields;
	}
      else if(errorRegex.search(line))
	throw String("Error in substitution matrix ")+
	  filename+":\n"+line;
    }
}



template<class ENTRY_TYPE>
void SubstitutionMatrix<ENTRY_TYPE>::printOn(ostream &os) const
{
  int N=matrix.getFirstDim();

  // Determine the spacing
  int maxLen=0;
  for(Symbol i=0 ; i<N ; ++i)
    for(Symbol j=0 ; j<N ; ++j)
      {
	String s=matrix[i][j];
	int len=s.length();
	if(len>maxLen) maxLen=len;
      }
  int colWidth=maxLen+1;

  // Print the header
  for(int i=0 ; i<colWidth ; ++i) os << ' ';
  for(Symbol i=0 ; i<N ; ++i)
    {
      os << alphabet.lookup(i);
      for(int i=1 ; i<colWidth ; ++i) os << ' ';
    }
  os << endl;

  // Print each row of the matrix
  for(Symbol i=0 ; i<N ; ++i)
    {
      os << alphabet.lookup(i) << ' ';
      for(Symbol j=0 ; j<N ; ++j)
	{
	  os << matrix[i][j];
	  String s=matrix[i][j];
	  int len=s.length();
	  int extra=colWidth-len;
	  for(int i=0 ; i<extra ; ++i) os << ' ';
	}
      os << endl;
    }
}




template<class ENTRY_TYPE>
ostream &operator<<(ostream &os,
		    const SubstitutionMatrix<ENTRY_TYPE> &matrix)
{
  matrix.printOn(os);
  return os;
}




template<class ENTRY_TYPE>
void SubstitutionMatrix<ENTRY_TYPE>::computeMeanValue(int ignoreIndex)
{
  smallestValue=POSITIVE_INFINITY;
  largestValue=NEGATIVE_INFINITY;
  ENTRY_TYPE sum=0;
  int maxX=matrix.getFirstDim(), maxY=matrix.getSecondDim();
  for(int x=0 ; x<maxX ; ++x)
    {
      if(x==ignoreIndex) continue;
      for(int y=0 ; y<maxY ; ++y)
	{
	  ENTRY_TYPE e=matrix[x][y];
	  if(isinf(e)) continue;
	  if(isinf(smallestValue)) smallestValue=largestValue=e;
	  else if(e<smallestValue) smallestValue=e;
	  else if(e>largestValue) largestValue=e;
	  if(y==ignoreIndex) continue;
	  sum+=e;
	}
    }
  for(int x=0 ; x<maxX ; ++x)
    for(int y=0 ; y<maxY ; ++y)
      {
	ENTRY_TYPE &e=matrix[x][y];
	if(isinf(e)) e=smallestValue;  // unspecified -> assume mismatch (N)
      }
  meanValue=sum/ENTRY_TYPE((maxX-1)*(maxY-1));
}



template<class ENTRY_TYPE>
void SubstitutionMatrix<ENTRY_TYPE>::computePercentOverZero(int 
							    ignoreIndex) 
{
  int count=0;
  int maxX=matrix.getFirstDim(), maxY=matrix.getSecondDim();
  for(int x=0 ; x<maxX ; ++x)
    {
      if(x==ignoreIndex) continue;
      for(int y=0 ; y<maxY ; ++y)
	{
	  if(y==ignoreIndex) continue;
	  if(matrix[x][y]>0) ++count;
	}
    }
  percentOverZero=count/float(maxX*maxY);
}


#endif
