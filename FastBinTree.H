/***********************************************************************
 FastBinTree.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_FastBinTree_H
#define INCL_BOOM_FastBinTree_H
#include "BinaryTree.H"
using namespace std;

/*============================================================
  BOOM::FastBinTree -- a binary tree template in which each node
  has a pointer to its left and right children, and ALSO a
  pointer to its parent.  Thus, finding a node's parent does NOT
  require searching down from the root, and is therefore very
  fast.  However, more memory is used by that extra pointer,
  and extra time for each insertion to initialize the parent 
  pointer.  This class also allows duplicate elements to be stored
  in the tree (if desired).
  ============================================================*/

namespace BOOM {
template<class T>
struct FastBinNode : public BaseBinTreeNode<T>
{
  FastBinNode<T> *parent;
  FastBinNode() : parent(NULL) {}
};


template <class T>
class FastBinTree : public BinaryTree<T>
{
public:
  FastBinTree(Comparator<T>& = *new DirectComparator<T>());
  void enableDuplicates();
  virtual bool insert(T);
protected:
  virtual BaseBinTreeNode<T> *getParent(BaseBinTreeNode<T> *);
  virtual bool insert(BaseBinTreeNode<T> *);
  virtual bool naiveInsert(FastBinNode<T> *);
  virtual void deleteNode(BaseBinTreeNode<T> *);
private:
  bool allowDuplicates;
};
}
using namespace BOOM;


template <class T>
BOOM::FastBinTree<T>::FastBinTree(BOOM::Comparator<T> &c) 
  : allowDuplicates(false),
    BOOM::BinaryTree<T>(c)
{
}



template <class T>
BOOM::BaseBinTreeNode<T> *BOOM::FastBinTree<T>::getParent(
   BOOM::BaseBinTreeNode<T> *thisNode)
{
  BOOM::FastBinNode<T> *fastNode=static_cast<BOOM::FastBinNode<T>*>(thisNode);
  return fastNode->parent;
}



template <class T>
bool FastBinTree<T>::insert(BaseBinTreeNode<T> *newNode) 
{
  return naiveInsert(static_cast<FastBinNode<T>*>(newNode)); 
}



template <class T>
bool FastBinTree<T>::naiveInsert(FastBinNode<T> *newNode)
{
  if(!this->root)
    {	
      this->root=newNode;
      newNode->left=newNode->right=newNode->parent=NULL;
      this->cardinality=1;
      return true;
    }
   
  FastBinNode<T> *thisNode=static_cast<FastBinNode<T>*>(this->root);
  while(true) 
    {
      // CASE -1 : LESS
      if(this->comparator.less(newNode->element,thisNode->element))
	{
	  if(!thisNode->left)
	    {
	      thisNode->left=newNode;
	      newNode->left=newNode->right=NULL;
	      newNode->parent=thisNode;
	      ++this->cardinality;
	      return true;
	    }
	  thisNode=static_cast<BOOM::FastBinNode<T>*>(thisNode->left);  
	  continue;
	}

      // CASE 0 : EQUAL
      else if(this->comparator.equal(newNode->element,thisNode->element))
	{
          if(!allowDuplicates)
	    return false;
	  // otherwise, fall through to CASE 1 below
	}

      // CASE 1 : GREATER
      if(!thisNode->right)
	{
	  thisNode->right=newNode;
	  newNode->left=newNode->right=NULL;
	  newNode->parent=thisNode;
	  ++this->cardinality;
	  return true;
	}
      
      thisNode=static_cast<BOOM::FastBinNode<T>*>(thisNode->right);
      continue;
    }
}



template <class T>
void BOOM::FastBinTree<T>::deleteNode(BOOM::BaseBinTreeNode<T> *baseNode)
{
  BOOM::FastBinNode<T> *thisNode=
    static_cast<BOOM::FastBinNode<T>*>(baseNode);
  int numChildren=0;
  BOOM::FastBinNode<T> *child, *mySuccessor, *hisParent;
  if(thisNode->left) numChildren++;
  if(thisNode->right) numChildren++;
   
  switch(numChildren) 
    {
    case 0:
      if(thisNode->parent)
	if(thisNode->parent->left==thisNode)
	  thisNode->parent->left=NULL;
	else
	  thisNode->parent->right=NULL;
      else  
	this->root=NULL;
      thisNode->parent=NULL;
      break;
      
    case 1:  
      if(thisNode->left)
	{
	  child=static_cast<BOOM::FastBinNode<T>*>(thisNode->left);
	  thisNode->left=NULL;
	}
      else
	{
	  child=static_cast<BOOM::FastBinNode<T>*>(thisNode->right);
	  thisNode->right=NULL;
	}
       
      if(thisNode->parent)
	{
	  if(thisNode->parent->left==thisNode)
	    thisNode->parent->left=child;
	  else
	    thisNode->parent->right=child;
	  child->parent=thisNode->parent;
	}
      else  
	{
	  this->root=child;
	  child->parent=NULL;
	}
      thisNode->parent=NULL;
      break;
      
    case 2:
      mySuccessor=static_cast<BOOM::FastBinNode<T>*>(thisNode->right);
      hisParent=NULL;
      while(mySuccessor->left)
	{
	  hisParent=mySuccessor;
	  mySuccessor=static_cast<BOOM::FastBinNode<T>*>(mySuccessor->left);
	}
      
      if(hisParent)  
	{
	  hisParent->left=mySuccessor->right;
	  if(mySuccessor->right)
	    static_cast<BOOM::FastBinNode<T>*>(mySuccessor->right)->parent=
	      hisParent;

	  mySuccessor->left=thisNode->left;
	  static_cast<BOOM::FastBinNode<T>*>(thisNode->left)->parent=
	    mySuccessor;
	  
	  mySuccessor->right=thisNode->right;
	  static_cast<BOOM::FastBinNode<T>*>(thisNode->right)->parent=
	    mySuccessor;
	  
	  if(thisNode->parent)
	    {
	      if(thisNode->parent->left==thisNode)
		thisNode->parent->left=mySuccessor;
	      else
		thisNode->parent->right=mySuccessor;
	      mySuccessor->parent=thisNode->parent;
	    }
	  else  
	    {
	      this->root=mySuccessor;
	      mySuccessor->parent=NULL;
	    }
	  thisNode->left=thisNode->right=thisNode->parent=NULL;
	}
      
      else  
	{         	
	  mySuccessor->left=thisNode->left;
	  static_cast<BOOM::FastBinNode<T>*>(thisNode->left)->parent=
	    mySuccessor;
	  
	  if(thisNode->parent)
	    {
	      if(thisNode->parent->left==thisNode)
		thisNode->parent->left=mySuccessor;
	      else
		thisNode->parent->right=mySuccessor;
	      mySuccessor->parent=thisNode->parent;
	    }
	  else  
	    {
	      this->root=mySuccessor;
	      mySuccessor->parent=NULL;
	    }
	  
	  thisNode->left=thisNode->right=thisNode->parent=NULL;
	}
      break;
    }

  --this->cardinality;
  delete thisNode;
}



template <class T>
void BOOM::FastBinTree<T>::enableDuplicates() 
{ 
  allowDuplicates=true; 
}



template <class T>
bool BOOM::FastBinTree<T>::insert(T t)
{
  BOOM::FastBinNode<T> *n=new BOOM::FastBinNode<T>;
  n->element=t;
  if(insert(n)) return true;
  delete n;
  return false;
}


#endif
