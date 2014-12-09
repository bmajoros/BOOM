/*
 globals.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "globals.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;

#define STATIC_CAST( T , E ) static_cast<T>(E)

namespace Lambda {

LambdaObject *true_object=NULL;
Evaluator *global_evaluator=NULL;
LambdaCmdLine *global_commandLine=NULL;



LambdaObject *ffCons(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *car=env.getParm(0);
  LambdaObject *cdr=env.getParm(1);

  LambdaObject *consCell=new ConsCell(car,cdr);
  env.registerGarbage(consCell);

  return consCell;
}



LambdaObject *ffCar(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *parm=env.getParm(0);
  if(!parm || parm->getType()!=OBTYPE_CONS_CELL)
    throw LambdaException("Wrong parm type in call to car");
  ConsCell *consCell=STATIC_CAST(ConsCell*,parm);

  return consCell->getCar();
}



LambdaObject *ffCdr(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *parm=env.getParm(0);
  if(!parm || parm->getType()!=OBTYPE_CONS_CELL)
    throw LambdaException("Wrong parm type in call to cdr");
  ConsCell *consCell=STATIC_CAST(ConsCell*,parm);

  return consCell->getCdr();
}



LambdaObject *ffPrint(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *parm=env.getParm(0);
  if(!parm) cout << "nil";
  else switch(parm->getType())
    {
    case OBTYPE_STRING:
      {
	LambdaString *string=STATIC_CAST(LambdaString*,parm);
	cout << string->getValue();
      }
      break;
    default:
      {
	ObjectPrinter printer(env);
	printer.print(parm,cout);
      }
      break;
    }

  cout.flush();
  return NULL;
}



LambdaObject *ffIf(RunTimeEnvironment &env,ContinuationType &ct)
{
  // This function implements the "if" construct

  // Get the parameters to this function
  LambdaObject *boolPart=env.getParm(0);
  LambdaObject *thenPart=env.getParm(1);
  LambdaObject *elsePart=env.getParm(2);

  if(!thenPart || 
     (thenPart->getType()!=OBTYPE_CLOSURE &&
      thenPart->getType()!=OBTYPE_FOREIGN_FUNCTION))
     throw LambdaException("'then' parameter to 'if' must be a function");

  if(!elsePart || 
     (elsePart->getType()!=OBTYPE_CLOSURE &&
      elsePart->getType()!=OBTYPE_FOREIGN_FUNCTION))
     throw LambdaException("'else' parameter to 'if' must be a function");

  // Instruct the evaluator to make a tail-recursive call
  // to my return value
  ct=CONT_TAIL_REC_CALL;

  // Decide which is the return value, the 'then' or 'else' part
  bool realBool=false;
  if(boolPart) {
    switch(boolPart->getType()) {
    case OBTYPE_INT: 
      if(static_cast<LambdaInt*>(boolPart)->getValue()!=0) realBool=true;
      break;
    case OBTYPE_CHAR:
      if(static_cast<LambdaChar*>(boolPart)->getValue()!='\0') realBool=true;
      break;
    case OBTYPE_FLOAT:
      if(static_cast<LambdaString*>(boolPart)->getValue()!="") realBool=true;
      break;
    case OBTYPE_STRING:
    case OBTYPE_CLOSURE:
    case OBTYPE_FOREIGN_FUNCTION:
    case OBTYPE_CONS_CELL:
    case OBTYPE_SYMBOL:
    case OBTYPE_FOREIGN_TYPE:
    case OBTYPE_VECTOR:
      realBool=true;
    }
  }
  LambdaObject *result=(realBool ? thenPart : elsePart);
  Closure *closure=STATIC_CAST(Closure*,result);

  // Push an AR for the function to be called
  Array1D<LambdaObject*> parms;
  ActivationRecord *ar=
    env.prepareAR(parms,0,closure->getStaticChain());
  env.pushAR(ar);
  
  // Return the function to be called tail-recursively
  return realBool ? thenPart : elsePart;
}



LambdaFloat *toLambdaFloat(LambdaObject *intOrFloat,RunTimeEnvironment &env)
{
  // Precondition: intOrFloat must be a LambdaInt or a LambdaFloat

  if(!intOrFloat)
    {
      LambdaFloat *f=new LambdaFloat(0.0);
      env.registerGarbage(f);
      return f;
    }
  switch(intOrFloat->getType())
    {
    case OBTYPE_INT:
      {
	LambdaInt *lambdaInt=STATIC_CAST(LambdaInt*,intOrFloat);
	LambdaFloat *f=new LambdaFloat((double)lambdaInt->getValue());
	env.registerGarbage(f);
	return f;
      }
    case OBTYPE_FLOAT:
      return STATIC_CAST(LambdaFloat*,intOrFloat);
    default:
      {
	LambdaFloat *f=new LambdaFloat(0.0);
	env.registerGarbage(f);
	return f;
      }
      cout<<"toLambdaFloat()"<<endl;
      throw LambdaException("Function requires int or double argument");
    }
}


LambdaObject *ffPlus(RunTimeEnvironment &env,ContinuationType &ct)
{
  LambdaObject *lhs=env.getParm(0);
  LambdaObject *rhs=env.getParm(1);

  if(!lhs || !rhs)
    throw LambdaException("nil parameter passed to + operator");

  bool integerArithmetic=
    (lhs->getType()==OBTYPE_INT && rhs->getType()==OBTYPE_INT);

  if(integerArithmetic)
    {
      LambdaInt *intLhs=STATIC_CAST(LambdaInt*,lhs);
      LambdaInt *intRhs=STATIC_CAST(LambdaInt*,rhs);
      int lhsValue=intLhs->getValue();
      int rhsValue=intRhs->getValue();
      int value=lhsValue+rhsValue;
      LambdaObject *result=new LambdaInt(value);
      env.registerGarbage(result);
      return result;
    }

  LambdaFloat *doubleLhs=toLambdaFloat(lhs,env);
  LambdaFloat *doubleRhs=toLambdaFloat(rhs,env);

  double lhsValue=doubleLhs->getValue();
  double rhsValue=doubleRhs->getValue();
  double value=lhsValue+rhsValue;
  LambdaObject *result=new LambdaFloat(value);
  env.registerGarbage(result);
  return result;
}



LambdaObject *ffMinus(RunTimeEnvironment &env,ContinuationType &ct)
{
  LambdaObject *lhs=env.getParm(0);
  LambdaObject *rhs=env.getParm(1);

  if(!lhs || !rhs)
    throw LambdaException("nil parameter passed to - operator");

  bool integerArithmetic=
    (lhs->getType()==OBTYPE_INT && rhs->getType()==OBTYPE_INT);

  if(integerArithmetic)
    {
      LambdaInt *intLhs=STATIC_CAST(LambdaInt*,lhs);
      LambdaInt *intRhs=STATIC_CAST(LambdaInt*,rhs);
      int lhsValue=intLhs->getValue();
      int rhsValue=intRhs->getValue();
      int value=lhsValue-rhsValue;
      LambdaObject *result=new LambdaInt(value);
      env.registerGarbage(result);
      return result;
    }

  LambdaFloat *doubleLhs=toLambdaFloat(lhs,env);
  LambdaFloat *doubleRhs=toLambdaFloat(rhs,env);

  double lhsValue=doubleLhs->getValue();
  double rhsValue=doubleRhs->getValue();
  double value=lhsValue-rhsValue;
  LambdaObject *result=new LambdaFloat(value);
  env.registerGarbage(result);
  return result;
}



LambdaObject *ffTimes(RunTimeEnvironment &env,ContinuationType &ct)
{
  LambdaObject *lhs=env.getParm(0);
  LambdaObject *rhs=env.getParm(1);

  if(!lhs || !rhs)
    throw LambdaException("nil parameter passed to * operator");

  bool integerArithmetic=
    (lhs->getType()==OBTYPE_INT && rhs->getType()==OBTYPE_INT);

  if(integerArithmetic)
    {
      LambdaInt *intLhs=STATIC_CAST(LambdaInt*,lhs);
      LambdaInt *intRhs=STATIC_CAST(LambdaInt*,rhs);
      int lhsValue=intLhs->getValue();
      int rhsValue=intRhs->getValue();
      int value=lhsValue*rhsValue;
      LambdaObject *result=new LambdaInt(value);
      env.registerGarbage(result);
      return result;
    }

  LambdaFloat *doubleLhs=toLambdaFloat(lhs,env);
  LambdaFloat *doubleRhs=toLambdaFloat(rhs,env);

  double lhsValue=doubleLhs->getValue();
  double rhsValue=doubleRhs->getValue();
  double value=lhsValue*rhsValue;
  LambdaObject *result=new LambdaFloat(value);
  env.registerGarbage(result);
  return result;
}



LambdaObject *ffDivide(RunTimeEnvironment &env,ContinuationType &ct)
{
  LambdaObject *lhs=env.getParm(0);
  LambdaObject *rhs=env.getParm(1);

  if(!lhs || !rhs)
    throw LambdaException("nil parameter passed to / operator");

  bool integerArithmetic=
    (lhs->getType()==OBTYPE_INT && rhs->getType()==OBTYPE_INT);

  if(integerArithmetic)
    {
      LambdaInt *intLhs=STATIC_CAST(LambdaInt*,lhs);
      LambdaInt *intRhs=STATIC_CAST(LambdaInt*,rhs);
      int lhsValue=intLhs->getValue();
      int rhsValue=intRhs->getValue();
      if(rhsValue==0)
	throw LambdaException("Division by zero in /");
      int value=lhsValue/rhsValue;
      LambdaObject *result=new LambdaInt(value);
      env.registerGarbage(result);
      return result;
    }

  LambdaFloat *doubleLhs=toLambdaFloat(lhs,env);
  LambdaFloat *doubleRhs=toLambdaFloat(rhs,env);

  double lhsValue=doubleLhs->getValue();
  double rhsValue=doubleRhs->getValue();
  if(rhsValue==0.0)
    throw LambdaException("Division by zero in /");
  double value=lhsValue/rhsValue;
  LambdaObject *result=new LambdaFloat(value);
  env.registerGarbage(result);
  return result;
}



LambdaObject *ffProtectedDiv(RunTimeEnvironment &env,
			     ContinuationType &ct)
{
  // (protected-div 1 0)

  LambdaObject *lhs=env.getParm(0);
  LambdaObject *rhs=env.getParm(1);

  if(!lhs || !rhs)
    throw LambdaException("nil parameter passed to / operator");

  bool integerArithmetic=
    (lhs->getType()==OBTYPE_INT && rhs->getType()==OBTYPE_INT);

  if(integerArithmetic)
    {
      LambdaInt *intLhs=STATIC_CAST(LambdaInt*,lhs);
      LambdaInt *intRhs=STATIC_CAST(LambdaInt*,rhs);
      int lhsValue=intLhs->getValue();
      int rhsValue=intRhs->getValue();
      if(rhsValue==0)
	rhsValue=1;
      int value=lhsValue/rhsValue;
      LambdaObject *result=new LambdaInt(value);
      env.registerGarbage(result);
      return result;
    }

  LambdaFloat *doubleLhs=toLambdaFloat(lhs,env);
  LambdaFloat *doubleRhs=toLambdaFloat(rhs,env);

  double lhsValue=doubleLhs->getValue();
  double rhsValue=doubleRhs->getValue();
  if(rhsValue==0.0)
    rhsValue=1.0;
  double value=lhsValue/rhsValue;
  LambdaObject *result=new LambdaFloat(value);
  env.registerGarbage(result);
  return result;
}



LambdaObject *ffModulus(RunTimeEnvironment &env,ContinuationType &ct)
{
  LambdaObject *lhs=env.getParm(0);
  LambdaObject *rhs=env.getParm(1);

  if(!lhs || !rhs)
    throw LambdaException("nil parameter passed to % operator");

  bool integerArithmetic=
    (lhs->getType()==OBTYPE_INT && rhs->getType()==OBTYPE_INT);

  if(integerArithmetic)
    {
      LambdaInt *intLhs=STATIC_CAST(LambdaInt*,lhs);
      LambdaInt *intRhs=STATIC_CAST(LambdaInt*,rhs);
      int lhsValue=intLhs->getValue();
      int rhsValue=intRhs->getValue();
      if(rhsValue==0)
	throw LambdaException("Division by zero in %");
      int value=lhsValue%rhsValue;
      LambdaObject *result=new LambdaInt(value);
      env.registerGarbage(result);
      return result;
    }

  throw LambdaException("double % double is illegal");
}



LambdaObject *ffLess(RunTimeEnvironment &env,ContinuationType &ct)
{
  LambdaObject *lhs=env.getParm(0);
  LambdaObject *rhs=env.getParm(1);

  if(!lhs || !rhs)
    throw LambdaException("nil parameter passed to < operator");

  LambdaFloat *doubleLhs=toLambdaFloat(lhs,env);
  LambdaFloat *doubleRhs=toLambdaFloat(rhs,env);

  double lhsValue=doubleLhs->getValue();
  double rhsValue=doubleRhs->getValue();

  LambdaObject *retval=
    lhsValue<rhsValue ? 
    new LambdaInt(1) : new LambdaInt(0);
  env.registerGarbage(retval);
  return retval;  
  //return lhsValue<rhsValue ? true_object : NULL;
}



LambdaObject *ffLessEqual(RunTimeEnvironment &env,ContinuationType &ct)
{
  LambdaObject *lhs=env.getParm(0);
  LambdaObject *rhs=env.getParm(1);

  if(!lhs || !rhs)
    throw LambdaException("nil parameter passed to <= operator");

  LambdaFloat *doubleLhs=toLambdaFloat(lhs,env);
  LambdaFloat *doubleRhs=toLambdaFloat(rhs,env);

  double lhsValue=doubleLhs->getValue();
  double rhsValue=doubleRhs->getValue();

  LambdaObject *retval=
    lhsValue<=rhsValue ? 
    new LambdaInt(1) : new LambdaInt(0);
  env.registerGarbage(retval);
  return retval;
  //return lhsValue<=rhsValue ? true_object : NULL;
}



LambdaObject *ffGreater(RunTimeEnvironment &env,ContinuationType &ct)
{
  LambdaObject *lhs=env.getParm(0);
  LambdaObject *rhs=env.getParm(1);

  if(!lhs || !rhs)
    throw LambdaException("nil parameter passed to > operator");

  LambdaFloat *doubleLhs=toLambdaFloat(lhs,env);
  LambdaFloat *doubleRhs=toLambdaFloat(rhs,env);

  double lhsValue=doubleLhs->getValue();
  double rhsValue=doubleRhs->getValue();

  LambdaObject *retval=
    lhsValue>rhsValue ? 
    new LambdaInt(1) : new LambdaInt(0);
  env.registerGarbage(retval);
  return retval;
  //return lhsValue>rhsValue ? true_object : NULL;
}



LambdaObject *ffGreaterEqual(RunTimeEnvironment &env,ContinuationType &ct)
{
  LambdaObject *lhs=env.getParm(0);
  LambdaObject *rhs=env.getParm(1);

  if(!lhs || !rhs)
    throw LambdaException("nil parameter passed to >= operator");

  LambdaFloat *doubleLhs=toLambdaFloat(lhs,env);
  LambdaFloat *doubleRhs=toLambdaFloat(rhs,env);

  double lhsValue=doubleLhs->getValue();
  double rhsValue=doubleRhs->getValue();

  LambdaObject *retval=
    lhsValue>=rhsValue ? 
    new LambdaInt(1) : new LambdaInt(0);
  env.registerGarbage(retval);
  return retval;
  //return lhsValue>=rhsValue ? true_object : NULL;
}



LambdaObject *ffEqual(RunTimeEnvironment &env,ContinuationType &ct)
{
  LambdaObject *lhs=env.getParm(0);
  LambdaObject *rhs=env.getParm(1);

  if(!lhs || !rhs)
    throw LambdaException("nil parameter passed to ==");

  LambdaFloat *doubleLhs=toLambdaFloat(lhs,env);
  LambdaFloat *doubleRhs=toLambdaFloat(rhs,env);

  double lhsValue=doubleLhs->getValue();
  double rhsValue=doubleRhs->getValue();

  LambdaObject *retval=
    lhsValue==rhsValue ? 
    new LambdaInt(1) : new LambdaInt(0);
  env.registerGarbage(retval);
  return retval;
  //return lhsValue==rhsValue ? true_object : NULL;
}



LambdaObject *ffNotEqual(RunTimeEnvironment &env,ContinuationType &ct)
{
  LambdaObject *lhs=env.getParm(0);
  LambdaObject *rhs=env.getParm(1);

  if(!lhs || !rhs)
    throw LambdaException("nil parameter passed to !=");

  LambdaFloat *doubleLhs=toLambdaFloat(lhs,env);
  LambdaFloat *doubleRhs=toLambdaFloat(rhs,env);

  double lhsValue=doubleLhs->getValue();
  double rhsValue=doubleRhs->getValue();

  LambdaObject *retval=
    lhsValue!=rhsValue ? 
    new LambdaInt(1) : new LambdaInt(0);
  env.registerGarbage(retval);
  return retval;
  //return lhsValue!=rhsValue ? true_object : NULL;
}



void backpatch(LambdaSymTabNode *symTabNode,RunTimeEnvironment &env)
{
  // Get list of AST nodes to backpatch
  BackpatchNode *backpatchNode=
    STATIC_CAST(BackpatchNode *,symTabNode);
  AstForest &patchList=backpatchNode->getPatchList();
  
  // Iterate through patchList
  const int numNodes=patchList.getNumTrees();
  for(int i=0 ; i<numNodes ; ++i)
    {
      // Get the next node to patch
      AstFreeVariable *freeVar=
	STATIC_CAST(AstFreeVariable *,patchList.getIthTree(i));
      
      // Compute lexical address of bound variable
      LexicalAddress la(freeVar->getLexicalDepth(),
			backpatchNode->getLexicalPosition());
      
      // Create a bound variable to replace the free variable
      AstBoundVariable *boundVar=new AstBoundVariable(la);
      
      // Ask this patch-node's parent to patch it
      freeVar->getParent()->bindVar(freeVar,boundVar);
      
      // We don't need the free variable any more, but the GC will
      // delete it for us, so there's nothing to do here.
    }
  
  // Replace the free entry in the scope stack with a bound entry
  LambdaSymTabNode *newSymTabNode=
    new LambdaSymTabNode(symTabNode->getLexeme(),
			symTabNode->getLexicalPosition());
  env.resolveGlobal(backpatchNode,newSymTabNode);
}



LambdaObject *ffDefine(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *parm1=env.getParm(0);
  LambdaObject *parm2=env.getParm(1);

  if(!parm1 || parm1->getType()!=OBTYPE_SYMBOL)
    throw LambdaException("define requires a symbol for 1st parameter");

  Symbol *symbol=STATIC_CAST(Symbol*,parm1);
  //bool debug=(symbol->getLexeme()=="spacer");

  // Declare the global, if it is not already declared
  int lexDepth;
  LambdaSymTabNode *symTabNode=env.Find(symbol->getLexeme(),lexDepth);
  //if(debug) cout<<"symTabNode="<<symTabNode<<endl;
  if(!symTabNode) {
    //if(debug) cout<<"declaring global "<<endl;
    env.declareGlobal(symbol->getLexeme());
  }
  //else if(debug) cout<<"not declaring global"<<endl;

  // Store the new value for that global
  env.defineGlobal(symbol->getLexeme(),parm2);

  // Perform backpatching if this global was previously
  // unresolved.  Recall that a name can be declared
  // or not declared, and if it is declared, it can
  // be resolved or not resolved.  This is because
  // unknown identifiers are entered into the symbol
  // table as 'unresolved' when they are referenced
  // in expressions.  This allows forward referencing
  // and recursion.
  if(symTabNode && !symTabNode->isResolved()) {
    //if(debug) cout<<"backpatching"<<endl;
    backpatch(symTabNode,env);
  }
  //else if(debug) cout<<"not backpatching"<<endl;

  return parm1;
}



void LoadSource(const String &filename,RunTimeEnvironment &env)
{
  ifstream is(filename.c_str());
  if(!is.good()) {
    cout<<"cannot open file "<<filename<<endl;
    throw LambdaException(String("Cannot open file: ")+filename);
  }

  Parser parser(is,env,false);
  AstForest *forest=parser.parse();
  is.close();

  global_evaluator->evaluateTopLevel(forest);
}



LambdaObject *ffLoad(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *parm=env.getParm(0);
  if(!parm || parm->getType()!=OBTYPE_STRING)
    throw LambdaException("'load' expecting string argument");

  ActivationRecord *ar=env.getTopAR();
  env.popAR();//otherwise, static chains will be set up wrong

  LambdaString *filename=STATIC_CAST(LambdaString*,parm);

  LoadSource(filename->getValue(),env);

/*
  ifstream is(filename->getValue().AsCharArray());
  if(!is)
    throw LambdaException("Cannot open file");

  Parser parser(is,env);
  AstForest *forest=parser.parse();
  is.close();
  
  global_evaluator->evaluateTopLevel(forest);
*/

  env.pushAR(ar);

  return true_object;
}



