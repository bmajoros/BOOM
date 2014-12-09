/***********************************************************************
 BinaryTree.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_BinaryTree_H
#define INCL_BOOM_BinaryTree_H
using namespace std;
#include <iostream>
#include "Comparator.H"
#include "Exceptions.H"

/*============================================================
  BOOM::BinaryTree -- a binary tree template in which each node
  has a pointer to its left and right children, but lacks a 
  pointer to its parent.  Thus, finding a node's parent requires
  searching down from the root.  If this is unacceptably slow,
  use the BOOM::FastBinaryTree instead (each node has a pointer to
  its parent).
  ============================================================*/

namespace BOOM {
template<class T>
struct BaseBinTreeNode
{
  BaseBinTreeNode<T> *left, *right;
  T element;
  BaseBinTreeNode() :left(NULL),right(NULL){}
};


template<class T>
class BinaryTree
{
public:
  /*
    enum IterationType
      {
	INORDER,   // tested
	POSTORDER, // tested
	PREORDER   // ### not tested
      };
  */
  class BinTreeIter
  {
    friend class BinaryTree<T>;
  public:
    enum IterationType
      {
	INORDER,   // tested
	POSTORDER, // tested
	PREORDER   // ### not tested
      };
    BinTreeIter(BinaryTree<T> *,IterationType=INORDER);
    T &operator*();
    operator bool();
    BinTreeIter &operator++();
    bool operator==(const BinTreeIter &) const;
  protected:
    enum FromType
      { 
	FROM_LEFT, 
	FROM_RIGHT, 
	FROM_PARENT, 
	FROM_NOWHERE 
      };
    BinTreeIter(BinaryTree<T> *,BaseBinTreeNode<T> *thisNode,
		    BaseBinTreeNode<T> *nextNode,FromType,
		    IterationType);
    virtual BaseBinTreeNode<T> *inOrder();
    virtual BaseBinTreeNode<T> *postOrder();
    virtual BaseBinTreeNode<T> *preOrder();
    virtual void reset();  
    IterationType iterationType;
    FromType fromWhere;
    BinaryTree<T> *myTree;
    BaseBinTreeNode<T> *thisNode, *nextNode;
  };
  typedef BinTreeIter iterator;
  
  // ### The following constructor will cause a memory leak if you
  //     use a large number of these binary trees with the default 
  //     argument:
  BinaryTree(BOOM::Comparator<T>& = *new DirectComparator<T>());
  virtual ~BinaryTree();                   // tested
  virtual bool insert(T);                      // tested
  virtual T &maximum() const;                  // tested
  virtual T &minimum() const;                  // tested
  virtual unsigned getCardinality();           
  bool isEmpty() const;                        // tested
  virtual void purge();                        // tested
  iterator begin();                            // tested
  iterator end();                              // tested
  void remove(T);                              // tested
  bool getPredecessor(T of,T &is);             // tested
  bool getSuccessor(T of,T &is);               // tested
  BOOM::Comparator<T> &getComparator();
protected:
  friend class BinTreeIter;
  BOOM::Comparator<T> &comparator;
  unsigned cardinality;
  BaseBinTreeNode<T> *root;
  virtual BOOM::BaseBinTreeNode<T> *subTreeMax(BOOM::BaseBinTreeNode<T> *) 
    const;
  virtual BOOM::BaseBinTreeNode<T> *subTreeMin(BOOM::BaseBinTreeNode<T> *) 
    const;
  virtual BOOM::BaseBinTreeNode<T> *getParent(BOOM::BaseBinTreeNode<T> *);
  virtual BOOM::BaseBinTreeNode<T> *getRoot();
  virtual BOOM::BaseBinTreeNode<T> *predecessor(BOOM::BaseBinTreeNode<T> *);
  virtual BOOM::BaseBinTreeNode<T> *successor(BOOM::BaseBinTreeNode<T> *);
  virtual bool insert(BOOM::BaseBinTreeNode<T> *);
  virtual void deleteNode(BOOM::BaseBinTreeNode<T> *);
  BOOM::BaseBinTreeNode<T> *find(T);
};
}



template<class T>
BOOM::BinaryTree<T>::BinaryTree(BOOM::Comparator<T> &c) 
  : root(NULL),
    cardinality(0),
    comparator(c)
{
}



template<class T>
BOOM::BinaryTree<T>::~BinaryTree()
{
  purge();
}



template<class T>
BOOM::Comparator<T> &BOOM::BinaryTree<T>::getComparator()
{
  return comparator;
}



