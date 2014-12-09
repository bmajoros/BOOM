/***********************************************************************
 UngappedAligner.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_UngappedAligner_H
#define INCL_UngappedAligner_H
#include "Sequence.H"
#include "Array2D.H"
#include "SubstitutionMatrix.H"

namespace BOOM {
template<class T>
class UngappedAligner
{
public:
  UngappedAligner(Alphabet &,const Sequence &,const Sequence &,
		  const SubstitutionMatrix<T> &);
  T getScore() { return score; }
private:
  Array2D<T> alignmentMatrix; // index like this: M[column][row]
  T score;

  inline void fillMatrix(Alphabet &,const Sequence &,const Sequence &,
			 const SubstitutionMatrix<T> &);
};
}
using namespace BOOM;


template<class T>
void UngappedAligner<T>::fillMatrix(Alphabet &alphabet,
			    const Sequence &seq1,
			    const Sequence &seq2,
			    const SubstitutionMatrix<T> &substMatrix)
{
  int n1=seq1.getLength(), n2=seq2.getLength();

  // Initialize boundary conditions (lower/left edges of matrix)
  Symbol x0=seq1[0], y0=seq2[0];
  for(int x=1 ; x<n1 ; ++x) 
    alignmentMatrix[x][0]=substMatrix(seq1[x],y0);
  for(int y=1 ; y<n2 ; ++y) 
    alignmentMatrix[0][y]=substMatrix(x0,seq2[y]);

  // Fill out interior of matrix
  for(int x=0 ; x<n1 ; ++x)
    for(int y=0 ; y<n2 ; ++y)
      {
	T newEntry=
	  alignmentMatrix[x-1][y-1]+
	  substMatrix(seq1[x],seq2[y]);
	alignmentMatrix[x][y]=newEntry;
      }

  // Compute score
  score=alignmentMatrix[n1-1][n2-1];
  for(int x=0 ; x<n1 ; ++x) score+=alignmentMatrix[x][n2-1];
  for(int y=0 ; y<n2 ; ++y) score+=alignmentMatrix[n1-1][y];
  cout<<score<<"\t"<<n1<<"\t"<<n2<<"\t"<<n1*n2<<endl;
  score/=( /*substMatrix.getMeanValue()* */ n1*n2);
  score=exp(score);
}



template<class T>
UngappedAligner<T>::UngappedAligner(Alphabet &alphabet,const Sequence &seq1,
			    const Sequence &seq2,
			    const SubstitutionMatrix<T> &substMatrix)
  : alignmentMatrix(seq1.getLength(),seq2.getLength())
{
  fillMatrix(alphabet,seq1,seq2,substMatrix);
}



#endif
