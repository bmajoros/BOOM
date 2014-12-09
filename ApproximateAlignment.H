/***********************************************************************
 ApproximateAlignment.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_ApproximateAlignment_H
#define INCL_ApproximateAlignment_H
#include <iostream>
#include <fstream>
#include "SubstitutionMatrix.H"
#include "MatchType.H"
#include "Vector.H"
#include "Sequence.H"
#include "ProteinTrans.H"
#include "AminoAlphabet.H"
#include "DnaAlphabet.H"
#include "StringMap.H"
#include "MolecularSequenceType.H"
#include "Constants.H"

//#define SUM_BLOSUM_SCORES


/****************************************************************
                     class ApproxAlignCell
 
 A single cell lying on the optimal path in the DP alignment matrix.
 Stores length and match count information to allow rapid computation
 of alignment scores of sub-regions of the alignment (for approximate
 alignment purposes).
 */
namespace BOOM {
class ApproxAlignCell
{
public:
  ApproxAlignCell();
  ApproxAlignCell(int x,int y,MatchType,float matches,int length);
  inline int getX() const;
  inline int getY() const;
  inline MatchType getMatchType() const;
  inline float cumulativeMatches() const;
  inline int cumulativeLength() const;
  void setCumulatives(float matches,int length);
  inline int lengthDiff(const ApproxAlignCell &other) const;
private:
  int x, y;
  MatchType matchType;
  float matches; // sum of matches from beginning of alignment
  int length;  // alignment length up to this point
};



/****************************************************************
                     class ApproximateAlignment

 Base class for ApproxNucAlignment and ApproxAminoAlignment. Con-
 sists of a list of cells which are the cells in the DP alignment
 matrix on the optimal solution.  The score along any stretch of
 the alignment can be computed in constant time once the two cells
 delimiting the region are know, through simple subtraction.  Can
 also provide an approximate alignment score between two DP cells
 not falling on the optimal path represented here, by adding in
 the minimum number of indels needed to get onto the optimal path.
 */
template<class FLOAT_OR_DOUBLE>
class ApproximateAlignment
{
public:
  ApproximateAlignment(const SubstitutionMatrix<FLOAT_OR_DOUBLE> &,
		       const Sequence &seq1,const char *seqStr1,
		       const Sequence &seq2,const char *seqStr2,
		       MolecularSequenceType);
  int getAlignmentLength() const;
  FLOAT_OR_DOUBLE getMatches() const;
  float getPercentIdentity() 
    {return getMatches()/(float)getAlignmentLength();}
  int getBeginX() const;
  int getEndX() const;
  int getBeginY() const;
  int getEndY() const;
  const ApproxAlignCell &getCell(int index) const;
  double score(const ApproxAlignCell &from,const ApproxAlignCell &to,
	       int extraIndels) const;
  double score(int beginX,int beginY,int endX,int endY) const;
  double show(int beginX,int beginY,int endX,int endY,
	      const Sequence &xSeq,const char *xSeqPtr,
	      const Sequence &ySeq,const char *ySeqPtr);//debugging
  MolecularSequenceType getSequenceType() const {return seqType;}
protected:
  const SubstitutionMatrix<FLOAT_OR_DOUBLE> &M;
  Vector<ApproxAlignCell> cells;
  MolecularSequenceType seqType;