LambdaObject *ffEq(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *parm1=env.getParm(0);
  LambdaObject *parm2=env.getParm(1);

  return parm1==parm2 ? true_object : NULL;
}



LambdaObject *ffIsNil(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *parm=env.getParm(0);
  return parm==NULL ? true_object : NULL;
}



LambdaObject *ffIsList(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *parm=env.getParm(0);
  if(parm==NULL) return true_object;

  if(parm->getType()!=OBTYPE_CONS_CELL)
    return NULL;

  ConsCell *consCell=STATIC_CAST(ConsCell*,parm);
  return isList(consCell) ? true_object : NULL;
}



LambdaObject *ffIsConsCell(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *parm=env.getParm(0);
  if(parm==NULL) return NULL;

  return parm->getType()==OBTYPE_CONS_CELL ? true_object : NULL;
}



LambdaObject *ffSin(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *arg=env.getParm(0);
  if(arg==NULL)
    throw LambdaException("sin requires numeric argument");
  arg=toLambdaFloat(arg,env);

  LambdaFloat *doubleArg=STATIC_CAST(LambdaFloat*,arg);
  double value=doubleArg->getValue();

  double result=sin(value);
  
  LambdaFloat *f=new LambdaFloat(result);
  env.registerGarbage(f);
  return f;
}



