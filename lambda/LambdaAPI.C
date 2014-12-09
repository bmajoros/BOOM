/*
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/

#include <sstream>
#include <fstream>
#include "LambdaAPI.H"
#include "globals.H"
#include "LambdaException.H"
#include "AstPrinter.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;


// **************************************************
//               LambdaAPI methods
// **************************************************

LambdaAPI::LambdaAPI(unsigned seed)
  : evaluator(*new Evaluator(env)), randomSeed(seed),
    env(*new RunTimeEnvironment)
{
  // ctor

  env.registerMarkHook(&evaluator);
  global_evaluator=&evaluator;
  defineGlobals(env);

  //randomize();
  if(seed==0)
    randomSeed=GetRandomSeed();
  SeedRandomizer(randomSeed);
}



LambdaAPI::~LambdaAPI()
{
  delete &evaluator;
  //env.getGC().deleteEverything();
  delete &env;
}



void LambdaAPI::declareGlobal(const String &name)
{
  env.declareGlobal(name);
}



void LambdaAPI::defineGlobal(const String &name,LambdaObject *object)
{
  env.defineGlobal(name,object);
}



unsigned LambdaAPI::getRandomSeed() const
{
  return randomSeed;
}



LambdaObject *LambdaAPI::evaluate(AstNode *expression)
{
  return evaluator.evaluateTopLevel(expression);
}



void LambdaAPI::unregisterGarbage(Garbage *g)
{
  env.unregisterGarbage(g);
}



void LambdaAPI::registerWithGC(Garbage *g)
{
  env.registerGarbage(g);
}



void LambdaAPI::runGC()
{
  env.runGC();
}



void LambdaAPI::checkGarbageLevel()
{
  env.checkGarbageLevel();
}



AstCombination *LambdaAPI::makeCombination(AstNode *function,
     AstForest *args,bool regWithGC)
{
  // Re-package args into an array
  Array1D<AstNode*> *array=new Array1D<AstNode*>(args->getNumTrees());
  for(int i=0 ; i<args->getNumTrees() ; ++i)
    (*array)[i]=args->getIthTree(i);
  
  // Delete the forest
  //delete args;

  // Create combination object
  AstCombination *combo=new AstCombination(function,array);

  // Register with garbage-collector
  if(regWithGC)
    env.registerGarbage(combo);

  return combo;
}



AstCombination *LambdaAPI::makeComboWithArity(AstNode *function,
  int arity,bool regWithGC)
{
  Array1D<AstNode*> *array=new Array1D<AstNode*>(arity);
  int i;
  for(i=0 ; i<arity ; ++i)
    (*array)[i]=NULL;

  AstCombination *combo=new AstCombination(function,array);

  if(regWithGC)
    env.registerGarbage(combo);

  return combo;
}



void LambdaAPI::setArg(AstCombination *combo,int argNum,
  AstNode *arg)
{
  combo->getParms()[argNum]=arg;
  arg->setParent(combo);
}



AstLambda *LambdaAPI::makeLambda(int arity,AstForest *body,
     bool regWithGC)
{
  AstLambda *lambda=new AstLambda(arity,body);
  if(regWithGC)
    env.registerGarbage(lambda);
  return lambda;
}



AstForest *LambdaAPI::makeForest(bool regWithGC)
{
  AstForest *forest=new AstForest();
  if(regWithGC)
    env.registerGarbage(forest);
  return forest;
}



void LambdaAPI::appendTree(AstForest *forest,AstNode *tree)
{
  forest->appendTree(tree);
}



AstIntLit *LambdaAPI::makeIntLit(int value,bool regWithGC)
{
  AstIntLit *astInt=new AstIntLit(value);
  if(regWithGC)
    env.registerGarbage(astInt);
  return astInt;
}



AstFloatLit *LambdaAPI::makeFloatLit(float value,bool regWithGC)
{
  AstFloatLit *astFloat=new AstFloatLit(value);
  if(regWithGC)
    env.registerGarbage(astFloat);
  return astFloat;
}



AstCharLit *LambdaAPI::makeCharLit(char value,bool regWithGC)
{
  AstCharLit *astChar=new AstCharLit(value);
  if(regWithGC)
    env.registerGarbage(astChar);
  return astChar;
}



AstStringLit *LambdaAPI::makeStringLit(const String &value,
     bool regWithGC)
{
  AstStringLit *str=new AstStringLit(value);
  if(regWithGC)
    env.registerGarbage(str);
  return str;
}



LambdaInt *LambdaAPI::makeInt(int v,bool registerWithGC)
{
  LambdaInt *obj=new LambdaInt(v);
  if(registerWithGC)
    env.registerGarbage(obj);
  return obj;
}



LambdaFloat *LambdaAPI::makeFloat(float v,bool registerWithGC)
{
  LambdaFloat *obj=new LambdaFloat(v);
  if(registerWithGC)
    env.registerGarbage(obj);
  return obj;
}



LambdaChar *LambdaAPI::makeChar(char v,bool registerWithGC)
{
  LambdaChar *obj=new LambdaChar(v);
  if(registerWithGC)
    env.registerGarbage(obj);
  return obj;
}



LambdaString *LambdaAPI::makeString(const String &v,bool registerWithGC)
{
  LambdaString *obj=new LambdaString(v);
  if(registerWithGC)
    env.registerGarbage(obj);
  return obj;
}



AstSymbol *LambdaAPI::makeSymbolNode(const String &lexeme,
     bool regWithGC)
{
  Symbol *symbol=env.findOrCreateSymbol(lexeme);
  AstSymbol *astSymbol=new AstSymbol(symbol);
  if(regWithGC)
    env.registerGarbage(astSymbol);
  return astSymbol;
}



AstBoundVariable *LambdaAPI::makeBoundVariable(LexicalAddress la,
     bool regWithGC)
{
  AstBoundVariable *boundVar=new AstBoundVariable(la);
  if(regWithGC)
    env.registerGarbage(boundVar);
  return boundVar;
}



AST_NodeType LambdaAPI::getNodeType(AstNode *node)
{
  return node->getNodeType();
}



AstNode *LambdaAPI::getFunction(AstCombination *combo)
{
  return combo->getFunction();
}



AstNode *LambdaAPI::getArg(AstCombination *combo,int argNum)
{
  return combo->getParms()[argNum];
}



int LambdaAPI::numArgs(AstCombination *combo)
{
  return combo->getParms().size();
}



int LambdaAPI::arity(AstLambda *lambda)
{
  return lambda->getArity();
}



AstForest *LambdaAPI::getBody(AstLambda *lambda)
{
  return lambda->getBody();
}



int LambdaAPI::numTrees(AstForest *forest)
{
  return forest->getNumTrees();
}



AstNode *LambdaAPI::getTree(AstForest *forest,int treeNum)
{
  return forest->getIthTree(treeNum);
}



AstNode *LambdaAPI::changeArg(AstCombination *combo,int argNum,
     AstNode *newArg)
{
  //returns the old arg

  AstNode *oldArg=combo->getParms()[argNum];
  combo->getParms()[argNum]=newArg;
  newArg->setParent(combo);
  return oldArg;
}



AstNode *LambdaAPI::changeFunction(AstCombination *combo,
  AstNode *newFunction)
{
  AstNode *oldFunction=combo->getFunction();

  combo->changeFunction(newFunction);

  return oldFunction;
}



AstNode *LambdaAPI::snipArg(AstCombination *combo,int argNum)
{
  // Sets the slot to nil; does not delete the slot

  return changeArg(combo,argNum,NULL);
}



AstForest *LambdaAPI::changeBody(AstLambda *lambda,AstForest *newBody)
{
  AstForest *oldBody=lambda->getBody();
  lambda->changeBody(newBody);
  return oldBody;
}



AstNode *LambdaAPI::changeTree(AstForest *forest,int treeNum,AstNode *newTree)
{
  AstNode *oldTree=forest->getIthTree(treeNum);
  forest->changeTree(treeNum,newTree);
  return oldTree;
}



AstNode *LambdaAPI::snipTree(AstForest *forest,int treeNum,
   bool regWithGC/*=false*/)
{
  AstNode *victim=forest->getIthTree(treeNum);
  forest->deleteTree(treeNum);
  if(regWithGC)
    env.registerGarbage(victim);
  return victim;
}