  virtual Symbol getResidue(const Sequence &,const char *sequenceString,
				   int index)=0;
  virtual int getIncrement() const=0;
  void load(const String &filename,
	    const Sequence &seq1,const char *seqStr1,
	    const Sequence &seq2,const char *seqStr2,
	    int basesPerSymbol);
  void load(istream &,
	    const Sequence &seq1,const char *seqStr1,
	    const Sequence &seq2,const char *seqStr2,
	    int basesPerSymbol);
  const ApproxAlignCell &findNearestViaX(int x,int y,int &distance) const;
  const ApproxAlignCell &findNearestViaY(int x,int y,int &distance) const;
  void boundsChecks(int beginX,int endX,int beginY,
		    int endY,int len1,int len2,int line);
  void boundsCheck(int x,int maxVal,int line);
};



/****************************************************************
                     class ApproxNucAlignment

 An approximate alignment in nucleotide space.  Each cell represents
 a pair of nucleotides aligned on the optimal DP alignment path.
 */
template<class FLOAT_OR_DOUBLE>
class ApproxNucAlignment : public ApproximateAlignment<FLOAT_OR_DOUBLE>
{
public:
  ApproxNucAlignment(const String &filename,
		     const SubstitutionMatrix<FLOAT_OR_DOUBLE> &,
		     const Sequence &seq1,const char *seqStr1,
		     const Sequence &seq2,const char *seqStr2);
  ApproxNucAlignment(istream &,
		     const SubstitutionMatrix<FLOAT_OR_DOUBLE> &,
		     const Sequence &seq1,const char *seqStr1,
		     const Sequence &seq2,const char *seqStr2);
protected:
  virtual Symbol getResidue(const Sequence &,const char *seqStr,
				   int index);
  virtual int getIncrement() const {return 3;}
};



/****************************************************************
                     class ApproxAminoAlignment

 An approximate alignment in amino acid space.  Each cell corres-
 ponds to a pair of amino acids aligned in the optimal DP alignment
 path.  Note that while each cell corresponds to a pair of amino
 acids, a cell's (x,y) coordinates are in nucleotide space -- ie.,
 coordinates of adjacent cells differ by 0 or 3.
 */
template<class FLOAT_OR_DOUBLE>
class ApproxAminoAlignment : public ApproximateAlignment<FLOAT_OR_DOUBLE>
{
public:
  ApproxAminoAlignment(const String &filename,
		       const SubstitutionMatrix<FLOAT_OR_DOUBLE> &,
		       const Sequence &seq1,const char *seqStr1,
		       const Sequence &seq2,const char *seqStr2,
		       StringMap<char> &stopCodons);
  ApproxAminoAlignment(istream &,
		       const SubstitutionMatrix<FLOAT_OR_DOUBLE> &,
		       const Sequence &seq1,const char *seqStr1,
		       const Sequence &seq2,const char *seqStr2,
		       StringMap<char> &stopCodons);
  static ApproxAminoAlignment *load(istream &,
	    const SubstitutionMatrix<FLOAT_OR_DOUBLE> &,
	    const Sequence &seq1,const char *seqStr1,
	    const Sequence &seq2,const char *seqStr2,
	    StringMap<char> &stopCodons);
protected:
  void extend(const Sequence &seq1,const char *seqStr1,
	      const Sequence &seq2,const char *seqStr2,
	      StringMap<char> &stopCodons);
  void extendLeft(const char *s1,const char *s2,
		  StringMap<char> &stopCodons);
  void extendRight(const char *s1,int len1,const char *s2,int len2,
		   StringMap<char> &stopCodons);
  void recomputeCumulatives(const Sequence &seq1,
			    const char *seqStr1,
			    const Sequence &seq2,
			    const char *seqStr2);
  virtual Symbol getResidue(const Sequence &,const char *sequenceString,
				   int index);
  virtual int getIncrement() const {return 3;}
};
}
using namespace BOOM;


/****************************************************************
                  inline ApproxAlignCell methods
 */
int ApproxAlignCell::getX() const
{
  return x;
}



int ApproxAlignCell::getY() const
{
  return y;
}



int ApproxAlignCell::lengthDiff(const ApproxAlignCell &other) const
{
  return abs(other.length-length);
}



MatchType ApproxAlignCell::getMatchType() const
{
  return matchType;
}



float ApproxAlignCell::cumulativeMatches() const
{
  return matches;
}



int ApproxAlignCell::cumulativeLength() const
{
  return length;
}



/****************************************************************
                  ApproximateAlignment methods
 */
template<class T>
ApproximateAlignment<T>::ApproximateAlignment(const SubstitutionMatrix<T> &M,
					      const Sequence &seq1,
					      const char *seqStr1,
					      const Sequence &seq2,
					      const char *seqStr2,
					      MolecularSequenceType t)
  : M(M), seqType(t)
{
}



template<class T>
int ApproximateAlignment<T>::getAlignmentLength() const
{
  return cells.size();
}



template<class T>
T ApproximateAlignment<T>::getMatches() const
{
  return cells[cells.size()-1].cumulativeMatches();
}



