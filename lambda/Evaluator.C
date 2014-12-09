/*
 Evaluator.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "Evaluator.H"
#include "AST.H"
#include "LambdaInt.H"
#include "LambdaFloat.H"
#include "LambdaString.H"
#include "LambdaChar.H"
#include "LambdaException.H"
#include "Closure.H"
#include <iostream>
#include "ForeignFunctionObject.H"
#include "AstPrinter.H"
#include <sstream>
#include "ObjectPrinter.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;

Evaluator::Evaluator(RunTimeEnvironment &env)
  : env(env)
{
  // ctor
}



LambdaObject *Evaluator::evaluateTopLevel(AstNode *ast)
{
  LambdaObject *object=evaluate(ast);
  env.cleanStack();
  return object;
}



Array1D<LambdaObject*> *Evaluator::evaluateParms(
   Array1D<AstNode*> &expressions)
{
  Array1D<LambdaObject*> &values=
    *new Array1D<LambdaObject*>(expressions.size());

  int i;
  for(i=0 ; i<expressions.size() ; ++i)
    {
      ActivationRecord *highWater=env.getTopAR();
      LambdaObject *parm=evaluate((AstNode*)expressions[i],false);
      values[i]=parm;
      localMarkStack.push(parm);
      env.popDownTo(highWater);
    }

  //purgeMarkStack();

  return &values;
}



LambdaObject *Evaluator::evaluate(AstNode *ast,bool tailRecursive)
{
  localMarkStack.push(ast);//###
  //env.checkGarbageLevel();// run GC only if necessary

  AstNode *currentNode=ast;
  while(true)
    {
      switch(currentNode->getNodeType())
	{
	case AST_OBJECT: // ###
	  return static_cast<AstObject*>(currentNode)->getObject();
	  break;
	case AST_COMBINATION:
	  {
	    // Uncomment to verify tail recursion optimization:
	    //cout << "Stack depth=" << env.getStackDepth() << endl;//###

	    AstCombination *combo=
	      static_cast<AstCombination*>(currentNode);
	    AstNode *functionNode=combo->getFunction();
	    Array1D<AstNode*> &parmExpressions=combo->getParms();

	    // Evaluate the function
	    ActivationRecord *highWater=env.getTopAR();
	    localMarkStack.push(currentNode);

	    LambdaObject *function=evaluate(functionNode);

	    if(!function) throw LambdaException("nil function called");
	    env.popDownTo(highWater);

	    // Evaluate the parameters
	    localMarkStack.push(function);
	    Array1D<LambdaObject*> *parmValues=
	      evaluateParms(parmExpressions);//could invoke GC
	    //purgeMarkStack();

	    // Handle internal & external functions differently
	    switch(function->getType())
	      {
	      case OBTYPE_CLOSURE: // A LAMBDA function
		{
		  Closure *closure=static_cast<Closure*>(function);
		  const int arity=closure->getArity();
		  ActivationRecord *ar=
		    env.prepareAR(*parmValues,arity,closure->getStaticChain());
		  delete parmValues;

		  // Pop old AR if this call is to be tail-recursive
		  //###if(tailRecursive) env.popAR_unlessGlobal();

		  // Push new AR for function about to be called
		  env.pushAR(ar);

		  // Call the function by executing a GOTO
		  currentNode=closure->getFunction()->getBody();
		  continue;//=goto top of loop
		}
		break;
	      case OBTYPE_FOREIGN_FUNCTION: // A C++ function
		{
		  ForeignFunctionObject *ffo=
		    static_cast<ForeignFunctionObject*>(function);
		  const int arity=ffo->getArity();

		  /*
		   Prepare an AR.  Notice the static chain 
		   is not quite what we would expect; it is 
		   set to the AR at the top of the stack so that
		   built-in routines can do sophisticated 
		   things, like change local variables at 
		   any depth in the stack.
		  */
		  ActivationRecord *ar=
		    env.prepareAR(*parmValues,arity,env.getTopAR());
		  delete parmValues;

		  // Pop old AR if this call is to be 
		  // tail-recursive
		  //###if(tailRecursive) env.popAR_unlessGlobal();

		  // Push new AR for function about to be called
		  env.pushAR(ar);