LambdaObject *ffCos(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *arg=env.getParm(0);
  if(arg==NULL)
    throw LambdaException("cos requires numeric argument");
  arg=toLambdaFloat(arg,env);

  LambdaFloat *doubleArg=STATIC_CAST(LambdaFloat*,arg);
  double value=doubleArg->getValue();

  double result=cos(value);
  
  LambdaFloat *f=new LambdaFloat(result);
  env.registerGarbage(f);
  return f;
}



LambdaObject *ffTan(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *arg=env.getParm(0);
  if(arg==NULL)
    throw LambdaException("tan requires numeric argument");
  arg=toLambdaFloat(arg,env);

  LambdaFloat *doubleArg=STATIC_CAST(LambdaFloat*,arg);
  double value=doubleArg->getValue();

  double result=tan(value);
  
  LambdaFloat *f=new LambdaFloat(result);
  env.registerGarbage(f);
  return f;
}



LambdaObject *ffSqrt(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *arg=env.getParm(0);
  if(arg==NULL)
    throw LambdaException("sqrt requires numeric argument");
  arg=toLambdaFloat(arg,env);

  LambdaFloat *doubleArg=STATIC_CAST(LambdaFloat*,arg);
  double value=doubleArg->getValue();

  double result=sqrt(value);
  
  LambdaFloat *f=new LambdaFloat(result);
  env.registerGarbage(f);
  return f;
}