template<class T>
int ApproximateAlignment<T>::getBeginX() const
{
  return cells[0].getX();
}



template<class T>
int ApproximateAlignment<T>::getEndX() const
{
  return cells[cells.size()-1].getX();
}



template<class T>
int ApproximateAlignment<T>::getBeginY() const
{
  return cells[0].getY();
}



template<class T>
int ApproximateAlignment<T>::getEndY() const
{
  return cells[cells.size()-1].getY();
}



template<class T>
const ApproxAlignCell &ApproximateAlignment<T>::getCell(int index) const
{
  return cells[index];
}



template<class T>
double ApproximateAlignment<T>::score(const ApproxAlignCell &from,
				      const ApproxAlignCell &to,
				      int extraIndels) const
{
  throw "I don't think this function actually gets called anywhere";
}



template<class T>
double ApproximateAlignment<T>::show(int beginX,int beginY,
				     int endX,int endY,
				     const Sequence &xSeq,
				     const char *xSeqPtr,
				     const Sequence &ySeq,
				     const char *ySeqPtr)
{
  cout<<"ALIGN("<<beginX<<","<<beginY<<","<<endX<<","<<endY<<") using ("
      <<cells[0].getX()<<","<<cells[0].getY()<<")-("
      <<cells[cells.size()-1].getX()<<","<<cells[cells.size()-1].getY()
      <<")"<<endl;
  int indels, dA=0, dB=0, dC=0, dD=0;
                    //### can optimize these initializations away...

  // Find the two nearest cells (vertically,horizontally) for start
  const ApproxAlignCell &A=findNearestViaX(beginX,beginY,dA);
  const ApproxAlignCell &B=findNearestViaY(beginX,beginY,dB);

  // Find the two nearest cells for end
  const ApproxAlignCell &C=findNearestViaX(endX,endY,dC);
  const ApproxAlignCell &D=findNearestViaY(endX,endY,dD);

  // Choose cell A or B, whichever is closer to C 
  // (or D...doesn't much matter)
  int dAC=A.lengthDiff(C), dBC=B.lengthDiff(C);
  const ApproxAlignCell *beginCell, *endCell;
  if(dAC<dBC) {beginCell=&A; indels=dA;cout<<"\t+"<<dA<<" indels"<<endl;}
  else {beginCell=&B; indels=dB;cout<<"\t+"<<dB<<" indels"<<endl;}

  // Choose cell C or D, whichever is closer to A
  int dAD=A.lengthDiff(D);
  if(dAD<dAC) {endCell=&D; indels+=dD;cout<<"\t+"<<dD<<" indels"<<endl;}
  else {endCell=&C; indels+=dC;cout<<"\t+"<<dC<<" indels"<<endl;}

  cout<<"\tbeginCell=("<<beginCell->getX()<<","<<beginCell->getY()<<")"<<endl;
  cout<<"\tendCell=("<<endCell->getX()<<","<<endCell->getY()<<")"<<endl;
  cout<<"\tindels="<<indels<<endl;

  // ### ARE THESE SUBTRACTIONS RIGHT, OR IS THERE A ONE-OFF ERROR?
  double matches=
    endCell->cumulativeMatches()-beginCell->cumulativeMatches();
  double length=endCell->cumulativeLength()-beginCell->cumulativeLength();
  cout<<"\tmatches="<<matches<<", length="<<length<<endl;
  if(length<=0) 
    {
      // These two points are too far from the optimal alignment path
      // to use it for approximating their alignment score, so we'll
      // just report the score expected from a random pair of sequences
      int xLen=endX-beginX, yLen=endY-beginY;
      int minLen, maxLen;
      if(xLen<yLen) {minLen=xLen;maxLen=yLen;}
      else {minLen=yLen;maxLen=xLen;}
      matches=M.percentageGreaterThanZero()*minLen;
      length=maxLen;
    }
  else
    {
      length+=indels;

      int n=cells.size(), i, j;
      Alphabet &alphabet=AminoAlphabet::global;
      String topLine, bottomLine;
      for(i=0 ; i<n ; ++i) if(&cells[i]==beginCell) break;
      for(j=i ; j<n ; ++j) if(&cells[j]==endCell) break;
      for(int k=i ; k<=j ; ++k)
	{
	  ApproxAlignCell &cell=cells[k];
	  int x=cell.getX(), y=cell.getY();
	  cout<<"("<<x<<","<<y<<") ";
	  Symbol residueX=getResidue(xSeq,xSeqPtr,x);
	  Symbol residueY=getResidue(ySeq,ySeqPtr,y);
	  switch(cell.getMatchType())
	    {
	    case MATCH:
	      topLine+=alphabet.lookup(residueX);
	      bottomLine+=alphabet.lookup(residueY);
	      cout<<"MATCH cm="<<cell.cumulativeMatches()
		  <<" cl="<<cell.cumulativeLength()<<endl;
	      break;
	    case FIRST_UNMATCHED:
	      topLine+=alphabet.lookup(residueX);
	      bottomLine+='-';
	      cout<<"INS cm="<<cell.cumulativeMatches()
		  <<" cl="<<cell.cumulativeLength()<<endl;
	      break;
	    case SECOND_UNMATCHED:
	      topLine+='-';
	      bottomLine+=alphabet.lookup(residueY);
	      cout<<"DEL cm="<<cell.cumulativeMatches()
		  <<" cl="<<cell.cumulativeLength()<<endl;
	      break;
	    }
	}
      cout<<topLine<<"\n"<<bottomLine<<endl;
    }
  if(length==0) return 1;
  return 0;//### make this void...
}



