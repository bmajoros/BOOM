/****************************************************************
 BandedSmithWaterman.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.

 Implements the Smith-Waterman global alignment algorithm.  The 
 matrix is banded, so that both time and space are conserved by 
 not allocating & evaluating the whole matrix.
 ****************************************************************/
#ifndef INCL_BandedSmithWaterman_H
#define INCL_BandedSmithWaterman_H
#include "Alignment.H"
#include "BandedAlignmentMatrix.H"
#include "SubstitutionMatrix.H"

template<class FLT_TYP>
class BandedSmithWaterman
{
public:
  BandedSmithWaterman(Alphabet &,const Sequence &,const Sequence &,
		      const SubstitutionMatrix<FLT_TYP> &,
		      FLT_TYP gapOpenPenalty,FLT_TYP gapExtendPenalty,
		      int bandWidth);
  Alignment *fullAlignment();
  static int recommendBandWidth(int maxBytes,int xDim);
  enum Direction {LEFT,DOWN,DIAGONAL};
private:
  struct Cell
  {
    FLT_TYP scores[3];  
    Direction links[3];
    Direction bestDir();
  };
  Alphabet &alphabet;
  const Sequence &s1, &s2;
  BandedAlignmentMatrix<Cell> alignmentMatrix; // M[column][row]
  const SubstitutionMatrix<FLT_TYP> &substitutionMatrix;
  const FLT_TYP gapOpenPenalty, gapExtendPenalty;
  Alignment *getAlignment(int finalRow,int finalColumn);
  Alignment *align(FLT_TYP finalOpenPenalty,
		   FLT_TYP finalExtendPenalty);
  void linkBack(Cell &thisCell,Cell *leftCell,Cell *downCell,
		Cell *diagonalCell,FLT_TYP gapOpenPenalty,
		FLT_TYP gapExtendPenalty,int x,int y);
  void followLink(Direction,int &x,int &y);
};

template<class FLT_TYP>
ostream &operator<<(ostream &,
		    typename BandedSmithWaterman<FLT_TYP>::Direction);



#include "Stack.H"
#include "Constants.H"


template<class FLT_TYP>
BandedSmithWaterman<FLT_TYP>::BandedSmithWaterman(Alphabet &alphabet,
						  const Sequence &s1,
						  const Sequence &s2,
			    const SubstitutionMatrix<FLT_TYP> &subst,
				              FLT_TYP gapOpenPenalty,
					    FLT_TYP gapExtendPenalty,
						       int bandWidth)
  : s1(s1), s2(s2), alphabet(alphabet), 
    alignmentMatrix(s1.getLength()+1,s2.getLength()+1,bandWidth),
    gapOpenPenalty(gapOpenPenalty), gapExtendPenalty(gapExtendPenalty),
    substitutionMatrix(subst)
{
  if(s1.getLength()<1 || s2.getLength()<1)
    throw "Aligning to a zero-length sequence in BandedSmithWaterman::\
BandedSmithWaterman";
  if(bandWidth<0) throw "negative bandwidth";
  if(bandWidth>s1.getLength()) bandWidth=s1.getLength();
  if(bandWidth>s2.getLength()) bandWidth=s2.getLength();

  //cout<<"matrix size: "<<alignmentMatrix.getMatrixSize()<<" bytes"<<endl;
}



template<class FLT_TYP>
typename BandedSmithWaterman<FLT_TYP>::Direction 
  BandedSmithWaterman<FLT_TYP>::Cell::bestDir()
{
  FLT_TYP left=scores[BandedSmithWaterman<FLT_TYP>::LEFT]; 
  FLT_TYP down=scores[BandedSmithWaterman<FLT_TYP>::DOWN];
  FLT_TYP diag=scores[BandedSmithWaterman<FLT_TYP>::DIAGONAL];
  if(left>down)
    if(left>diag)
      return BandedSmithWaterman<FLT_TYP>::LEFT;
    else
      return BandedSmithWaterman<FLT_TYP>::DIAGONAL;
  else
    if(down>diag)
      return BandedSmithWaterman<FLT_TYP>::DOWN;
    else
      return BandedSmithWaterman<FLT_TYP>::DIAGONAL;
}



template<class FLT_TYP>
ostream &operator<<(ostream &os,
		    typename BandedSmithWaterman<FLT_TYP>::Direction dir)
{
  switch(dir)
    {
    case BandedSmithWaterman<FLT_TYP>::LEFT:
      os<<"LEFT"<<endl;
      break;
    case BandedSmithWaterman<FLT_TYP>::DOWN:
      os<<"DOWN"<<endl;
      break;
    case BandedSmithWaterman<FLT_TYP>::DIAGONAL:
      os<<"DIAGONAL"<<endl;
      break;
    }
}