LambdaObject *ffAcos(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *arg=env.getParm(0);
  if(arg==NULL)
    throw LambdaException("acos requires numeric argument");
  arg=toLambdaFloat(arg,env);

  LambdaFloat *doubleArg=STATIC_CAST(LambdaFloat*,arg);
  double value=doubleArg->getValue();

  double result=acos(value);
  
  LambdaObject *o=new LambdaFloat(result);
  env.registerGarbage(o);
  return o;
}



LambdaObject *ffAsin(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *arg=env.getParm(0);
  if(arg==NULL)
    throw LambdaException("asin requires numeric argument");
  arg=toLambdaFloat(arg,env);

  LambdaFloat *doubleArg=STATIC_CAST(LambdaFloat*,arg);
  double value=doubleArg->getValue();

  double result=asin(value);
  
  LambdaObject *o=new LambdaFloat(result);
  env.registerGarbage(o);
  return o;
}



LambdaObject *ffAtan2(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *arg1=env.getParm(0);
  LambdaObject *arg2=env.getParm(1);

  if(arg1==NULL || arg2==NULL)
    throw LambdaException("atan2 requires numeric arguments");
  arg1=toLambdaFloat(arg1,env);
  arg2=toLambdaFloat(arg2,env);

  LambdaFloat *doubleArg1=STATIC_CAST(LambdaFloat*,arg1);
  LambdaFloat *doubleArg2=STATIC_CAST(LambdaFloat*,arg2);
  double value1=doubleArg1->getValue();
  double value2=doubleArg2->getValue();

  double result=atan2(value1,value2);
  
  LambdaFloat *f=new LambdaFloat(result);
  env.registerGarbage(f);
  return f;
}