template<class T>
double ApproximateAlignment<T>::score(int beginX,int beginY,
				      int endX,int endY) 
  const
{
  int indels, dA=0, dB=0, dC=0, dD=0, INC=getIncrement();
                    //### can optimize these initializations away...

  // Find the two nearest cells (vertically,horizontally) for start
  const ApproxAlignCell &A=findNearestViaX(beginX,beginY,dA);
  const ApproxAlignCell &B=findNearestViaY(beginX,beginY,dB);
  //###dA/=INC; dB/=INC;

  // Find the two nearest cells for end
  const ApproxAlignCell &C=findNearestViaX(endX,endY,dC);
  const ApproxAlignCell &D=findNearestViaY(endX,endY,dD);
  //### dC/=INC; dD/=INC;

  // Choose cell A or B, whichever is closer to C 
  // (or D...doesn't much matter)
  int dAC=A.lengthDiff(C), dBC=B.lengthDiff(C);
  const ApproxAlignCell *beginCell, *endCell;
  if(dAC<dBC) {beginCell=&A; indels=dA;}
  else {beginCell=&B; indels=dB;}

  // Choose cell C or D, whichever is closer to A
  int dAD=A.lengthDiff(D);
  if(dAD<dAC) {endCell=&D; indels+=dD;}
  else {endCell=&C; indels+=dC;}

  // ### ARE THESE SUBTRACTIONS RIGHT, OR IS THERE A ONE-OFF ERROR?
  double matches=
    endCell->cumulativeMatches()-beginCell->cumulativeMatches();
  double length=endCell->cumulativeLength()-beginCell->cumulativeLength();
  if(length<=0) 
    {
      // These two points are too far from the optimal alignment path
      // to use it for approximating their alignment score, so we'll
      // just report the score expected from a random pair of sequences
      int xLen=endX-beginX, yLen=endY-beginY;
      int minLen, maxLen;
      if(xLen<yLen) {minLen=xLen;maxLen=yLen;}
      else {minLen=yLen;maxLen=xLen;}
#ifdef SUM_BLOSUM_SCORES
      int numMatch, numMismatch;
      if(seqType==DNA) numMatch=int(0.25*minLen+0.5);
      else numMatch=int(0.05*minLen+0.5);
      numMismatch=minLen-numMatch;
      matches=numMatch*M.largestEntry()+numMismatch*M.smallestEntry();
      return matches;
#else
      matches=M.percentageGreaterThanZero()*minLen;
#endif
      length=maxLen;
    }
#ifdef SUM_BLOSUM_SCORES
  return matches;
#else
  else length+=indels;
  if(length==0) return 1;
  return matches/length;
#endif
}