template<class T>
BOOM::BaseBinTreeNode<T> *BOOM::BinaryTree<T>::getParent(
			     BOOM::BaseBinTreeNode<T> *thisNode)
{
  if(thisNode==root) return NULL;

  BOOM::BaseBinTreeNode<T> *parent=root;
  while(true)
    {
      if(comparator.less(thisNode->element,parent->element))
	{
	  if(parent->left==thisNode) 
	    return parent;
	  parent=parent->left;
	  continue;
	}
      else
	{
	  if(parent->right==thisNode) 
	    return parent;
	  parent=parent->right;
	  continue;
	}
    }
}



template<class T>
BOOM::BaseBinTreeNode<T> *BOOM::BinaryTree<T>::getRoot()
{
  return root;
}



template<class T>
T &BOOM::BinaryTree<T>::maximum() const
{
  return subTreeMax(root)->element;
}



template<class T>
T &BOOM::BinaryTree<T>::minimum() const
{
  if(!root) INTERNAL_ERROR;//###
  return subTreeMin(root)->element;
}



template<class T>
BOOM::BaseBinTreeNode<T> *BOOM::BinaryTree<T>::predecessor(
					BOOM::BaseBinTreeNode<T> *ofNode)
{
  BOOM::BaseBinTreeNode<T> *thisNode;
   
  if(ofNode->left) 
    return subTreeMax(ofNode->left);
   
  thisNode=getParent(ofNode);
  while(thisNode && (ofNode==thisNode->left))
    {
      ofNode=thisNode;
      thisNode=getParent(thisNode);
    }
  return thisNode;
}



template<class T>
BOOM::BaseBinTreeNode<T> *BOOM::BinaryTree<T>::subTreeMax(
			    BOOM::BaseBinTreeNode<T> *subTree) const
{
  if(!subTree) 
    return NULL;

  while(subTree->right) 
    subTree=subTree->right;

  return subTree;
}



template<class T>
BOOM::BaseBinTreeNode<T> *BOOM::BinaryTree<T>::subTreeMin(
			     BOOM::BaseBinTreeNode<T> *subTree) const
{
  if(!subTree) 
    return NULL;

  while(subTree->left) 
    subTree=subTree->left;

  return subTree;
}



template<class T>
BOOM::BaseBinTreeNode<T> *BOOM::BinaryTree<T>::successor(
				   BOOM::BaseBinTreeNode<T> *ofNode)
{
  BOOM::BaseBinTreeNode<T> *thisNode;
   
  if(ofNode->right) 
    return subTreeMin(ofNode->right);
   
  thisNode=getParent(ofNode);
  while(thisNode && (ofNode==thisNode->right))
    {
      ofNode=thisNode;
      thisNode=getParent(thisNode);
    }
  return thisNode;
}



template<class T>
bool BOOM::BinaryTree<T>::insert(T t)
{
  BOOM::BaseBinTreeNode<T> *n=new BOOM::BaseBinTreeNode<T>;
  n->element=t;
  if(insert(n)) return true;
  delete n;
  return false;
}



template<class T>
bool BOOM::BinaryTree<T>::insert(BOOM::BaseBinTreeNode<T> *newNode)
{
  if(!root)
    {	
      root=newNode;
      newNode->left=newNode->right=NULL;
      cardinality=1;
      return true;
    }
   
  BOOM::BaseBinTreeNode<T> *thisNode=root;
  while(true) 
    {
      if(comparator.less(newNode->element,thisNode->element))
	{
	  if(!thisNode->left)
	    {
	      thisNode->left=newNode;
	      newNode->left=newNode->right=NULL;
	      ++cardinality;
	      return true;
	    }
	  thisNode=thisNode->left; 
	  continue;
	}
      else if(comparator.equal(newNode->element,thisNode->element))
	return false;
      else
	{
	  if(!thisNode->right)
	    {
	      thisNode->right=newNode;
	      newNode->left=newNode->right=NULL;
	      ++cardinality;
	      return true;
	    }
	  thisNode=thisNode->right; 
	  continue;
	}
    }
}



template<class T>
unsigned BOOM::BinaryTree<T>::getCardinality()
{
  return cardinality;
}