LambdaObject *ffExp(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *arg=env.getParm(0);
  if(arg==NULL)
    throw LambdaException("exp requires numeric argument");
  arg=toLambdaFloat(arg,env);

  LambdaFloat *doubleArg=STATIC_CAST(LambdaFloat*,arg);
  double value=doubleArg->getValue();

  double result=exp(value);
  
  LambdaFloat *f=new LambdaFloat(result);
  env.registerGarbage(f);
  return f;
}



LambdaObject *ffLog(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *arg=env.getParm(0);
  if(arg==NULL)
    throw LambdaException("log requires numeric argument");
  arg=toLambdaFloat(arg,env);

  LambdaFloat *doubleArg=STATIC_CAST(LambdaFloat*,arg);
  double value=doubleArg->getValue();

  double result=log(value);
  
  LambdaFloat *f=new LambdaFloat(result);
  env.registerGarbage(f);
  return f;
}



LambdaObject *ffLog10(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *arg=env.getParm(0);
  if(arg==NULL)
    throw LambdaException("log10 requires numeric argument");
  arg=toLambdaFloat(arg,env);

  LambdaFloat *doubleArg=STATIC_CAST(LambdaFloat*,arg);
  double value=doubleArg->getValue();

  double result=log10(value);
  
  LambdaObject *o=new LambdaFloat(result);
  env.registerGarbage(o);
  return o;
}



LambdaObject *ffPow(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *arg1=env.getParm(0);
  LambdaObject *arg2=env.getParm(1);

  if(arg1==NULL || arg2==NULL)
    throw LambdaException("pow requires numeric arguments");
  arg1=toLambdaFloat(arg1,env);
  arg2=toLambdaFloat(arg2,env);

  LambdaFloat *doubleArg1=STATIC_CAST(LambdaFloat*,arg1);
  LambdaFloat *doubleArg2=STATIC_CAST(LambdaFloat*,arg2);
  double value1=doubleArg1->getValue();
  double value2=doubleArg2->getValue();

  double result=pow(value1,value2);
  
  LambdaFloat *f=new LambdaFloat(result);
  env.registerGarbage(f);
  return f;
}



LambdaObject *ffCeil(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *arg=env.getParm(0);
  if(arg==NULL)
    throw LambdaException("ceil requires numeric argument");
  arg=toLambdaFloat(arg,env);

  LambdaFloat *doubleArg=STATIC_CAST(LambdaFloat*,arg);
  double value=doubleArg->getValue();

  double result=ceil(value);
  
  LambdaObject *o=new LambdaInt((int)result);
  env.registerGarbage(o);
  return o;
}



LambdaObject *ffFloor(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *arg=env.getParm(0);
  if(arg==NULL)
    throw LambdaException("floor requires numeric argument");
  arg=toLambdaFloat(arg,env);

  LambdaFloat *doubleArg=STATIC_CAST(LambdaFloat*,arg);
  double value=doubleArg->getValue();

  double result=floor(value);
  
  LambdaObject *o=new LambdaInt((int)result);
  env.registerGarbage(o);
  return o;
}