template<class T>
const ApproxAlignCell &ApproximateAlignment<T>::findNearestViaX(int x,int y,
							 int &distance) 
  const
{
  // First, perform a binary search for any cell at this x coordinate
  const int n=cells.size();
  int begin=0, end=n;
  while(begin<end)
    {
      int mid=(begin+end)/2;
      const ApproxAlignCell &midCell=cells[mid];
      if(x>midCell.getX()) begin=mid+1;
      else end=mid;
    }

  // Back up to the leftmost cell having this x coordinate
  while(begin && cells[begin-1].getX()==x) --begin;
  if(begin>=n) --begin;

  // Now consider all the cells in this run having the same x 
  // coordinate, and choose the nearest
  int bestDistance=abs(cells[begin].getY()-y), bestIndex=begin;
  for(int i=begin+1 ; i<n ; ++i)
    {
      const ApproxAlignCell &thisCell=cells[i];
      if(thisCell.getX()!=x) break;
      int distance=abs(thisCell.getY()-y);
      if(distance<bestDistance) {bestDistance=distance; bestIndex=i;}
    }
  distance=bestDistance;
  return cells[bestIndex];
}



template<class T>
const ApproxAlignCell &ApproximateAlignment<T>::findNearestViaY(int x,int y,
							    int &distance) 
  const
{
  // First, perform a binary search for any cell at this y coordinate
  const int n=cells.size();
  int begin=0, end=n;
  while(begin<end)
    {
      int mid=(begin+end)/2;
      const ApproxAlignCell &midCell=cells[mid];
      if(y>midCell.getY()) begin=mid+1;
      else end=mid;
    }

  // Back up to the leftmost cell having this y coordinate
  while(begin && cells[begin-1].getY()==y) --begin;
  if(begin>=n) --begin;

  // Now consider all the cells in this run having the same 
  // x coordinate, and choose the nearest
  int bestDistance=abs(cells[begin].getX()-x), bestIndex=begin;
  for(int i=begin+1 ; i<n ; ++i)
    {
      const ApproxAlignCell &thisCell=cells[i];
      if(thisCell.getY()!=y) break;
      int distance=abs(thisCell.getX()-x);
      if(distance<bestDistance) {bestDistance=distance; bestIndex=i;}
    }
  distance=bestDistance;
  return cells[bestIndex];
}



template<class T>
void ApproximateAlignment<T>::load(const String &filename,
				   const Sequence &seq1,const char *seqStr1,
				   const Sequence &seq2,const char *seqStr2,
				   int basesPerSymbol)
{
  ifstream is(filename.c_str());
  if(!is.good()) 
    throw String("Error loading approximate alignment from file ")+
      filename;
  load(is,seq1,seqStr1,seq2,seqStr2,basesPerSymbol);
}



template<class T>
void ApproximateAlignment<T>::boundsChecks(int beginX,int endX,int beginY,
					   int endY,int len1,int len2,
					   int line)
{
  boundsCheck(beginX,len1,line);
  boundsCheck(endX,len1,line);
  boundsCheck(beginY,len2,line);
  boundsCheck(endY,len2,line);
}



template<class T>
void ApproximateAlignment<T>::boundsCheck(int x,int maxVal,int line)
{
  if(x<0 || x>=maxVal) throw String(x)+" out of bounds in ApproximateAlignment (line "+line+"), max is "+maxVal+"\nHINT: DID YOU GIVE THE COMMAND-LINE ARGUMENTS IN THE WRONG ORDER?\n";
}