cpp_ff_tail: //destination of goto, below

		  // Call the C++ function
		  ForeignFunction ff=ffo->getCPlusPlusFunction();
		  ContinuationType continuation=CONT_SIMPLE_RETURN;
		  localMarkStack.push(currentNode);
		  LambdaObject *result=(*ff)(env,continuation);

		  // See if the C++ function wants to make a
		  // tail-recursive call to some other function
		  switch(continuation)
		    {
		    case CONT_TAIL_REC_CALL:
		      // 'result' is to be interpreted as another
		      // function to which we should make a tail-
		      // recursive call.  The C++ function is
		      // assumed to have prepared an AR and
		      // pushed it onto the stack.
		      {
			ActivationRecord *newAR=env.getTopAR();
			env.popAR();//newAR
			env.popAR();//the AR below newAR
			env.pushAR(newAR);
			
			function=result;
			if(!function)
			  throw LambdaException("nil function called");
			switch(function->getType())
			  {
			  case OBTYPE_CLOSURE: // A LAMBDA function
			    // Call the function by executing a GOTO
			    {
			      Closure *closure=
				static_cast<Closure*>(function);
			      currentNode=
				closure->getFunction()->getBody();
			      continue;//=goto top of loop
			    }
			  case OBTYPE_FOREIGN_FUNCTION: // A C++ function
			    ffo=
			      static_cast<ForeignFunctionObject*>(function);
			    goto cpp_ff_tail;//first 'goto' I've ever written in C++
			  default:
			    throw LambdaException("call of a non-function");
			  }
		      }
		      break;
		    case CONT_SIMPLE_RETURN:
		      // 'result' is to be interpreted as the
		      // return value of the C++ function.
		      env.popAR();
		      return result;
		    }
		}
		break;
	      default:
		cout<<function->getType()<<endl;
		throw LambdaException("Call of a non-function");
	      }
	  }
	  break;
	case AST_BOUND_VARIABLE:
	  {
	    AstBoundVariable *boundVar=
	      static_cast<AstBoundVariable*>(currentNode);
	    LambdaObject *obj=env.retrieve(boundVar->getLexAddr());
	    return obj;
	  }
	  break;
	case AST_FREE_VARIABLE:
	  {
	    AstFreeVariable *freeVar=
	      static_cast<AstFreeVariable*>(currentNode);
	    ostringstream os;
	    os << "Free variable " << freeVar->getLexeme() <<
	      " encountered";
	    throw LambdaException(os.str());
	  }
	  break;
	case AST_INT_LIT:
	  {
	    AstIntLit *astIntLit=
	      static_cast<AstIntLit*>(currentNode);
	    LambdaObject *o=new LambdaInt(astIntLit->getValue());
	    env.registerGarbage(o);
	    return o;
	  }
	  break;
	case AST_FLOAT_LIT:
	  {
	    AstFloatLit *astFloatLit=
	      static_cast<AstFloatLit*>(currentNode);
	    LambdaObject *o=
	      new LambdaFloat(astFloatLit->getValue());
	    env.registerGarbage(o);
	    return o;
	  }
	  break;
	case AST_LAMBDA:
	  {
	    ActivationRecord *staticChain=env.getTopAR();
	    AstLambda *lambda=
	      static_cast<AstLambda*>(currentNode);
	    LambdaObject *o=new Closure(staticChain,lambda);
	    env.registerGarbage(o);
	    return o;
	  }
	  break;
	case AST_STRING_LIT:
	  {
	    AstStringLit *astStringLit=
	      static_cast<AstStringLit*>(currentNode);
	    LambdaObject *o=
	      new LambdaString(astStringLit->getValue());
	    env.registerGarbage(o);
	    return o;
	  }
	  break;
	case AST_CHAR_LIT:
	  {
	    AstCharLit *astCharLit=
	      static_cast<AstCharLit*>(currentNode);
	    LambdaObject *o=new LambdaChar(astCharLit->getValue());
	    env.registerGarbage(o);
	    return o;
	  }
	  break;
	case AST_SYMBOL:
	  return static_cast<AstSymbol*>(currentNode)->getSymbol();
	  break;
	case AST_FOREST:
	  {
	    AstForest *forest=static_cast<AstForest*>(currentNode);
	    int i, num=forest->getNumTrees()-1;
	    localMarkStack.push(forest);
	    for(i=0 ; i<num ; ++i)
	      {
		ActivationRecord *highWater=env.getTopAR();
		evaluate(forest->getIthTree(i),false);
		env.popDownTo(highWater);
	      }
	    //purgeMarkStack();
	    currentNode=forest->getIthTree(i);
	    continue;
	  }
	  break;
	}

      if(currentNode==ast) break;
    }

  return NULL; //  unreachable code
}



void Evaluator::pushAccessibles(MarkStack &markStack)
{
  Garbage *g;
  while(g=localMarkStack.pop())
    markStack.push(g);
  //purgeMarkStack();//###
}



void Evaluator::purgeMarkStack()
{
  //while(localMarkStack.pop());
  localMarkStack.clear();
}