template<class T>
void BOOM::BinaryTree<T>::deleteNode(BOOM::BaseBinTreeNode<T> *thisNode)
{
  int numChildren;
  BOOM::BaseBinTreeNode<T> *parent, *child, *mySuccessor, *hisParent;
  numChildren=0;
  if(thisNode->left) numChildren++;
  if(thisNode->right) numChildren++;
  parent=getParent(thisNode);

  switch(numChildren) 
    {
    case 0:
      if(parent)
	if(parent->left==thisNode)
	  parent->left=NULL;
	else
	  parent->right=NULL;
      else  
	root=NULL;
      break;
      
    case 1:
      if(thisNode->left)
	{
	  child=thisNode->left;
	  thisNode->left=NULL;
	}
      else
	{
	  child=thisNode->right;
	  thisNode->right=NULL;
	}
      if(parent)
	if(parent->left==thisNode)
	  parent->left=child;
	else
	  parent->right=child;
      else  
	root=child;
      break;

    case 2:
      mySuccessor=thisNode->right;
      hisParent=NULL;
      while(mySuccessor->left)
	{
	  hisParent=mySuccessor;
	  mySuccessor=mySuccessor->left;
	}
      
      if(hisParent)  
	{
	  hisParent->left=mySuccessor->right;
	  mySuccessor->right=thisNode->right;
	}

      mySuccessor->left=thisNode->left;
      if(parent)
	if(parent->left==thisNode)
	  parent->left=mySuccessor;
	else
	  parent->right=mySuccessor;
      else  
	root=mySuccessor;
      thisNode->left=thisNode->right=NULL;
      break;
    }

  delete thisNode;
  --cardinality;
}



template<class T>
void BOOM::BinaryTree<T>::purge()
{
  if(!root) return;

  BinTreeIter iterator(this,BinTreeIter::POSTORDER);
  while(true)
    {
      if(!iterator) break;
      delete iterator.thisNode;
      ++iterator;
    }

  cardinality=0;
  root=NULL;
}



template<class T>
typename BOOM::BinaryTree<T>::iterator BOOM::BinaryTree<T>::begin()
{
  return BinTreeIter(this);
}



template<class T>
typename BOOM::BinaryTree<T>::iterator BOOM::BinaryTree<T>::end()
{
  return BinTreeIter(this,NULL,NULL,BinTreeIter::FROM_NOWHERE,
			BinTreeIter::INORDER);
}



template<class T>
bool BOOM::BinaryTree<T>::isEmpty() const
{
  return cardinality==0;
}



template<class T>
BOOM::BaseBinTreeNode<T> *BOOM::BinaryTree<T>::find(T t)
{
  BOOM::BaseBinTreeNode<T> *n=root;
  while(n)
    if(comparator.less(t,n->element)) n=n->left;
    else if(comparator.equal(t,n->element)) return n;
    else n=n->right;
  return NULL;
}



template<class T>
void BOOM::BinaryTree<T>::remove(T t)
{
  BOOM::BaseBinTreeNode<T> *victim=find(t);
  if(victim) deleteNode(victim);
}



template<class T>
bool BOOM::BinaryTree<T>::getPredecessor(T of,T &is)
{
  BOOM::BaseBinTreeNode<T> *tp=find(of);
  if(!tp) throw "Element not found in BOOM::BinaryTree<T>::predecessor(T)";
  tp=predecessor(tp);
  if(!tp) return false;
  is=tp->element;
  return true;
}



template<class T>
bool BOOM::BinaryTree<T>::getSuccessor(T of,T &is)
{
  BOOM::BaseBinTreeNode<T> *tp=find(of);
  if(!tp) throw "Element not found in BOOM::BinaryTree<T>::successor(T)";
  tp=successor(tp);
  if(!tp) return false;
  is=tp->element;
  return true;
}


//==========================================================================
//                        BOOM::BinTreeIter methods
//==========================================================================


using namespace BOOM;

template<class T>
BinaryTree<T>::BinTreeIter::BinTreeIter(BinaryTree<T> *
						 ofTree,
				     IterationType iterationType)
  : iterationType(iterationType),
    myTree(ofTree)
{
  reset();

  switch(iterationType)
    {
    case INORDER: thisNode=inOrder(); break;
    case PREORDER: thisNode=preOrder(); break;
    case POSTORDER: thisNode=postOrder(); break;
    }
}



template <class T>
BinaryTree<T>::BinTreeIter::BinTreeIter(BinaryTree<T> *tree,
				   BaseBinTreeNode<T> *thisNode,
				   BaseBinTreeNode<T> *nextNode,
				   BinTreeIter::FromType fromWhere,
					      IterationType it)
  : iterationType(it),
    myTree(tree),
    thisNode(thisNode),
    nextNode(nextNode),
    fromWhere(fromWhere)
{
}