template<class T>
void ApproximateAlignment<T>::load(istream &is,
				   const Sequence &seq1,const char *seqStr1,
				   const Sequence &seq2,const char *seqStr2,
				   int basesPerSymbol)
{
  const int INC=basesPerSymbol;
  int dummy, beginX, endX, beginY, endY;
  is >> beginY;
  if(is.eof()) return;
  is >> endY >> beginX >> endX >> dummy >> dummy >> dummy;
  int len1=seq1.getLength(), len2=seq2.getLength();
  boundsChecks(beginX-1,endX-1,beginY-1,endY-1,len2,len1,__LINE__);

  char op;
  int len, x=beginX-1-INC, y=beginY-1-INC, dX, dY, cumLen=0;
  T matches=0;
  MatchType matchType;
  while(!is.eof())
    {
      is >> op >> len; // NOTE: len is *always* in nucleotides!
      if(is.eof()) break;
      switch(op)
	{
	case 'i': 
	  matchType=SECOND_UNMATCHED;  
	  dX=0;
	  dY=INC; 
	  break;
	case 'd': 
	  matchType=FIRST_UNMATCHED; 
	  dX=INC;
	  dY=0; 
	  break;
	case 'm': 
	  matchType=MATCH;            
	  dX=INC; 
	  dY=INC; 
	  break;
	case 'e': return;
	}
      for(int i=0 ; i<len ; i+=INC)
	{
	  if(matchType==MATCH)
	    {
	      boundsCheck(y+INC,len1,__LINE__);
	      boundsCheck(x+INC,len2,__LINE__);
	      Symbol s1=getResidue(seq1,seqStr1,y+INC);
	      Symbol s2=getResidue(seq2,seqStr2,x+INC);
	      if(M(s1,s2)>0) ++matches;
	    }
	  cumLen++;
	  x+=dX;
	  y+=dY;
	  cells.push_back(ApproxAlignCell(y,x,matchType,matches,cumLen));
	}
    }
}




/****************************************************************
                    ApproxNucAlignment methods
 */
template<class T>
ApproxNucAlignment<T>::ApproxNucAlignment(const String &filename,
					  const SubstitutionMatrix<T> &M,
					  const Sequence &seq1,
					  const char *seqStr1,
					  const Sequence &seq2,
					  const char *seqStr2)
  : ApproximateAlignment<T>(M,seq1,seqStr1,seq2,seqStr2,DNA)
{
  load(filename,seq1,seqStr1,seq2,seqStr2,1);
}



template<class T>
ApproxNucAlignment<T>::ApproxNucAlignment(istream &is,
					  const SubstitutionMatrix<T> &M,
					  const Sequence &seq1,
					  const char *seqStr1,
					  const Sequence &seq2,
					  const char *seqStr2)
  : ApproximateAlignment<T>(M,seq1,seqStr1,seq2,seqStr2)
{
  load(is,seq1,seqStr1,seq2,seqStr2,1);
}



template<class T>
Symbol ApproxNucAlignment<T>::getResidue(const Sequence &seq,
					 const char *seqStr,
					 int index)
{
  return seq[index];
}



/****************************************************************
                  ApproxAminoAlignment methods
 */
template<class T>
ApproxAminoAlignment<T>::ApproxAminoAlignment(const String &filename,
					   const SubstitutionMatrix<T> &M,
					   const Sequence &seq1,
					   const char *seqStr1,
					   const Sequence &seq2,
					   const char *seqStr2,
					   StringMap<char> &stopCodons)
  : ApproximateAlignment<T>(M,seq1,seqStr1,seq2,seqStr2,PROTEIN)
{
  ApproximateAlignment<T>::load(filename,seq1,seqStr1,seq2,seqStr2,3);
  if(cells.size()) 
    extend(seq1,seqStr1,seq2,seqStr2,stopCodons);
}



template<class T>
ApproxAminoAlignment<T>::ApproxAminoAlignment(istream &is,
					   const SubstitutionMatrix<T> &M,
					   const Sequence &seq1,
					   const char *seqStr1,
					   const Sequence &seq2,
					   const char *seqStr2,
					   StringMap<char> &
					      stopCodons)
  : ApproximateAlignment<T>(M,seq1,seqStr1,seq2,seqStr2,PROTEIN)
{
  ApproximateAlignment<T>::load(is,seq1,seqStr1,seq2,seqStr2,3);
  if(cells.size())
    extend(seq1,seqStr1,seq2,seqStr2,stopCodons);
}



template<class T>
Symbol ApproxAminoAlignment<T>::getResidue(const Sequence &seq,
					   const char *seqStr,
					   int index)
{
  char buffer[4];
  buffer[3]='\0';
  memcpy(buffer,seqStr+index,3);
  char aminoAcid=ProteinTrans::mapCodon(buffer);
  return AminoAlphabet::global.lookup(aminoAcid);
}