template<class FLT_TYP>
Alignment *BandedSmithWaterman<FLT_TYP>::getAlignment(int x,int y)
{
  // Trace back from the end cell along the optimal path to
  // the origin in the lower left corner of the DP matrix

  Cell *cell;
  if(!alignmentMatrix.getCell(x,y,cell)) throw "BandedSmithWaterman: bad!";
  Direction dir=cell->bestDir();
  FLT_TYP alignmentScore=cell->scores[dir];

  Stack<Direction> pathStack;
  while(x!=0 || y!=0)
    {
      if(!alignmentMatrix.getCell(x,y,cell)) 
	throw "BandedSmithWaterman: bad! (2)";
     
      pathStack.push(dir);
      followLink(dir,x,y);
      dir=cell->links[dir];
    }

  // Reverse the stack contents to produce a forward-oriented
  // alignment
  Alignment &alignment=*new Alignment(s1,s2,alphabet,alignmentScore);
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



template<class FLT_TYP>
Alignment *BandedSmithWaterman<FLT_TYP>::fullAlignment()
{
  return align(gapOpenPenalty,gapExtendPenalty);
}



template<class FLT_TYP>
Alignment *BandedSmithWaterman<FLT_TYP>::align(FLT_TYP finalOpenPenalty,
					 FLT_TYP finalExtendPenalty)
{
  int matrixWidth=alignmentMatrix.getFirstDim();
  int matrixHeight=alignmentMatrix.getSecondDim();

  // Initialize the bottom row and leftmost column of the dynamic 
  // programming matrix
  Cell *origin;
  if(!alignmentMatrix.getCell(0,0,origin)) throw "BandedSmithWaterman";
  for(int i=0 ; i<3 ; ++i) origin->scores[i]=0;
  for(int i=1 ; i<matrixWidth ; ++i)
    {
      Cell *bottomCell;
      if(!alignmentMatrix.getCell(i,0,bottomCell)) continue;
      bottomCell->links[LEFT]=LEFT;
      Cell *left;
      if(!alignmentMatrix.getCell(i-1,0,left)) throw "BandedSW";
      bottomCell->scores[LEFT]=left->scores[LEFT]+gapExtendPenalty;
      bottomCell->scores[DOWN]=
	bottomCell->scores[DIAGONAL]=NEGATIVE_INFINITY;
    }
  for(int i=1 ; i<matrixHeight ; ++i)
    {
      Cell *leftCell;
      if(!alignmentMatrix.getCell(0,i,leftCell)) continue;
      leftCell->links[DOWN]=DOWN;
      Cell *down;
      if(!alignmentMatrix.getCell(0,i-1,down)) throw "BandedSmith-W";
      leftCell->scores[DOWN]=down->scores[DOWN]+gapExtendPenalty;
      leftCell->scores[LEFT]=leftCell->scores[DIAGONAL]=NEGATIVE_INFINITY;
    }

  // Fill out the main portion of the matrix
  int widthMinusOne=matrixWidth-1, heightMinusOne=matrixHeight-1;
  for(int x=1 ; x<widthMinusOne ; ++x)
    for(int y=1 ; y<heightMinusOne ; ++y)
      {
	Cell *thisCell;
	if(!alignmentMatrix.getCell(x,y,thisCell)) continue;
	Cell *leftCell=NULL, *downCell=NULL, *diagonalCell=NULL;
	alignmentMatrix.getCell(x-1,y,leftCell);
	alignmentMatrix.getCell(x,y-1,downCell);
	alignmentMatrix.getCell(x-1,y-1,diagonalCell);
	linkBack(*thisCell,leftCell,downCell,diagonalCell,
		 gapOpenPenalty,gapExtendPenalty,x,y);
      }

  // Finish up by filling out the top row and rightmost column
  // (these use a *potentially* different set of gap penalties to
  // accommodate "partial" alignments)
  for(int i=1 ; i<matrixWidth ; ++i)
    {
      Cell *topCell;
      if(!alignmentMatrix.getCell(i,heightMinusOne,topCell)) continue;
      Cell *topCellLeft=NULL, *topCellDown=NULL, *topCellDiagonal=NULL;
      alignmentMatrix.getCell(i-1,heightMinusOne,topCellLeft);
      alignmentMatrix.getCell(i,heightMinusOne-1,topCellDown);
      alignmentMatrix.getCell(i-1,heightMinusOne-1,topCellDiagonal);
      linkBack(*topCell,topCellLeft,topCellDown,topCellDiagonal,
	       finalOpenPenalty,finalExtendPenalty,i,heightMinusOne);
    }
  for(int i=1 ; i<matrixHeight ; ++i)
    {
      Cell *rightmostCell;
      if(!alignmentMatrix.getCell(widthMinusOne,i,rightmostCell)) continue;
      Cell *rightmostCellLeft=NULL, *rightmostCellDown=NULL,
	*rightmostCellDiagonal=NULL;
      alignmentMatrix.getCell(widthMinusOne-1,i,rightmostCellLeft);
      alignmentMatrix.getCell(widthMinusOne,i-1,rightmostCellDown);
      alignmentMatrix.getCell(widthMinusOne-1,i-1,rightmostCellDiagonal);
      linkBack(*rightmostCell,rightmostCellLeft,rightmostCellDown,
	       rightmostCellDiagonal,finalOpenPenalty,finalExtendPenalty,
	       widthMinusOne,i);
    }

  return getAlignment(widthMinusOne,heightMinusOne);
}



/************************************************************************
 This method handles the actual linking back of one cell to a predecessor
 cell, either immediately left, below, or diagonally of this cell.  Note
 that we do not allow insertions directly next to deletions -- at least
 one match must come between adjacent insertions and deletions, although
 this could could easily be changed to relax this restriction.
 */
template<class FLT_TYP>
void BandedSmithWaterman<FLT_TYP>::linkBack(Cell &thisCell,
					    Cell *leftCell,
					    Cell *downCell,
					    Cell *diagonalCell,
					    FLT_TYP gapOpenPenalty,
					    FLT_TYP gapExtendPenalty,
					    int x,int y)
{
  // LEFT
  FLT_TYP leftLeft, leftDiag;
  if(leftCell)
    {
      leftLeft=leftCell->scores[LEFT]+gapExtendPenalty;
      leftDiag=leftCell->scores[DIAGONAL]+gapOpenPenalty;
    }
  else 
    leftLeft=leftDiag=NEGATIVE_INFINITY;
  if(leftLeft>leftDiag)
    {
      thisCell.scores[LEFT]=leftLeft;
      thisCell.links[LEFT]=LEFT;
    }
  else
    {
      thisCell.scores[LEFT]=leftDiag;
      thisCell.links[LEFT]=DIAGONAL;
    }

  // DOWN
  FLT_TYP downDown, downDiag;
  if(downCell)
    {
      downDown=downCell->scores[DOWN]+gapExtendPenalty;
      downDiag=downCell->scores[DIAGONAL]+gapOpenPenalty;
    }
  else
    downDown=downDiag=NEGATIVE_INFINITY;
  if(downDown>downDiag)
    {
      thisCell.scores[DOWN]=downDown;
      thisCell.links[DOWN]=DOWN;
    }
  else
    {
      thisCell.scores[DOWN]=downDiag;
      thisCell.links[DOWN]=DIAGONAL;
    }
  
  // DIAGONAL
  FLT_TYP matchScore=substitutionMatrix(s1[x-1],s2[y-1]);
  FLT_TYP diagLeft, diagDown, diagDiag;
  if(diagonalCell)
    {
      diagLeft=diagonalCell->scores[LEFT]+matchScore;
      diagDown=diagonalCell->scores[DOWN]+matchScore;
      diagDiag=diagonalCell->scores[DIAGONAL]+matchScore;
    }
  else
    diagLeft=diagDown=diagDiag=NEGATIVE_INFINITY;
  if(diagLeft>diagDown)
    if(diagLeft>diagDiag)
      { // left is best
	thisCell.scores[DIAGONAL]=diagLeft;
	thisCell.links[DIAGONAL]=LEFT;
      }
    else goto DIAGDIAG;
  else 
    if(diagDown>diagDiag)
      { // down is best
	thisCell.scores[DIAGONAL]=diagDown;
	thisCell.links[DIAGONAL]=DOWN;
      }
    else // diagonal is best
      {
      DIAGDIAG:
	thisCell.scores[DIAGONAL]=diagDiag;
	thisCell.links[DIAGONAL]=DIAGONAL;
      }
}



template<class FLT_TYP>
void BandedSmithWaterman<FLT_TYP>::followLink(Direction dir,int &x,int &y)
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



template<class FLT_TYP>
int BandedSmithWaterman<FLT_TYP>::recommendBandWidth(int maxBytes,int xDim)
{
  return BandedAlignmentMatrix<Cell>::recommendBandWidth(maxBytes,xDim);
}



#endif