void LambdaAPI::insertTree(AstForest *forest,int treeNum,AstNode *newTree)
{
  forest->insertTree(treeNum,newTree);
}



LexicalAddress LambdaAPI::getAddress(const String &lexeme)
{
  int lexicalDepth;
  LambdaSymTabNode *symTabNode=env.Find(lexeme,lexicalDepth);
  if(!symTabNode) 
    throw LambdaException("lexeme not found in LambdaAPI::getAddress()");
  return LexicalAddress(lexicalDepth,symTabNode->getLexicalPosition());
}



LambdaObject *LambdaAPI::lookupGlobal(const String &lexeme)
{
  int lexicalDepth;
  LambdaSymTabNode *symTabNode=env.Find(lexeme,lexicalDepth);
  if(!symTabNode) return NULL;
  return env.retrieveGlobal(symTabNode->getLexicalPosition());
}



AstBoundVariable *LambdaAPI::makeGlobalRef(const String 
  &globalIdent,int lexicalDepth,bool regWithGC)
{
  int dummy;
  LambdaSymTabNode *symTabNode=env.Find(globalIdent,dummy);
  if(!symTabNode) 
    {
      ostringstream os;
      os << "Can't find \"" << globalIdent << 
	"\" in LambdaAPI::makeGlobalRef()";
      throw LambdaException(os.str());
    }
  
  AstBoundVariable *boundVar=
    new AstBoundVariable(LexicalAddress(lexicalDepth,
					symTabNode->getLexicalPosition()));

  if(regWithGC)
    env.registerGarbage(boundVar);
  return boundVar;
}