template<class T>
ApproxAminoAlignment<T> *ApproxAminoAlignment<T>::load(istream &is,
                            const SubstitutionMatrix<T> &M,
		            const Sequence &seq1,const char *seqStr1,
		            const Sequence &seq2,const char *seqStr2,
			    StringMap<char> &stopCodons)
{
  ApproxAminoAlignment<T> *a=
    new ApproxAminoAlignment<T>(is,M,seq1,seqStr1,seq2,seqStr2,stopCodons);
  if(a->cells.size()==0) {delete a; return NULL;}
  return a;
}



template<class T>
void ApproxAminoAlignment<T>::extend(const Sequence &seq1,
				     const char *seqStr1,
				     const Sequence &seq2,
const char *seqStr2,
				     StringMap<char> &stopCodons)
{
  // First, extend the alignment left and right as far as possible without
  // passing any in-frame stop codons
  extendLeft(seqStr1,seqStr2,stopCodons);
  extendRight(seqStr1,seq1.getLength(),seqStr2,seq2.getLength(),stopCodons);

  // Now process the cells that were added from left to right to compute
  // the cumulative match count and alignment length.
  recomputeCumulatives(seq1,seqStr1,seq2,seqStr2);
}



template<class T>
void ApproxAminoAlignment<T>::extendLeft(const char *s1,const char *s2,
					 StringMap<char> &stopCodons)
{
  // Extend leftward, adding cells until either the beginning of either
  // sequence is reached, or a stop codon is encountered in either genome
  ApproxAlignCell &firstCell=cells[0];
  int x=firstCell.getX(), y=firstCell.getY();
  if(stopCodons.isDefined(s1,x,3) ||
     stopCodons.isDefined(s2,y,3)) return;
  while(x>2 && y>2)
    {
      x-=3;
      y-=3;
      if(stopCodons.isDefined(s1,x,3) ||
	 stopCodons.isDefined(s2,y,3)) 
	break;//### check that x & y aren't reversed...
      cells.push_front(ApproxAlignCell(x,y,MATCH,-1,-1));
    }
}



template<class T>
void ApproxAminoAlignment<T>::extendRight(const char *s1,int len1,
					  const char *s2,int len2,
					  StringMap<char> &stopCodons)
{
  // Extend rightward, adding cells until either the end of either 
  // sequence is reached or a stop codon is encountered in either genome
  ApproxAlignCell &lastCell=cells[cells.size()-1];
  int x=lastCell.getX(), y=lastCell.getY();
  if(stopCodons.isDefined(s1,x,3) ||
     stopCodons.isDefined(s2,y,3)) return;
  int lastPos1=len1-5, lastPos2=len2-5;
  while(x<lastPos1 && y<lastPos2)
    {
      x+=3;
      y+=3;
      if(stopCodons.isDefined(s1,x,3) ||
	 stopCodons.isDefined(s2,y,3)) 
	break;//### check that x & y aren't reversed...
      cells.push_back(ApproxAlignCell(x,y,MATCH,-1,-1));
    }
}



template<class T>
void ApproxAminoAlignment<T>::recomputeCumulatives(const Sequence &seq1,
						   const char *seqStr1,
						   const Sequence &seq2,
						   const char *seqStr2)
{
  int n=cells.size(), length=0;
  T matches=0;
  for(int i=0 ; i<n ; ++i)
    {
      ApproxAlignCell &cell=cells[i];
      switch(cell.getMatchType())
	{
	case MATCH:
	  {
	  Symbol s1=getResidue(seq1,seqStr1,cell.getX());
	  Symbol s2=getResidue(seq2,seqStr2,cell.getY());
#ifdef SUM_BLOSUM_SCORES
	  matches+=M(s1,s2)*3;
#else
	  if(M(s1,s2)>0) ++matches;
#endif
	  }
	  break;
	case FIRST_UNMATCHED:
#ifdef SUM_BLOSUM_SCORES
	  //###matches+=M.smallestEntry()*3;
#endif
	  break;
	case SECOND_UNMATCHED:
#ifdef SUM_BLOSUM_SCORES
	  //###matches+=M.smallestEntry()*3;
#endif
	  break;
	}
#ifdef SUM_BLOSUM_SCORES
      length+=3;
#else
      length++;
#endif
      cell.setCumulatives(matches,length);
    }
}

#endif
