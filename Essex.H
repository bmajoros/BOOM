/*
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 */
#ifndef INCL_Essex_H
#define INCL_Essex_H
#include <fstream>
#include "Vector.H"
#include "String.H"
#include "Regex.H"
using namespace std;
using namespace BOOM;
namespace Essex {


/*****************************************************

 ESSEX : S-Expression Data Format

 The Essex format is very simple: it consists of
 hierarchical or list structures organized by paren-
 theses. For example:

    (book
       (title "Some Title")
       (author "Some Author")
       (other-attributes a1 a2 a3 a4 ...etc... )
    )
    (book
       ...etc...
    )
    ...etc...

 You can parse an Essex file very simply via:

   #include "Essex.H"
   Essex::Parser parser("filename.sx");
   while(!parser.eof()) {
      Essex::Node *elem=parser.nextElem();
      if(!elem) break;
      ...
   }   

The following special characters are defined, similarly to
HTML:

  code         meaning
  ----         -------
  &lparen;     (
  &rparen;     )
  &tab;        <tab>
  &space;      <space>

Within a string literal, you can precede any character by a '\' to
"escape" that character; the only characters for which this is
useful are:

  code  meaning
  ----  -------
  \n    newline
  \r    linefeed
  \"    quote
  
*****************************************************/


/*****************************************************
                      ESSEX GRAMMAR

  File ::= SExpression File
  File ::= 
  
  SExpression ::= Combination 
  SExpression ::= Literal 
  
  Literal ::= INT_LIT 
  Literal ::= FLOAT_LIT 
  Literal ::= STRING_LIT 
  Literal ::= IDENT
  
  Combination ::= L_PAREN  IDENT  ExprList  R_PAREN
  
  ExprList ::= SExpression  ExprList
  ExprList ::= 
  
*****************************************************/




// **************************************************
//                    Node classes
// **************************************************

enum NodeType
{
  COMPOSITE,
  NUMBER,
  STRING
};

class NodeVisitor;

class Node
{
  NodeType nodeType;
public:
  Node(NodeType);
  virtual ~Node();
  NodeType getNodeType() const;
  virtual void acceptVisitor(NodeVisitor &)=0;
  virtual Node *findChild(const String &tag);
  virtual void findChildren(const String &tag,Vector<Node*> &);
  virtual void findDescendents(const String &tag,Vector<Node*> &);
  virtual String getAttribute(const String &tag);
  virtual void printOn(ostream &) const=0;
  virtual void printXML(ostream &) const=0;
};

ostream &operator<<(ostream &,const Node &);

class NumericNode : public Node {
  float value;
public:
  NumericNode(float);
  float getValue() const;
  virtual void acceptVisitor(NodeVisitor &);
  virtual void printOn(ostream &) const;
  virtual void printXML(ostream &) const;
};

class StringNode : public Node {
  String value;
public:
  StringNode(const String &);
  const String &getValue() const;
  String &getValue();
  virtual void acceptVisitor(NodeVisitor &);
  virtual void printOn(ostream &) const;
  virtual void printXML(ostream &) const;
};

class CompositeNode : public Node {
  String tag;
  Vector<Node*> children;
  void printRecursive(ostream &,int depth) const;
  void printXMLrecursive(ostream &,int depth) const;
public:
  CompositeNode(const String &tag);
  virtual ~CompositeNode();
  const String &getTag() const;
  String &getTag();
  int getNumChildren() const;
  Node *getIthChild(int) const;
  void setIthChild(int,Node *);
  void appendChild(Node *);
  void prepend(const String &);
  void prepend(Node *node) { if(node) children.push_front(node); }
  void append(Node *node) { appendChild(node); }
  void append(int);
  void append(float);
  void append(double d) { append(float(d)); }
  void append(const String &);
  void append(const String &tag,int value);
  void append(const String &tag,float value);
  void append(const String &tag,const String & value);
  void append(const String &tag,const char *value);
  virtual void acceptVisitor(NodeVisitor &);
  virtual Node *findChild(const String &tag);
  virtual void findChildren(const String &tag,Vector<Node*> &);
  virtual void findDescendents(const String &tag,Vector<Node*> &);
  virtual String getAttribute(const String &tag);
  virtual void setAttribute(const String &tag,const String &value);
  virtual void printOn(ostream &) const;
  virtual void printXML(ostream &) const;
  bool hasCompositeChildren() const;
  void deleteChild(const String &tag);
};



// **************************************************
//                   class Query
// **************************************************

enum OPERATOR {
  LESS,        // <
  GREATER,     // >
  LESS_EQ,     // <=
  GREATER_EQ,  // >=
  EQUAL,       // =
  NOT_EQUAL,   // !=
  SUBSTRING    // ~
};

class Query 
{
  String tag, strVal;
  float floatVal;
  Vector<String> path;
  OPERATOR op;
  static Regex R;
  void parse(const String &);
  bool testCandidate(Node *);
  bool testValue(Node *);
public:
  Query(const String &);
  Vector<Node*> *search(Node *);
  void search(Node *root,Vector<Node*> &into);
  void deleteResults(Vector<Node*> &results);
};



// **************************************************
//                   class Token
// **************************************************

enum TokenType
{
  TOK_IDENT,          // any-identifier
  TOK_STRING_LIT,     // "any string literal"
  TOK_INT_LIT,        // 123
  TOK_FLOAT_LIT,      // 3.14
  TOK_LEFT_PAREN,     // (
  TOK_RIGHT_PAREN,    // )
  TOK_EOF             // end of file
};
ostream &operator<<(ostream &,TokenType);


class Token
{
  String lexeme;
  TokenType tokenType;
  int lineNum;
public:
  Token(const String &,TokenType,int lineNum);
  const String &getLexeme() const;
  TokenType getTokenType() const;
  int getLineNum() const;
  void printOn(ostream &) const;
  void changeLexeme(const String &);
};

ostream &operator<<(ostream &,const Token &);



// **************************************************
//                  class Scanner
// **************************************************

class Scanner
{
  istream &is;
  int lineNum;
  bool isEofChar(int ic);
  void skipWhiteSpace();
  Token *scanStringLit();
  Token *scanFreeformString(String prefix="");
  void skipComment();
  Token *scanNumber();
  Token *scanIdent();
  Token *scanMinus();
  bool validIdentChar(char);
  void lexicalError(char,int);
public:
  Scanner(istream &);
  Token *nextToken();
};


// **************************************************
//                  class TokenStream
// **************************************************

class TokenStream
{
  Scanner &scanner;
  Token *pushedBack;
public:
  TokenStream(Scanner *);
  TokenStream(istream &);
  ~TokenStream();
  Token *nextToken();
  void pushBack(Token *);
  TokenType peekTokenType();
};


// **************************************************
//                   class Parser
// **************************************************

class Parser
{
  ifstream is;
  TokenStream &tokenStream;
  void match(TokenType);
  bool canBeginSExpression(TokenType);
  Vector<Node*> *ppFile();
  Node *ppSExpression();
  Node *ppLiteral();
  Node *ppCombination();
  void ppExprList(CompositeNode *);
  void syntaxError(Token *);
public:
  Parser(TokenStream *);
  Parser(istream &);
  Parser(const String &filename);
  ~Parser();
  Node *nextElem(); // returns NULL if eof
  bool eof();
  Vector<Node*> *parseAll();
  void close();
};



// **************************************************
//                 class NodeVisitor
// **************************************************

class NodeVisitor
{
public:
  virtual void visit(CompositeNode &) {}
  virtual void visit(NumericNode &) {}
  virtual void visit(StringNode &) {}
};



}

#endif
