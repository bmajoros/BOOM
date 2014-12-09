/***********************************************************************
 Needleman.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_Needleman_H
#define INCL_Needleman_H
#include "Alignment.H"
#include "Array2D.H"
#include "SubstitutionMatrix.H"
#include "Stack.H"


/****************************************************************
 Implements the Needleman-Wunsch global alignment algorithm.  Can
 perform "partial" alignment -- i.e., where terminal indels are
 not penalized.
 ****************************************************************/
namespace BOOM {
template<class SCORE_TYPE>
class Needleman
{
public:
  Needleman(Alphabet &,const Sequence &,const Sequence &,
	    const SubstitutionMatrix<SCORE_TYPE> &,
	    SCORE_TYPE gapExtendPenalty);
  Alignment *fullAlignment();
  Alignment *partialAlignment();
  SCORE_TYPE getPartialScore();
private:
  enum Direction
    {
      LEFT,
      DOWN,
      DIAGONAL
    };
  struct Cell
  {
    SCORE_TYPE score;
    Direction link;
    void set(SCORE_TYPE score,Direction);
  };
  Alphabet &alphabet;
  const Sequence &s1, &s2;
  Array2D<Cell> alignmentMatrix; // index like this: M[column][row]
  const SubstitutionMatrix<SCORE_TYPE> &substitutionMatrix;
  const SCORE_TYPE gapExtendPenalty;
  Alignment *getAlignment(int finalRow,int finalColumn);
  Alignment *align(SCORE_TYPE finalExtendPenalty);
  void linkBack(Cell &thisCell,Cell &leftCell,Cell &downCell,
		Cell &diagonalCell,SCORE_TYPE gapExtendPenalty,int x,int y);
  void followLink(Direction,int &x,int &y);
};
}
using namespace BOOM;


template<class SCORE_TYPE>
Needleman<SCORE_TYPE>::Needleman(Alphabet &alphabet,const Sequence &s1,
				 const Sequence &s2,
				 const SubstitutionMatrix<SCORE_TYPE> &
				   subst,
				 SCORE_TYPE gapExtendPenalty)
  : s1(s1), s2(s2), alphabet(alphabet), 
    alignmentMatrix(s1.getLength()+1,s2.getLength()+1),
    gapExtendPenalty(gapExtendPenalty),
    substitutionMatrix(subst)
{
}



template<class SCORE_TYPE>
Alignment *Needleman<SCORE_TYPE>::getAlignment(int x,int y)
{
  // Trace back from the end cell along the optimal path to
  // the origin in the lower left corner of the DP matrix
  SCORE_TYPE score=alignmentMatrix[x][y].score;
  Stack<Direction> pathStack;
  while(x!=0 || y!=0)
    {
      Cell &cell=alignmentMatrix[x][y];
      pathStack.push(cell.link);
      followLink(cell.link,x,y);
    }

  // Reverse the stack contents to produce a forward-oriented
  // alignment
  Alignment &alignment=*new Alignment(s1,s2,alphabet,score);
  const int len=pathStack.size();
  for(int i=0 ; i<len ; ++i)
    {
      Direction dir=pathStack.pop();
      switch(dir)
	{
	case LEFT:
	  alignment+=FIRST_UNMATCHED;
	  break;
	case DOWN:
	  alignment+=SECOND_UNMATCHED;
	  break;
	case DIAGONAL:
	  alignment+=MATCH;
	  break;
	}
    }
  return &alignment;
}



template<class SCORE_TYPE>
Alignment *Needleman<SCORE_TYPE>::fullAlignment()
{
  return align(gapExtendPenalty);
}



template<class SCORE_TYPE>
Alignment *Needleman<SCORE_TYPE>::partialAlignment()
{
  return align(0);
}



