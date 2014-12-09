/*
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 */

#ifndef INCL_Parser_H
#define INCL_Parser_H
#include "AST.H"
#include "TokenStream.H"
#include "RunTimeEnvironment.H"
using namespace std;
using namespace BOOM;
namespace Lambda {

/*****************************************************
                         GRAMMAR

  Program ::= Expression Program
  Program ::= epsilon
  
  Expression ::= Combination 
  Expression ::= Literal 
  Expression ::= IDENT 
  
  Literal ::= LambdaExpr 
  Literal ::= INT_LIT 
  Literal ::= FLOAT_LIT 
  Literal ::= STRING_LIT 
  Literal ::= CHAR_LIT 
  Literal ::= Operator 
  Literal ::= SYMBOL
  
  Operator ::=TIMES
  Operator ::=DIVIDE
  Operator ::=MOD 
  Operator ::=PLUS 
  Operator ::=MINUS 
  Operator ::=LESS 
  Operator ::=GREATER 
  Operator ::=LESS_EQUAL 
  Operator ::=GREATER_EQUAL 
  Operator ::=EQUAL 
  Operator ::=NOT_EQUAL
  
  LambdaExpr ::= L_BRACKET  ParmList  PIPE  ExprList  R_BRACKET
  
  ParmList ::= IDENT  ParmList 
  ParmList ::= epsilon
  
  Combination ::= L_PAREN  ExprList  R_PAREN
  
  ExprList ::= Expression  ExprList
  ExprList ::= Expression
  
*****************************************************/



// **************************************************
//                   class Parser
// **************************************************

class Parser
{
  TokenStream &tokenStream;
  RunTimeEnvironment &env;
  int currentLexDepth; // current lexical depth (scope nesting)
  bool regWithGC; // whether to register tree nodes with GC

  void match(TokenType);
  bool canBeginExpression(TokenType);
  AstVariable *makeVariableNode(AstNode *parent);
  void declareParms(AstForest &);

  // Parsing procedures
  AstForest *ppProgram();
  AstNode *ppExpression(AstNode *parent);
  AstNode *ppLiteral(AstNode *parent);
  AstNode *ppOperator(AstNode *parent);
  AstNode *ppLambdaExpr(AstNode *parent);
  AstForest *ppParmList(AstNode *parent);
  AstNode *ppCombination(AstNode *parent);
  AstForest *ppExprList(AstNode *parent);
public:
  Parser(TokenStream *,RunTimeEnvironment &,bool regWithGC=true);
  Parser(istream &,RunTimeEnvironment &,bool regWithGC=true);
  ~Parser();
  AstForest *parse();
};
}

#endif
