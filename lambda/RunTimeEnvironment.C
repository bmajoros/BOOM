/*
 RunTimeEnvironment.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "RunTimeEnvironment.H"
#include "LambdaException.H"
#include "ConsCell.H"
#include <iostream>
#include <time.h>
#include "globals.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;


// **************************************************
//             LambdaSymTabNode methods
// **************************************************

LambdaSymTabNode::LambdaSymTabNode(const String &lexeme,
				 int lexicalPosition)
  : lexeme(lexeme), lexicalPosition(lexicalPosition)
{
  // ctor
}



const String &LambdaSymTabNode::getLexeme() const
{
  return lexeme;
}



bool LambdaSymTabNode::isResolved()
{
  return true;
}



bool LambdaSymTabNode::Equals(const char *Key)
{
  return !strcmp(Key,lexeme.c_str());
}



int LambdaSymTabNode::getLexicalPosition() const
{
  return lexicalPosition;
}


// **************************************************
//              BackpatchNode methods
// **************************************************

BackpatchNode::BackpatchNode(const String &lexeme,
			     int lexicalPosition)
  : LambdaSymTabNode(lexeme,lexicalPosition)
{
  // ctor
}



void BackpatchNode::addToList(AstVariable *var)
{
  patchList.appendTree(var);
}



AstForest &BackpatchNode::getPatchList()
{
  return patchList;
}



bool BackpatchNode::isResolved()
{
  return false;
}



// **************************************************
//             RunTimeEnvironment methods
// **************************************************

RunTimeEnvironment::RunTimeEnvironment()
  : globalNames(*new ElasVector<String*>(NULL)), 
    scopeStack(*new ScopeStack(41)),
    symbolRepository(*new SymbolRepository),
    runTimeStack(*new RunTimeStack),
    gc(*new GarbageCollector),
    immortals(*new Immortals)
{
  // Enter the global scope
  scopeStack.EnterScope();

  gc.registerMarkHook(&immortals);

  clock();
}



RunTimeEnvironment::~RunTimeEnvironment()
{
  //gc.deleteEverything();
  cout<<"delete symbolRepository"<<endl;
  delete &symbolRepository;
  cout<<"delete scopeStack"<<endl;
  delete &scopeStack;
  cout<<"delete runTimeStack"<<endl;
  delete &runTimeStack;
  cout<<"delete gc"<<endl;
  delete &gc;
  cout<<"delete globalNames"<<endl;
  delete &globalNames;
  cout<<"delete immortals"<<endl;
  delete &immortals;
  cout<<"leaving RTE dtor"<<endl;
}



ActivationRecord *RunTimeEnvironment::getTopAR()
{
  return runTimeStack.peekTop();
}



ActivationRecord *RunTimeEnvironment::getGlobalAR()
{
  return runTimeStack.getGlobalAR();
}



void RunTimeEnvironment::declareGlobal(const String &name)
{
  const int lexicalPosition=scopeStack.getGlobalScopeSize();
  globalNames[lexicalPosition]=new String(name);

  LambdaSymTabNode *node=new LambdaSymTabNode(name,lexicalPosition);
  scopeStack.insertGlobal(node,name.c_str());
}



const String &RunTimeEnvironment::getGlobalName(
  int lexicalPosition)
{
  return (String &) *globalNames[lexicalPosition];
}



void RunTimeEnvironment::declareGlobal(BackpatchNode *
				       backpatchNode,
				       const String &key)
{
  int lexicalPosition=scopeStack.getGlobalScopeSize();
  globalNames[lexicalPosition]=new String(key);
  scopeStack.insertGlobal(backpatchNode,key.c_str());
}



void RunTimeEnvironment::defineGlobal(const String &name,
				      LambdaObject *object)
{
  LambdaSymTabNode *node=
    (LambdaSymTabNode *) scopeStack.findGlobal(name.c_str());
  if(!node)
    throw LambdaException("Defining undeclared global "
			  "in RunTimeEnvironment");

  const int lexicalPosition=node->getLexicalPosition();
  ActivationRecord *ar=runTimeStack.getGlobalAR();
  ar->setEntry(lexicalPosition,object);
}



void RunTimeEnvironment::setGCthreshold(int t)
{
  gc.setThreshold(t);
}



void RunTimeEnvironment::pushGlobalAR()
{
  runTimeStack.pushAR(new ExpandableAR());
}



LambdaObject *RunTimeEnvironment::retrieve(LexicalAddress la)
{
  return runTimeStack.retrieve(la);
}



LambdaObject *RunTimeEnvironment::retrieveGlobal(int lexicalPosition)
{
  return runTimeStack.getGlobalAR()->getEntry(lexicalPosition);
}



void RunTimeEnvironment::set(LexicalAddress la,LambdaObject *object)
{
  runTimeStack.set(la,object);
}



LambdaObject *RunTimeEnvironment::getParm(int pos)
{
  return runTimeStack.retrieve(LexicalAddress(0,pos));
}



Symbol *RunTimeEnvironment::findOrCreateSymbol(const String &s)
{
  return symbolRepository.findOrCreate(s);
}



LambdaSymTabNode *RunTimeEnvironment::Find(const String &key,
					  int &lexicalDepth)
{
  return (LambdaSymTabNode*)
    scopeStack.Find(key.c_str(),lexicalDepth);
}



int RunTimeEnvironment::GetCurrentScopeSize()
{
  return scopeStack.GetCurrentScopeSize();
}



void RunTimeEnvironment::Declare(LambdaSymTabNode *node,
				 const String &key)
{
  scopeStack.Insert(node,key.c_str());
}



void RunTimeEnvironment::enterScope()
{
  scopeStack.EnterScope();
}



void RunTimeEnvironment::leaveScope()
{
  scopeStack.LeaveScope();
}



void RunTimeEnvironment::pushAR(ActivationRecord *ar)
{
  runTimeStack.pushAR(ar);
}



void RunTimeEnvironment::popAR()
{
  // This method pops the top AR and gives it to the
  // garbage collector

  runTimeStack.popAR(gc);
}



void RunTimeEnvironment::popAR_unlessGlobal()
{
  if(!runTimeStack.topARisGlobal())
    runTimeStack.popAR(gc);
}



void RunTimeEnvironment::registerGarbage(Garbage *garbage)
{
  gc.registerObject(garbage);
}



int RunTimeEnvironment::getStackDepth()
{
  // ### for debugging

  int d=0;
  runTimeStack.initIterator();
  while(runTimeStack.canIterate())
    {
      ++d;
      runTimeStack.iterate();
    }

  return d;
}



void RunTimeEnvironment::popDownTo(ActivationRecord *ar)
{
  while(runTimeStack.peekTop()!=ar)
    runTimeStack.popAR(gc);
}



void RunTimeEnvironment::cleanStack()
{
  while(!runTimeStack.topARisGlobal())
    runTimeStack.popAR(gc);  
}



ActivationRecord *RunTimeEnvironment::prepareAR(
   Array1D<LambdaObject*> &parmValues,int arity,
   ActivationRecord *staticChain)
{
  int numParms=parmValues.size();

  // Arity<0 means "varadic" => use expandable AR
  if(arity<0) {
    ActivationRecord *ar=new ExpandableAR(); // ### why no static chain?
    registerGarbage(ar);
    for(int i=0 ; i<numParms ; ++i) (*ar)[i]=parmValues[i];
    return ar;
  }

  // Allocate an activation record
  ActivationRecord *ar=new ActivationRecord(arity,staticChain);
  registerGarbage(ar);

  if(numParms<arity) {
    for(int i=0 ; i<numParms ; ++i)
      ar->setEntry(i,parmValues[i]);
    // the rest are initialized to nil in the ctor
    return ar;
  }

  if(arity>0)
    {
      // Store the first arity-1 values in the activation record
      for(int i=0 ; i<arity-1 ; ++i)
	ar->setEntry(i,(LambdaObject*)parmValues[i]);
      
      // If more than one value remains, package them into a list
      // and store it as the last parameter
      if(numParms>arity)
	ar->setEntry(arity-1,prepareList(parmValues,arity-1));
      else
	ar->setEntry(arity-1,(LambdaObject*)parmValues[arity-1]);
    }

  return ar;
}



ConsCell *RunTimeEnvironment::prepareList(
   Array1D<LambdaObject*> &parmValues,int startingAt)
{
  ConsCell *list=NULL;

  int i;
  for(i=parmValues.size()-1 ; i>=startingAt ; --i)
    {
      list=new ConsCell((LambdaObject*)parmValues[i],list);
      registerGarbage(list);
    }

  return list;
}



void RunTimeEnvironment::resolveGlobal(
   BackpatchNode *backpatchNode,
   LambdaSymTabNode *symTabNode)
{
  scopeStack.replaceGlobal(backpatchNode,symTabNode,
			   symTabNode->getLexeme().c_str());
}



int RunTimeEnvironment::getGlobalScopeSize()
{
  return scopeStack.getGlobalScopeSize();
}



void RunTimeEnvironment::runGC()
{
  if((!global_commandLine || !global_commandLine->isGCquiet())
     && !gc.isSilent())
    {
      cerr << "Running GC...";
      cerr.flush();
    }

  int before=gc.numLiveObjects();

  clock_t startTime=clock();
  gc.collect(runTimeStack);
  clock_t endTime=clock();

  int after=gc.numLiveObjects();
  int collected=before-after;

  float seconds=float(endTime-startTime)/float(CLOCKS_PER_SEC);

  if((!global_commandLine || !global_commandLine->isGCquiet())
     && !gc.isSilent())
    {
      cerr << "done.  " << collected << " objects collected, " 
	   << seconds << " sec" << endl;
    }
}



void RunTimeEnvironment::unregisterGarbage(Garbage *g)
{
  gc.unregisterObject(g);
}



void RunTimeEnvironment::registerMarkHook(MarkHook *hook)
{
  gc.registerMarkHook(hook);
}



int RunTimeEnvironment::getNumParms()
{
  ActivationRecord *ar=getTopAR();
  return ar->getNumEntries();
}



void RunTimeEnvironment::backpatch(LambdaSymTabNode *symTabNode)
{
  ::backpatch(symTabNode,*this);
}



void RunTimeEnvironment::declareGlobalAndBackpatch(const String &name)
{
  int lexDepth;
  LambdaSymTabNode *symTabNode=Find(name,lexDepth);
  if(!symTabNode) declareGlobal(name);
  defineGlobal(name,NULL);
  if(symTabNode && !symTabNode->isResolved()) backpatch(symTabNode);
}



void RunTimeEnvironment::assign(const String &variableName,
				LambdaObject *value)
{
  throw "RunTimeEnvironment::assign() -- under construction";

  int lexDepth;
  LambdaSymTabNode *symTabNode=Find(variableName,lexDepth);
  if(!symTabNode) throw LambdaException(variableName+" undefined");
  LexicalAddress addr(lexDepth,symTabNode->getLexicalPosition());
  set(addr,value); 
}


void RunTimeEnvironment::makeImmortal(Garbage *g)
{
  immortals.add(g);
}



GarbageCollector &RunTimeEnvironment::getGC()
{
  return gc;
}