template<class T>
BOOM::BaseBinTreeNode<T> *BOOM::BinaryTree<T>::BinTreeIter::inOrder()
{
  BOOM::BaseBinTreeNode<T> *returnValue;
  
  while(true) 
    {
      if(nextNode==NULL) return NULL;
      switch(fromWhere) 
	{
	case FROM_LEFT:
	  returnValue=nextNode;
	  if(nextNode->right)
	    {
	      nextNode=nextNode->right;
	      fromWhere=FROM_PARENT;
	    }
	  else
	    {
	      nextNode=myTree->getParent(nextNode);
	      if(nextNode!=NULL)
		if(nextNode->left!=returnValue)
		  fromWhere=FROM_RIGHT;
	    }
	  return returnValue;
	  
	case FROM_RIGHT:
	  if(!myTree->getParent(nextNode)) 
	    {
	      nextNode=NULL;
	      return NULL;
	    }
	  if(myTree->getParent(nextNode)->left==nextNode)
	    fromWhere=FROM_LEFT;
	  nextNode=myTree->getParent(nextNode);
	  break;
	  
	case FROM_PARENT:
	  if(nextNode->left)
	    nextNode=nextNode->left;
	  else
	    fromWhere=FROM_LEFT;
	  break;
	  
	case FROM_NOWHERE:
	  return NULL;
	}
    }
}



template<class T>
BOOM::BaseBinTreeNode<T> *BOOM::BinaryTree<T>::BinTreeIter::postOrder()
{
  BOOM::BaseBinTreeNode<T> *returnValue;

  while(true) 
    {
      if(nextNode==NULL) return NULL;
      switch(fromWhere) 
	{
	case FROM_RIGHT:
	  returnValue=nextNode;
	  nextNode=myTree->getParent(nextNode);
	  if(nextNode!=NULL)
	    if(nextNode->left==returnValue)
	      fromWhere=FROM_LEFT;
	  return returnValue;
	  
	case FROM_LEFT:
	  if(nextNode->right)
	    {
	      nextNode=nextNode->right;
	      fromWhere=FROM_PARENT;
	    }
	  else
	    fromWhere=FROM_RIGHT;
	  break;
	  
	case FROM_PARENT:
	  if(nextNode->left)
	    nextNode=nextNode->left;
	  else
	    fromWhere=FROM_LEFT;
	  break;
	  
	case FROM_NOWHERE:
	  return NULL;
	}
    }
}



template<class T>
BOOM::BaseBinTreeNode<T> *BOOM::BinaryTree<T>::BinTreeIter::preOrder()
{
  BOOM::BaseBinTreeNode<T> *returnValue;

  while(true) 
    {
      if(nextNode==NULL) return NULL;
      switch(fromWhere) 
	{
	case FROM_PARENT:
	  returnValue=nextNode;
	  if(nextNode->left)
	    nextNode= nextNode->left;
	  else 
	    fromWhere=FROM_LEFT;
	  return returnValue;
	  
	case FROM_LEFT:
	  if(nextNode->right)
	    {
	      nextNode=nextNode->right;
	      fromWhere=FROM_PARENT;
	    }
	  else
	    fromWhere=FROM_RIGHT;
	  break;
	  
	case FROM_RIGHT:
	  returnValue=nextNode;
	  nextNode=myTree->getParent(nextNode);
	  if(nextNode!=NULL)
	    if(nextNode->left==returnValue)
	      fromWhere=FROM_LEFT;
	    else
	      fromWhere=FROM_RIGHT;
	  break;
	  
	case FROM_NOWHERE:
	  return NULL;
	}
    }
}



template<class T>
void BOOM::BinaryTree<T>::BinTreeIter::reset()
{
  nextNode=myTree->getRoot();
  fromWhere=FROM_PARENT;
}



template<class T>
T &BOOM::BinaryTree<T>::BinTreeIter::operator*()
{
  return thisNode->element;
}



template<class T>
typename BinaryTree<T>::BinTreeIter &
   BinaryTree<T>::BinTreeIter::operator++()
{
  switch(iterationType)
    {
    case PREORDER:   thisNode=preOrder();  break;
    case POSTORDER:  thisNode=postOrder(); break;
    case INORDER:    thisNode=inOrder();   break;
    }
  return *this;
}



template<class T>
BinaryTree<T>::BinTreeIter::operator bool()
{
  return thisNode!=NULL;
}



template<class T>
bool BinaryTree<T>::BinTreeIter::operator==(const 
					BinTreeIter &other)
  const
{
  return thisNode==other.thisNode;
}



#endif