RunTimeEnvironment &LambdaAPI::getEnvironment()
{
  return env;
}



Evaluator &LambdaAPI::getEvaluator()
{
  return evaluator;
}



void LambdaAPI::assignToGlobal(const String &name,LambdaObject *value)
{
  env.set(getAddress(name),value);
}



AstForest *LambdaAPI::parseFile(const String &filename,bool regWithGC)
{
  ifstream is(filename.c_str());
  if(!is.good()) throw String("Cannot open file: ")+filename;
  Parser parser(is,env,regWithGC);
  return parser.parse();
}



AstForest *LambdaAPI::parseSource(const String &sourceCode,bool regWithGC)
{
  istringstream is(sourceCode);
  Parser parser(is,env,regWithGC);
  return parser.parse();
}


//LambdaObject *ffRandInt(RunTimeEnvironment &env,ContinuationType &)

ForeignFunctionObject *LambdaAPI::defineFunction(ForeignFunction ff,
						 const String &name,
						 int arity)
{
  ForeignFunctionObject *ffo=new ForeignFunctionObject(ff,arity);
  env.declareGlobal(name);
  env.defineGlobal(name,ffo);
  return ffo;
}



bool LambdaAPI::isNumeric(LambdaObject *obj)
{
  LambdaObjectType t=obj->getType();
  return t==OBTYPE_INT || t==OBTYPE_FLOAT;
}



float LambdaAPI::asFloat(LambdaObject *obj)
{
  switch(obj->getType())
    {
    case OBTYPE_INT:
      return float(static_cast<LambdaInt*>(obj)->getValue());
    case OBTYPE_FLOAT:
      return static_cast<LambdaFloat*>(obj)->getValue();
    }
  throw "LambdaAPI::asFloat() requires an int or float object";
}



void LambdaAPI::makeImmortal(Garbage *obj)
{
  if(obj) env.makeImmortal(obj);
}



GarbageCollector &LambdaAPI::getGC()
{
  return env.getGC();
}



void LambdaAPI::print(AstNode *node,ostream &os)
{
  AstPrinter printer(os,env);
  printer.print(node);
}



void LambdaAPI::setGCthreshold(int t)
{
  env.setGCthreshold(t);
}


