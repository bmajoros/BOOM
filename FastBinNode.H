/***********************************************************************
 FastBinNode.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_FastBinNode_H
#define INCL_BOOM_FastBinNode_H
#include "BaseBinTreeNode.H"
using namespace std;

namespace BOOM {
class FastBinNode : public BOOM::BaseBinTreeNode
{
public:
  BOOM::FastBinNode *parent;
};
}

#endif
