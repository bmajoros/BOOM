/*
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/

#include "Parser.H"
#include "LambdaException.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;


// **************************************************
//                 Parser methods
// **************************************************

Parser::Parser(TokenStream *tokenStream,RunTimeEnvironment &env,
  bool regWithGC)
  : tokenStream(*tokenStream), env(env), regWithGC(regWithGC)
{
  // ctor
}



Parser::Parser(istream &is,RunTimeEnvironment &env,bool regWithGC)
  : tokenStream(*new TokenStream(is)), env(env), 
    regWithGC(regWithGC)
{
  // ctor
}



Parser::~Parser()
{
  // dtor

  delete &tokenStream;
}



AstForest *Parser::parse()
{
  return ppProgram();
}



void Parser::match(TokenType tokenType)
{
  Token *token=tokenStream.nextToken();
  if(token->getTokenType()!=tokenType)
    throw SyntaxError(token);
  delete token;
}



AstForest *Parser::ppProgram()
{
  // First={TOK_LEFT_PAREN,TOK_IDENT,TOK_INT_LIT,TOK_FLOAT_LIT,
  //        TOK_STRING_LIT,TOK_CHAR_LIT,TOK_SYMBOL,TOK_TIMES,
  //        TOK_DIVIDE,TOK_MOD,TOK_PLUS,TOK_MINUS,TOK_GREATER,
  //        TOK_GREATER_EQUAL,TOK_LESS,TOK_LESS_EQUAL,TOK_EQUAL,
  //        TOK_NOT_EQUAL,TOK_LEFT_BRACKET}

  //Program ::= Expression Program
  //Program ::= epsilon

  currentLexDepth=0;

  AstForest *forest=new AstForest;
  if(regWithGC)
    env.registerGarbage(forest);

  while(tokenStream.peekTokenType()!=TOK_EOF)
    forest->appendTree(ppExpression(forest));

  return forest;
}



AstVariable *Parser::makeVariableNode(AstNode *parent)
{
  AstVariable *astNode; // return value
  
  Token *token=tokenStream.nextToken(); // TOK_IDENT

  // Look up this identifier in the ScopeStack
  int lexicalDepth;
  LambdaSymTabNode *symTabNode=env.Find(token->getLexeme(),lexicalDepth);

  if(symTabNode && symTabNode->isResolved())
    {
      // It was found...use the lexical address to create
      // a bound variable node

      const int lexicalPosition=symTabNode->getLexicalPosition();
      LexicalAddress la(lexicalDepth,lexicalPosition);

      astNode=new AstBoundVariable(la);
      if(regWithGC)
	env.registerGarbage(astNode);
    }
  else
    {
      BackpatchNode *backpatchNode=
	static_cast<BackpatchNode*>(symTabNode);// may be NULL

      if(!backpatchNode)
	{
	  // It was not found...create a BackpatchNode so that
	  // nodes needing backpatching can be stored for later

	  int lexicalPosition=env.getGlobalScopeSize();
	  backpatchNode=
	    new BackpatchNode(token->getLexeme(),lexicalPosition);
	  env.declareGlobal(backpatchNode,token->getLexeme());
	}
      
      // Add this identifier to the list of nodes to be backpatched
      // later when the name gets defined
      astNode=
	new AstFreeVariable(token->getLexeme(),currentLexDepth,parent);
      if(regWithGC)
	env.registerGarbage(astNode);
      backpatchNode->addToList(astNode);
    }
  
  delete token;
  return astNode;
}



AstNode *Parser::ppExpression(AstNode *parent)
{
  // First={TOK_LEFT_PAREN,TOK_IDENT,TOK_INT_LIT,TOK_FLOAT_LIT,
  //        TOK_STRING_LIT,TOK_CHAR_LIT,TOK_SYMBOL,TOK_TIMES,
  //        TOK_DIVIDE,TOK_MOD,TOK_PLUS,TOK_MINUS,TOK_GREATER,
  //        TOK_GREATER_EQUAL,TOK_LESS,TOK_LESS_EQUAL,TOK_EQUAL,
  //        TOK_NOT_EQUAL,TOK_LEFT_BRACKET}

  //Expression ::= Combination 
  //Expression ::= Literal 
  //Expression ::= TOK_IDENT 

  switch(tokenStream.peekTokenType())
    {
    case TOK_IDENT:
      return makeVariableNode(parent);

    case TOK_LEFT_PAREN:
      return ppCombination(parent);

    default:
      return ppLiteral(parent);
    }
}



bool Parser::canBeginExpression(TokenType tokenType)
{
  switch(tokenType)
    {
    case TOK_LEFT_PAREN:
    case TOK_IDENT:
    case TOK_INT_LIT:
    case TOK_FLOAT_LIT:
    case TOK_STRING_LIT:
    case TOK_CHAR_LIT:
    case TOK_SYMBOL:
    case TOK_TIMES:
    case TOK_DIVIDE:
    case TOK_MOD:
    case TOK_PLUS:
    case TOK_MINUS:
    case TOK_GREATER:
    case TOK_GREATER_EQUAL:
    case TOK_LESS:
    case TOK_LESS_EQUAL:
    case TOK_EQUAL:
    case TOK_NOT_EQUAL:
    case TOK_LEFT_BRACKET:
      return true;
    }
  return false;
}




AstNode *Parser::ppLiteral(AstNode *parent)
{
  // First={TOK_INT_LIT,TOK_FLOAT_LIT,TOK_LEFT_BRACKET
  //        TOK_STRING_LIT,TOK_CHAR_LIT,TOK_SYMBOL,TOK_TIMES,
  //        TOK_DIVIDE,TOK_MOD,TOK_PLUS,TOK_MINUS,TOK_GREATER,
  //        TOK_GREATER_EQUAL,TOK_LESS,TOK_LESS_EQUAL,TOK_EQUAL,
  //        TOK_NOT_EQUAL}

  //Literal ::= LambdaExpr 
  //Literal ::= TOK_INT_LIT 
  //Literal ::= TOK_FLOAT_LIT 
  //Literal ::= TOK_STRING_LIT 
  //Literal ::= TOK_CHAR_LIT 
  //Literal ::= Operator 
  //Literal ::= TOK_SYMBOL

  switch(tokenStream.peekTokenType())
    {
    case TOK_LEFT_BRACKET:
      return ppLambdaExpr(parent);
    case TOK_INT_LIT:
      {
	Token *token=tokenStream.nextToken();
	AstNode *node=new AstIntLit(token->getLexeme().asInt());
	if(regWithGC)
	  env.registerGarbage(node);
	delete token;
	return node;
      }
    case TOK_FLOAT_LIT:
      {
	Token *token=tokenStream.nextToken();
	AstNode *node=
	  new AstFloatLit(token->getLexeme().asFloat());
	if(regWithGC)
	  env.registerGarbage(node);
	delete token;
	return node;
      }
    case TOK_STRING_LIT:
      {
	Token *token=tokenStream.nextToken();
	AstNode *node=new AstStringLit(token->getLexeme());
	if(regWithGC)
	  env.registerGarbage(node);
	delete token;
	return node;
      }
    case TOK_CHAR_LIT:
      {
	Token *token=tokenStream.nextToken();
	char c=token->getLexeme().c_str()[0];
	AstNode *node=new AstCharLit(c);
	if(regWithGC)
	  env.registerGarbage(node);
	delete token;
	return node;
      }
    case TOK_SYMBOL:
      {
	Token *token=tokenStream.nextToken();
	Symbol *symbol=
	  env.findOrCreateSymbol(token->getLexeme());
	AstNode *node=new AstSymbol(symbol);
	if(regWithGC)
	  env.registerGarbage(node);
	delete token;
	return node;
      }
      return NULL;
    default:
      return ppOperator(parent);
    }
}



AstNode *Parser::ppOperator(AstNode *parent)
{
  // First={TOK_TIMES,
  //        TOK_DIVIDE,TOK_MOD,TOK_PLUS,TOK_MINUS,TOK_GREATER,
  //        TOK_GREATER_EQUAL,TOK_LESS,TOK_LESS_EQUAL,TOK_EQUAL,
  //        TOK_NOT_EQUAL}

  //Operator ::=TOK_TIMES
  //Operator ::=TOK_DIVIDE
  //Operator ::=TOK_MOD 
  //Operator ::=TOK_PLUS 
  //Operator ::=TOK_MINUS 
  //Operator ::=TOK_LESS 
  //Operator ::=TOK_GREATER 
  //Operator ::=TOK_LESS_EQUAL 
  //Operator ::=TOK_GREATER_EQUAL 
  //Operator ::=TOK_EQUAL 
  //Operator ::=TOK_NOT_EQUAL


  switch(tokenStream.peekTokenType())
    {
    case TOK_TIMES:
    case TOK_DIVIDE:
    case TOK_MOD:
    case TOK_PLUS:
    case TOK_MINUS:
    case TOK_LESS:
    case TOK_GREATER:
    case TOK_LESS_EQUAL:
    case TOK_GREATER_EQUAL:
    case TOK_EQUAL:
    case TOK_NOT_EQUAL:
      return makeVariableNode(parent);
    default:
      {
	Token *token=tokenStream.nextToken();
	throw SyntaxError(token);
      }
    }
}



void Parser::declareParms(AstForest &parms)
{
  const int numParms=parms.getNumTrees();
  int i;
  for(i=0 ; i<numParms ; ++i)
    {
      AstFreeVariable *freeVar=
	static_cast<AstFreeVariable*>(parms.getIthTree(i));
      LambdaSymTabNode *node=
	new LambdaSymTabNode(freeVar->getLexeme(),i);
      env.Declare(node,freeVar->getLexeme());
    }
}



AstNode *Parser::ppLambdaExpr(AstNode *parent)
{
  // First={TOK_LEFT_BRACKET}

  //LambdaExpr ::= TOK_LEFT_BRACKET ParmList TOK_PIPE 
  //                   ExprList TOK_RIGHT_BRACKET

  match(TOK_LEFT_BRACKET);
  env.enterScope();
  ++currentLexDepth;

  AstForest *parmList=ppParmList(parent);
  const int arity=parmList->getNumTrees();

  declareParms(*parmList);

  match(TOK_PIPE);

  AstForest *body=ppExprList(parent);
  delete parmList;

  match(TOK_RIGHT_BRACKET);
  env.leaveScope();
  --currentLexDepth;

  AstLambda *lambda=new AstLambda(arity,body);
  if(regWithGC)
    env.registerGarbage(lambda);
  return lambda;
}



AstForest *Parser::ppParmList(AstNode *parent)
{
  // First={TOK_IDENT}

  //ParmList ::= TOK_IDENT  ParmList 
  //ParmList ::= epsilon

  AstForest *forest=new AstForest;
  //env.registerGarbage(forest); // don't register with GC; caller will delete it

  while(tokenStream.peekTokenType()==TOK_IDENT)
    {
      Token *token=tokenStream.nextToken();
      AstNode *node=
	new AstFreeVariable(token->getLexeme(),0,parent);
      if(regWithGC)
	env.registerGarbage(node);
      forest->appendTree(node);
      delete token;
    }

  return forest;
}



AstNode *Parser::ppCombination(AstNode *parent)
{
  // First={TOK_LEFT_PAREN}

  //Combination ::= TOK_LEFT_PAREN  ExprList  TOK_RIGHT_PAREN

  match(TOK_LEFT_PAREN);
  AstForest *forest=ppExprList(parent);
  const int numTrees=forest->getNumTrees();
  match(TOK_RIGHT_PAREN);

  AstNode *function=forest->getIthTree(0);
  Array1D<AstNode*> &parms=*new Array1D<AstNode*>(numTrees-1);
  int i;
  for(i=1 ; i<numTrees ; ++i)
    parms[i-1]=forest->getIthTree(i);
  AstNode *combo=new AstCombination(function,&parms);
  if(regWithGC)
    env.registerGarbage(combo);

  return combo;
}



AstForest *Parser::ppExprList(AstNode *parent)
{
  // First={TOK_LEFT_PAREN,TOK_IDENT,TOK_INT_LIT,TOK_FLOAT_LIT,
  //        TOK_STRING_LIT,TOK_CHAR_LIT,TOK_SYMBOL,TOK_TIMES,
  //        TOK_DIVIDE,TOK_MOD,TOK_PLUS,TOK_MINUS,TOK_GREATER,
  //        TOK_GREATER_EQUAL,TOK_LESS,TOK_LESS_EQUAL,TOK_EQUAL,
  //        TOK_NOT_EQUAL,TOK_LEFT_BRACKET}

  //ExprList ::= Expression  ExprList
  //ExprList ::= Expression

  AstForest *forest=new AstForest;
  if(regWithGC)
    env.registerGarbage(forest);

  forest->appendTree(ppExpression(parent));
  while(canBeginExpression(tokenStream.peekTokenType()))
    forest->appendTree(ppExpression(parent));

  return forest;
}