template<class SCORE_TYPE>
Alignment *Needleman<SCORE_TYPE>::align(SCORE_TYPE finalExtendPenalty)
{
  int matrixWidth=alignmentMatrix.getFirstDim();
  int matrixHeight=alignmentMatrix.getSecondDim();

  // Initialize the bottom row and leftmost column of the dynamic 
  // programming matrix
  Cell &origin=alignmentMatrix[0][0];
  origin.score=0;
  origin.link=DIAGONAL; // <---doesn't matter, should delete this line...
  for(int i=1 ; i<matrixWidth ; ++i)
    {
      Cell &bottomCell=alignmentMatrix[i][0];
      bottomCell.link=LEFT;
      bottomCell.score=alignmentMatrix[i-1][0].score+gapExtendPenalty;
    }
  for(int i=1 ; i<matrixHeight ; ++i)
    {
      Cell &leftCell=alignmentMatrix[0][i];
      leftCell.link=DOWN;
      leftCell.score=alignmentMatrix[0][i-1].score+gapExtendPenalty;
    }

  // Fill out the main portion of the matrix
  int widthMinusOne=matrixWidth-1, heightMinusOne=matrixHeight-1;
  for(int x=1 ; x<widthMinusOne ; ++x)
    for(int y=1 ; y<heightMinusOne ; ++y)
      {
	Cell &thisCell=alignmentMatrix[x][y],
	  &leftCell=alignmentMatrix[x-1][y],
	  &downCell=alignmentMatrix[x][y-1],
	  &diagonalCell=alignmentMatrix[x-1][y-1];
	linkBack(thisCell,leftCell,downCell,diagonalCell,
		 gapExtendPenalty,x,y);
      }

  // Finish up by filling out the top row and rightmost column
  // (these use a *potentially* different set of gap penalties to
  // accommodate "partial" alignments)
  if(heightMinusOne)
    for(int i=1 ; i<matrixWidth ; ++i)
      {
	Cell &topCell=alignmentMatrix[i][heightMinusOne],
	  topCellLeft=alignmentMatrix[i-1][heightMinusOne],
	  topCellDown=alignmentMatrix[i][heightMinusOne-1],
	  topCellDiagonal=alignmentMatrix[i-1][heightMinusOne-1];
	linkBack(topCell,topCellLeft,topCellDown,topCellDiagonal,
		 finalExtendPenalty,i,heightMinusOne);
      }
  if(widthMinusOne)
    for(int i=1 ; i<matrixHeight ; ++i)
      {
	Cell &rightmostCell=alignmentMatrix[widthMinusOne][i],
	  &rightmostCellLeft=alignmentMatrix[widthMinusOne-1][i],
	  &rightmostCellDown=alignmentMatrix[widthMinusOne][i-1],
	  &rightmostCellDiagonal=alignmentMatrix[widthMinusOne-1][i-1];
	linkBack(rightmostCell,rightmostCellLeft,rightmostCellDown,
		 rightmostCellDiagonal,finalExtendPenalty,
		 widthMinusOne,i);
      }

  return getAlignment(widthMinusOne,heightMinusOne);
}



template<class SCORE_TYPE>
void Needleman<SCORE_TYPE>::linkBack(Cell &thisCell,Cell &leftCell,
				     Cell &downCell,
				     Cell &diagonalCell,
				     SCORE_TYPE gapExtendPenalty,
				     int x,int y)
{
  SCORE_TYPE leftScore=leftCell.score+gapExtendPenalty;
  SCORE_TYPE downScore=downCell.score+gapExtendPenalty;
  SCORE_TYPE diagonalScore=
    diagonalCell.score+substitutionMatrix(s1[x-1],s2[y-1]);
  if(leftScore>downScore)
    if(leftScore>diagonalScore) thisCell.set(leftScore,LEFT);
    else thisCell.set(diagonalScore,DIAGONAL);
  else 
    if(downScore>diagonalScore) thisCell.set(downScore,DOWN);
    else thisCell.set(diagonalScore,DIAGONAL);
}



template<class SCORE_TYPE>
void Needleman<SCORE_TYPE>::Cell::set(SCORE_TYPE s,Direction dir)
{
  score=s;
  link=dir;
}



template<class SCORE_TYPE>
void Needleman<SCORE_TYPE>::followLink(Direction dir,int &x,int &y)
{
  switch(dir)
    {
    case LEFT:
      --x;
      break;
    case DOWN:
      --y;
      break;
    case DIAGONAL:
      --x;
      --y;
      break;
    }
}



template<class SCORE_TYPE>
SCORE_TYPE Needleman<SCORE_TYPE>::getPartialScore()
{
  // Trace back from the end cell along the optimal path to
  // the origin in the lower left corner of the DP matrix
  int x=alignmentMatrix.getFirstDim()-1;
  int y=alignmentMatrix.getSecondDim()-1;
  while(x!=0 || y!=0)
    {
      Cell &cell=alignmentMatrix[x][y];
      if(cell.link==DIAGONAL) return cell.score;
      followLink(cell.link,x,y);
    }
  return 0;
}



#endif