LambdaObject *ffFloat(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *arg=env.getParm(0);
  if(arg==NULL) throw LambdaException("(float) requires argument");
  arg=toLambdaFloat(arg,env);
  LambdaFloat *doubleArg=STATIC_CAST(LambdaFloat*,arg);
  //env.registerGarbage(doubleArg);
  return doubleArg;
}



LambdaObject *ffFabs(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *arg=env.getParm(0);
  if(arg==NULL)
    throw LambdaException("fabs requires numeric argument");
  arg=toLambdaFloat(arg,env);

  LambdaFloat *doubleArg=STATIC_CAST(LambdaFloat*,arg);
  double value=doubleArg->getValue();

  double result=fabs(value);
  
  LambdaFloat *f=new LambdaFloat(result);
  env.registerGarbage(f);
  return f;
}



LambdaObject *ffAbs(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *arg=env.getParm(0);
  if(arg==NULL || arg->getType()!=OBTYPE_INT)
    throw LambdaException("abs requires int argument");

  LambdaInt *intArg=STATIC_CAST(LambdaInt*,arg);
  int value=intArg->getValue();

  int result=abs(value);
  
  LambdaObject *o=new LambdaInt(result);
  env.registerGarbage(o);
  return o;
}



LambdaObject *ffMakeVector(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *parm=env.getParm(0);

  if(parm==NULL || parm->getType()!=OBTYPE_INT)
    throw LambdaException("make-vector requires an int argument");

  LambdaInt *lambdaInt=STATIC_CAST(LambdaInt*,parm);
  
  LambdaObject *o=new LambdaVector(lambdaInt->getValue());
  env.registerGarbage(o);
  return o;
}



LambdaObject *ffIsVector(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *parm=env.getParm(0);

  return (parm && parm->getType()==OBTYPE_VECTOR) ?
    true_object : NULL;
}



LambdaObject *ffVectorRef(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *parm1=env.getParm(0);
  LambdaObject *parm2=env.getParm(1);

  if(parm1==NULL || parm1->getType()!=OBTYPE_VECTOR)
    throw LambdaException("vector-ref requires a vector argument");
  if(parm2==NULL || parm2->getType()!=OBTYPE_INT)
    throw LambdaException("vector-ref requires an int argument");

  LambdaVector *lambdaVector=STATIC_CAST(LambdaVector*,parm1);
  LambdaInt *lambdaInt=STATIC_CAST(LambdaInt*,parm2);
  
  return lambdaVector->getElement(lambdaInt->getValue());
}



LambdaObject *ffVectorSet(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *parm1=env.getParm(0);
  LambdaObject *parm2=env.getParm(1);
  LambdaObject *parm3=env.getParm(2);

  if(parm1==NULL || parm1->getType()!=OBTYPE_VECTOR)
    throw LambdaException("vector-set requires a vector argument");
  if(parm2==NULL || parm2->getType()!=OBTYPE_INT)
    throw LambdaException("vector-set requires an int argument");

  LambdaVector *lambdaVector=STATIC_CAST(LambdaVector*,parm1);
  LambdaInt *lambdaInt=STATIC_CAST(LambdaInt*,parm2);
  
  lambdaVector->setElement(lambdaInt->getValue(),parm3);

  return NULL;
}



LambdaObject *ffVectorLength(RunTimeEnvironment &env,ContinuationType &)
{  
  LambdaObject *parm=env.getParm(0);

  if(parm==NULL || parm->getType()!=OBTYPE_VECTOR)
    throw LambdaException("vector-length requires a vector argument");

  LambdaVector *lambdaVector=STATIC_CAST(LambdaVector*,parm);
  
  LambdaObject *o=new LambdaInt(lambdaVector->getSize());
  env.registerGarbage(o);
  return o;
}



LambdaObject *ffVectorToList(RunTimeEnvironment &env,ContinuationType &)
{
  LambdaObject *parm=env.getParm(0);

  if(parm==NULL || parm->getType()!=OBTYPE_VECTOR)
    throw LambdaException("vector-length requires a vector argument");

  LambdaVector *lambdaVector=STATIC_CAST(LambdaVector*,parm);

  ConsCell *consCell=VectorToList(lambdaVector);
  env.registerGarbage(consCell);
  return consCell;
}



LambdaObject *ffListToVector(RunTimeEnvironment &env,ContinuationType &)
{
  // (list-to-vector <list>)

  LambdaObject *parm=env.getParm(0);

  if(parm && parm->getType()!=OBTYPE_CONS_CELL)
    throw LambdaException("list-to-vector requires a list argument");

  ConsCell *consCell=STATIC_CAST(ConsCell*,parm);

  LambdaVector *vector=ListToVector(consCell);
  env.registerGarbage(vector);
  return vector;
}



LambdaObject *ffSetLocal(RunTimeEnvironment &env,ContinuationType &)
{
  // Allows setting of a local variable, given the lexical position of that variable
  // in the innermost enclosing scope

  // (set-local <lex-pos> <object>)

  LambdaObject *parm0=env.getParm(0);
  LambdaObject *parm1=env.getParm(1);
  
  if(!parm0 || parm0->getType()!=OBTYPE_INT)
    throw LambdaException("set-local requires integer lexical position");

  LambdaInt *lexPos=STATIC_CAST(LambdaInt*,parm0);

  LexicalAddress la(1,lexPos->getValue());
  env.set(la,parm1);

  return parm1;
}



LambdaObject *ffSetEnclosing(RunTimeEnvironment &env,ContinuationType &)
{
  // Allows setting of a variable in an enclosing scope

  // (set-enclosing <lex-depth> <lex-pos> <object>)

  LambdaObject *parm0=env.getParm(0);
  LambdaObject *parm1=env.getParm(1);
  LambdaObject *parm2=env.getParm(2);
  
  if(!parm0 || parm0->getType()!=OBTYPE_INT ||
     !parm1 || parm1->getType()!=OBTYPE_INT)
    throw LambdaException("set-local requires integer lexical address");

  LambdaInt *lexDepth=STATIC_CAST(LambdaInt*,parm0);
  LambdaInt *lexPos=STATIC_CAST(LambdaInt*,parm1);

  LexicalAddress la(lexDepth->getValue()+1,
		    lexPos->getValue());
  env.set(la,parm2);

  return parm2;
}



LambdaObject *ffRandReal(RunTimeEnvironment &env,ContinuationType &)
{
  // (rand-real)
  // returns random real between 0 and 1 (inclusive)

  return new LambdaFloat(Random0to1());
}



LambdaObject *ffRandInt(RunTimeEnvironment &env,ContinuationType &)
{
  // (rand-int <n>)
  // returns a random int between 0 and n-1 (inclusive)
  
  LambdaObject *parm=env.getParm(0);
  if(!parm || parm->getType()!=OBTYPE_INT)
    throw LambdaException("rand-int requires integer argument");

  LambdaInt *n=STATIC_CAST(LambdaInt*,parm);
  
  return new LambdaInt(RandomNumber(n->getValue()));
}



void declareBuiltInGlobals(RunTimeEnvironment &env)
{
  env.declareGlobal("print");
  env.declareGlobal("if");
  env.declareGlobal("nil");
  env.declareGlobal("true");
  env.declareGlobal("false");
  env.declareGlobal("cons");
  env.declareGlobal("car");
  env.declareGlobal("cdr");
  env.declareGlobal("+");
  env.declareGlobal("-");
  env.declareGlobal("*");
  env.declareGlobal("/");
  env.declareGlobal("%");
  env.declareGlobal("define");
  env.declareGlobal("<");
  env.declareGlobal(">");
  env.declareGlobal("<=");
  env.declareGlobal(">=");
  env.declareGlobal("=");
  env.declareGlobal("!=");
  env.declareGlobal("load");
  env.declareGlobal("eq?");
  env.declareGlobal("nil?");
  env.declareGlobal("list?");
  env.declareGlobal("is-cons-cell?");
  env.declareGlobal("sin");
  env.declareGlobal("cos");
  env.declareGlobal("tan");
  env.declareGlobal("sqrt");
  env.declareGlobal("acos");
  env.declareGlobal("asin");
  env.declareGlobal("atan2");
  env.declareGlobal("exp");
  env.declareGlobal("log");
  env.declareGlobal("log10");
  env.declareGlobal("pow");
  env.declareGlobal("ceil");
  env.declareGlobal("float");
  env.declareGlobal("floor");
  env.declareGlobal("fabs");
  env.declareGlobal("abs");
  env.declareGlobal("make-vector");
  env.declareGlobal("vector?");
  env.declareGlobal("vector-ref");
  env.declareGlobal("vector-set");
  env.declareGlobal("vector-length");
  env.declareGlobal("vector-to-list");
  env.declareGlobal("list-to-vector");
  env.declareGlobal("set-local");
  env.declareGlobal("set-enclosing");
  env.declareGlobal("random-real");
  env.declareGlobal("random-int");
  env.declareGlobal("protected-div");
}



void defineBuiltInGlobals(RunTimeEnvironment &env)
{
  // Define globals
  LambdaObject *ffo_print=new ForeignFunctionObject(&ffPrint,1);
  env.defineGlobal("print",ffo_print);

  LambdaObject *ffo_if=new ForeignFunctionObject(&ffIf,3);
  env.defineGlobal("if",ffo_if);

  env.defineGlobal("nil",NULL);

  true_object=new LambdaInt(1);
  env.defineGlobal("true",true_object);

  env.defineGlobal("false",NULL);

  LambdaObject *ffo_cons=new ForeignFunctionObject(&ffCons,2);
  env.defineGlobal("cons",ffo_cons);

  LambdaObject *ffo_car=new ForeignFunctionObject(&ffCar,1);
  env.defineGlobal("car",ffo_car);

  LambdaObject *ffo_cdr=new ForeignFunctionObject(&ffCdr,1);
  env.defineGlobal("cdr",ffo_cdr);

  LambdaObject *ffo_plus=new ForeignFunctionObject(&ffPlus,2);
  env.defineGlobal("+",ffo_plus);

  LambdaObject *ffo_minus=new ForeignFunctionObject(&ffMinus,2);
  env.defineGlobal("-",ffo_minus);

  LambdaObject *ffo_times=new ForeignFunctionObject(&ffTimes,2);
  env.defineGlobal("*",ffo_times);

  LambdaObject *ffo_divide=new ForeignFunctionObject(&ffDivide,2);
  env.defineGlobal("/",ffo_divide);

  LambdaObject *ffo_modulus=new ForeignFunctionObject(&ffModulus,2);
  env.defineGlobal("%",ffo_modulus);

  LambdaObject *ffo_define=new ForeignFunctionObject(&ffDefine,2);
  env.defineGlobal("define",ffo_define);

  LambdaObject *ffo_less=new ForeignFunctionObject(&ffLess,2);
  env.defineGlobal("<",ffo_less);

  LambdaObject *ffo_lessEqual=new ForeignFunctionObject(&ffLessEqual,2);
  env.defineGlobal("<=",ffo_lessEqual);

  LambdaObject *ffo_greater=new ForeignFunctionObject(&ffGreater,2);
  env.defineGlobal(">",ffo_greater);

  LambdaObject *ffo_greaterEqual=new ForeignFunctionObject(&ffGreaterEqual,2);
  env.defineGlobal(">=",ffo_greaterEqual);

  LambdaObject *ffo_equal=new ForeignFunctionObject(&ffEqual,2);
  env.defineGlobal("=",ffo_equal);

  LambdaObject *ffo_notEqual=new ForeignFunctionObject(&ffNotEqual,2);
  env.defineGlobal("!=",ffo_notEqual);

  LambdaObject *ffo_load=new ForeignFunctionObject(&ffLoad,1);
  env.defineGlobal("load",ffo_load);

  LambdaObject *ffo_eq=new ForeignFunctionObject(&ffEq,2);
  env.defineGlobal("eq?",ffo_eq);

  LambdaObject *ffo_IsNil=new ForeignFunctionObject(&ffIsNil,1);
  env.defineGlobal("nil?",ffo_IsNil);

  LambdaObject *ffo_IsList=new ForeignFunctionObject(&ffIsList,1);
  env.defineGlobal("list?",ffo_IsList);

  LambdaObject *ffo_IsConsCell=new ForeignFunctionObject(&ffIsConsCell,1);
  env.defineGlobal("is-cons-cell?",ffo_IsConsCell);

  LambdaObject *ffo_sin=new ForeignFunctionObject(&ffSin,1);
  env.defineGlobal("sin",ffo_sin);

  LambdaObject *ffo_cos=new ForeignFunctionObject(&ffCos,1);
  env.defineGlobal("cos",ffo_cos);

  LambdaObject *ffo_tan=new ForeignFunctionObject(&ffTan,1);
  env.defineGlobal("tan",ffo_tan);

  LambdaObject *ffo_Sqrt=new ForeignFunctionObject(&ffSqrt,1);
  env.defineGlobal("sqrt",ffo_Sqrt);

  LambdaObject *ffo_Acos=new ForeignFunctionObject(&ffAcos,1);
  env.defineGlobal("acos",ffo_Acos);

  LambdaObject *ffo_Asin=new ForeignFunctionObject(&ffAsin,1);
  env.defineGlobal("asin",ffo_Asin);

  LambdaObject *ffo_Atan2=new ForeignFunctionObject(&ffAtan2,2);
  env.defineGlobal("atan2",ffo_Atan2);

  LambdaObject *ffo_Exp=new ForeignFunctionObject(&ffExp,1);
  env.defineGlobal("exp",ffo_Exp);

  LambdaObject *ffo_Log=new ForeignFunctionObject(&ffLog,1);
  env.defineGlobal("log",ffo_Log);

  LambdaObject *ffo_Log10=new ForeignFunctionObject(&ffLog10,1);
  env.defineGlobal("log10",ffo_Log10);

  LambdaObject *ffo_Pow=new ForeignFunctionObject(&ffPow,2);
  env.defineGlobal("pow",ffo_Pow);

  LambdaObject *ffo_Ceil=new ForeignFunctionObject(&ffCeil,1);
  env.defineGlobal("ceil",ffo_Ceil);

  LambdaObject *ffo_Floor=new ForeignFunctionObject(&ffFloor,1);
  env.defineGlobal("floor",ffo_Floor);

  LambdaObject *ffo_Float=new ForeignFunctionObject(&ffFloat,1);
  env.defineGlobal("float",ffo_Float);

  LambdaObject *ffo_Fabs=new ForeignFunctionObject(&ffFabs,1);
  env.defineGlobal("fabs",ffo_Fabs);

  LambdaObject *ffo_Abs=new ForeignFunctionObject(&ffAbs,1);
  env.defineGlobal("abs",ffo_Abs);

  LambdaObject *ffo_MakeVector=new ForeignFunctionObject(&ffMakeVector,1);
  env.defineGlobal("make-vector",ffo_MakeVector);

  LambdaObject *ffo_IsVector=new ForeignFunctionObject(&ffIsVector,1);
  env.defineGlobal("vector?",ffo_IsVector);

  LambdaObject *ffo_VectorRef=new ForeignFunctionObject(&ffVectorRef,2);
  env.defineGlobal("vector-ref",ffo_VectorRef);

  LambdaObject *ffo_VectorSet=new ForeignFunctionObject(&ffVectorSet,3);
  env.defineGlobal("vector-set",ffo_VectorSet);

  LambdaObject *ffo_VectorLength=new ForeignFunctionObject(&ffVectorLength,1);
  env.defineGlobal("vector-length",ffo_VectorLength);

  LambdaObject *ffo_VectorToList=new ForeignFunctionObject(&ffVectorToList,1);
  env.defineGlobal("vector-to-list",ffo_VectorToList);

  LambdaObject *ffo_ListToVector=new ForeignFunctionObject(&ffListToVector,1);
  env.defineGlobal("list-to-vector",ffo_ListToVector);

  LambdaObject *ffo_SetLocal=new ForeignFunctionObject(&ffSetLocal,2);
  env.defineGlobal("set-local",ffo_SetLocal);

  LambdaObject *ffo_SetEnclosing=new ForeignFunctionObject(&ffSetEnclosing,3);
  env.defineGlobal("set-enclosing",ffo_SetEnclosing);

  LambdaObject *ffo_RandReal=new ForeignFunctionObject(&ffRandReal,0);
  env.defineGlobal("random-real",ffo_RandReal);

  LambdaObject *ffo_RandInt=new ForeignFunctionObject(&ffRandInt,1);
  env.defineGlobal("random-int",ffo_RandInt);

  LambdaObject *ffo_protectedDiv=new ForeignFunctionObject(&ffProtectedDiv,2);
  env.defineGlobal("protected-div",ffo_protectedDiv);
}



void defineGlobals(RunTimeEnvironment &env)
{
  // deprecated

  declareBuiltInGlobals(env);
  env.pushGlobalAR();  
  defineBuiltInGlobals(env);
}

}
