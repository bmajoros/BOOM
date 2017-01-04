/*
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 */
#include "Essex.H"
#include <ctype.h>
#include "ElasVector.H"

using namespace std;
using namespace BOOM;
//using namespace Essex;
namespace Essex {

int TAB_SIZE=3;
Regex Query::R("(\\S+)([<>=!~]+)(\\S+)");

// **************************************************
//                   Query methods
// **************************************************

Query::Query(const String &q)
{
  parse(q);
}



void Query::parse(const String &q)
{
  Vector<BOOM::String> &fields=*q.getFields("/");
  int n=fields.size();
  if(n<1) throw "error in Essex::Query";
  tag=fields[0];
  for(int i=1 ; i<n-1 ; ++i)
    path.push_back(fields[i]);
  String predicate=fields[n-1];
  if(!R.match(predicate)) throw "Syntax error in Essex query";
  path.push_back(R[1]);
  String opStr=R[2];
  strVal=R[3];
  floatVal=strVal.asFloat();
  if(opStr=="<") op=LESS;
  else if(opStr=="<=") op=LESS_EQ;
  else if(opStr==">") op=GREATER;
  else if(opStr==">=") op=GREATER_EQ;
  else if(opStr=="=") op=EQUAL;
  else if(opStr=="!=") op=NOT_EQUAL;
  else if(opStr=="~") op=SUBSTRING;
  else throw "Invalid operator in Essex query";

  delete &fields;
}



void Query::deleteResults(Vector<Node*> &results)
{
  for(Vector<Node*>::iterator cur=results.begin(), end=results.end() ;
      cur!=end ; ++cur) delete *cur;
}



void Query::search(Node *root,Vector<Node*> &results)
{
  if(!results.empty()) throw "Query::search(): results is not empty";
  Vector<Node*> candidates;
  root->findDescendents(tag,candidates);
  if(root->getNodeType()==COMPOSITE &&
     static_cast<CompositeNode*>(root)->getTag()==tag) 
    candidates.push_back(root);
  for(Vector<Node*>::iterator cur=candidates.begin(), end=candidates.end() ;
      cur!=end ; ++cur) {
    Node *candidate=*cur;
    if(testCandidate(candidate)) results.push_back(candidate);
  }
}



Vector<Node*> *Query::search(Node *root)
{
  Vector<Node*> &results=*new Vector<Node*>;
  search(root,results);
  return &results;
}



bool Query::testCandidate(Node *node)
{
  int n=path.size();
  for(int i=0 ; i<n ; ++i) {
    node=node->findChild(path[i]);
    if(!node) return false;
  }
  node=static_cast<CompositeNode*>(node)->getIthChild(0);
  return testValue(node);
}



bool Query::testValue(Node *node)
{
  NodeType nodeType=node->getNodeType();
  if(nodeType==STRING) {
    StringNode *strNode=static_cast<StringNode*>(node);
    const String &s=strNode->getValue();
    switch(op)
      {
      case EQUAL:      return s==strVal;
      case NOT_EQUAL:  return s!=strVal;
      case SUBSTRING:  return s.contains(strVal);
      }
  }
  else {
    NumericNode *nNode=static_cast<NumericNode*>(node);
    float num=nNode->getValue();
    switch(op)
      {
      case LESS:        return num<floatVal;
      case GREATER:     return num>floatVal;
      case LESS_EQ:     return num<=floatVal;
      case GREATER_EQ:  return num>=floatVal;
      case EQUAL:       return num==floatVal;
      case NOT_EQUAL:   return num!=floatVal;
      }
  }
 throw "Internal error";
}



// **************************************************
//                   Node methods
// **************************************************

ostream &operator<<(ostream &os,const Node &node)
{
  node.printOn(os);
  return os;
}



Node::Node(NodeType nodeType)
  : nodeType(nodeType)
{
  // ctor
}



Node::~Node()
{
  // dtor
}



NodeType Node::getNodeType() const
{
  return nodeType;
}



Node *Node::findChild(const String &tag)
{
  return NULL;
}



void Node::findChildren(const String &,Vector<Node*> &)
{
  return;
}



void Node::findDescendents(const String &,Vector<Node*> &)
{
  return;
}



String Node::getAttribute(const String &tag)
{
  return "";
}



NumericNode::NumericNode(float value)
  : Node(NUMBER), value(value)
{
  // ctor
}



float NumericNode::getValue() const
{
  return value;
}



void NumericNode::acceptVisitor(NodeVisitor &v)
{
  v.visit(*this);
}



void NumericNode::printOn(ostream &os) const
{
  if(float(int(value))==value) os<<int(value);
  else os<<value;
}



void NumericNode::printXML(ostream &os) const
{
  printOn(os);
}



StringNode::StringNode(const String &value)
  : Node(STRING), value(value)
{
  // ctor
}



const String &StringNode::getValue() const
{
  return value;
}



String &StringNode::getValue()
{
  return value;
}



void StringNode::acceptVisitor(NodeVisitor &v)
{
  v.visit(*this);
}



void StringNode::printXML(ostream &os) const
{
  String escaped=value.substitute("&","&amp;");
  escaped=escaped.substitute("\"","&quot;");
  escaped=escaped.substitute("'","&apos;");
  escaped=escaped.substitute("<","&lt;");
  escaped=escaped.substitute(">","&gt;");
  os<<escaped;
}



void StringNode::printOn(ostream &os) const
{
  String escaped=value.substitute("\"","\\\"");
  bool needsQuotes=false;
  const char *cstr=value.c_str();
  for(; *cstr ; ++cstr)
    if(isspace(*cstr)) needsQuotes=true;
  if(needsQuotes) os<<'"'<<escaped<<'"';
  else {
    escaped=escaped.substitute("(","&lparen;");
    escaped=escaped.substitute(")","&rparen;");
    os<<escaped;
  }	
}



CompositeNode::CompositeNode(const String &tag)
  : Node(COMPOSITE), tag(tag)
{
  // ctor
}



CompositeNode::~CompositeNode()
{
  for(Vector<Node*>::iterator cur=children.begin(), end=children.end() ;
      cur!=end ; ++cur) delete *cur;
}



void CompositeNode::appendChild(Node *child)
{
  if(child) children.push_back(child);
}



void CompositeNode::append(int x)
{
  appendChild(new NumericNode(float(x)));
}



void CompositeNode::append(float x)
{
  appendChild(new NumericNode(x));
}



void CompositeNode::append(const String &s)
{
  appendChild(new StringNode(s));
}



void CompositeNode::prepend(const String &s)
{
  children.push_front(new StringNode(s));
}



void CompositeNode::append(const String &tag,int value)
{
  CompositeNode *node=new CompositeNode(tag);
  node->append(value);
  children.push_back(node);
}



void CompositeNode::append(const String &tag,float value)
{
  CompositeNode *node=new CompositeNode(tag);
  node->append(value);
  children.push_back(node);
}



void CompositeNode::append(const String &tag,const String &value)
{
  CompositeNode *node=new CompositeNode(tag);
  node->append(value);
  children.push_back(node);
}



void CompositeNode::append(const String &tag,const char *value)
{
  CompositeNode *node=new CompositeNode(tag);
  node->append(value);
  children.push_back(node);
}



const String &CompositeNode::getTag() const
{
  return tag;
}



String &CompositeNode::getTag()
{
  return tag;
}



int CompositeNode::getNumChildren() const
{
  return children.size();
}



Node *CompositeNode::getIthChild(int i) const
{
  return children[i];
}



void CompositeNode::setIthChild(int i,Node *child)
{
  children[i]=child;
}



void CompositeNode::acceptVisitor(NodeVisitor &v)
{
  int n=children.size();
  for(int i=0 ; i<n ; ++i)
    children[i]->acceptVisitor(v);
  v.visit(*this);
}



Node *CompositeNode::findChild(const String &tag)
{
  int n=getNumChildren();
  for(int i=0 ; i<n ; ++i) {
    Node *child=getIthChild(i);
    if(child->getNodeType()==COMPOSITE &&
       static_cast<CompositeNode*>(child)->getTag()==tag)
      return child;
  }
  return NULL;
}



void CompositeNode::findChildren(const String &tag,Vector<Node*> &v)
{
  int n=getNumChildren();
  for(int i=0 ; i<n ; ++i) {
    Node *child=getIthChild(i);
    if(child->getNodeType()==COMPOSITE &&
       static_cast<CompositeNode*>(child)->getTag()==tag)
      v.push_back(child);
  }
}



void CompositeNode::findDescendents(const String &tag,Vector<Node*> &v)
{
  int n=getNumChildren();
  for(int i=0 ; i<n ; ++i) {
    Node *child=getIthChild(i);
    if(child->getNodeType()==COMPOSITE) {
      CompositeNode *cNode=static_cast<CompositeNode*>(child);
      if(cNode->getTag()==tag) v.push_back(child);
      cNode->findDescendents(tag,v);
    }
  }
}



String CompositeNode::getAttribute(const String &tag)
{
  Node *child=findChild(tag);
  if(!child) return "";
  child=static_cast<CompositeNode*>(child)->getIthChild(0);
  String attr;
  switch(child->getNodeType()) 
    {
    case COMPOSITE:
      throw "Can't convert composite attribute in Essex structure";
    case NUMBER:
      attr=String(static_cast<StringNode*>(child)->getValue());
      break;
    case STRING:
      attr=static_cast<StringNode*>(child)->getValue();
      break;
    }
  return attr;
}



void CompositeNode::printXML(ostream &os) const
{
  printXMLrecursive(os,0);
}



void CompositeNode::printOn(ostream &os) const
{
  printRecursive(os,0);
}



bool CompositeNode::hasCompositeChildren() const
{
  for(Vector<Node*>::const_iterator cur=children.begin(), end=children.end() ;
      cur!=end ; ++cur)
    if((*cur)->getNodeType()==COMPOSITE) return true;
  return false;
}



void CompositeNode::printRecursive(ostream &os,int depth) const
{
  String tab;
  tab.padOrTruncate(TAB_SIZE*depth);
  os<<tab<<'('<<tag;
  int n=getNumChildren();
  if(hasCompositeChildren()) {
    for(int i=0 ; i<n ; ++i) {
      Node *child=getIthChild(i);
      if(child->getNodeType()==COMPOSITE) {
	os<<"\n";
	static_cast<CompositeNode*>(child)->printRecursive(os,depth+1);
      }
      else {
	String tab;
	tab.padOrTruncate(TAB_SIZE*(depth+1));
	os<<"\n"<<tab<<*child;
      }
    }
  }
  else for(int i=0 ; i<n ; ++i) os<<" "<<*getIthChild(i);
  os<<')';//<<endl;
}



void CompositeNode::printXMLrecursive(ostream &os,int depth) const
{
  String tab;
  tab.padOrTruncate(TAB_SIZE*depth);
  os<<tab<<'<'<<tag<<'>';
  int n=getNumChildren();
  if(hasCompositeChildren()) {
    os<<endl;
    for(int i=0 ; i<n ; ++i) {
      Node *child=getIthChild(i);
      if(child->getNodeType()==COMPOSITE) 
	static_cast<CompositeNode*>(child)->printXMLrecursive(os,depth+1);
      else {
	String tab;
	tab.padOrTruncate(TAB_SIZE*(depth+1));
	os<<tab; child->printXML(os); os<<endl;
      }
    }
    os<<tab;
  }
  else {
    for(int i=0 ; i<n ; ++i) {
      getIthChild(i)->printXML(os);
      if(i+1<n) os<<" ";
    }
  }
  os<<"</"<<tag<<'>'<<endl;
}



void CompositeNode::deleteChild(const String &tag)
{
  int n=getNumChildren();
  for(int i=0 ; i<n ; ++i) {
    Node *child=getIthChild(i);
    if(child->getNodeType()==COMPOSITE &&
       static_cast<CompositeNode*>(child)->getTag()==tag) {
      children.cut(i);
      delete child;
    }
  }
}



// **************************************************
//                   Token methods
// **************************************************

ostream &operator<<(ostream &os,TokenType tokenType)
{
  switch(tokenType)
    {
    case TOK_IDENT:      // any-identifier
      os << "identifier";
      break;
    case TOK_STRING_LIT:     // "any string literal"
      os << "string literal";
      break;
    case TOK_INT_LIT:        // 123
      os << "number";
      break;
    case TOK_FLOAT_LIT:      // 3.14
      os << "number";
      break;
    case TOK_LEFT_PAREN:     // (
      os << "'('";
      break;
    case TOK_RIGHT_PAREN:     // )
      os << "')'";
      break;
    case TOK_EOF: // end of file
      os << "end of file";
      break;
    }
}



ostream &operator<<(ostream &os,const Token &token)
{
  token.printOn(os);
  return os;
}



Token::Token(const String &lexeme,TokenType tokenType,
	     int lineNum)
  : lexeme(lexeme), tokenType(tokenType), lineNum(lineNum)

{
  // ctor
}



const String &Token::getLexeme() const
{
  return lexeme;
}



TokenType Token::getTokenType() const
{
  return tokenType;
}



int Token::getLineNum() const
{
  return lineNum;
}



void Token::printOn(ostream &os) const
{
  os<<tokenType<< "\t" << lexeme;
}




void Token::changeLexeme(const String &newValue)
{
  lexeme=newValue;
}



// **************************************************
//              TokenStream methods
// **************************************************

TokenStream::TokenStream(Scanner *scanner)
  : scanner(*scanner), pushedBack(NULL)
{
  // ctor
}



TokenStream::TokenStream(istream &is)
  : scanner(*new Scanner(is)), pushedBack(NULL)
{
  // ctor
}



TokenStream::~TokenStream()
{
  // dtor

  delete &scanner;
}



TokenType TokenStream::peekTokenType()
{
  if(!pushedBack)
    pushedBack=scanner.nextToken();
  return pushedBack->getTokenType();
}



Token *TokenStream::nextToken()
{
  Token *retval;
  if(pushedBack) {
    retval=pushedBack;
    pushedBack=NULL;
  }
  else retval=scanner.nextToken();
  return retval;
}



void TokenStream::pushBack(Token *token)
{
  if(pushedBack)
    throw "Pushing back more than one token in Essex::TokenStream";
  pushedBack=token;
}




// **************************************************
//                  Scanner methods
// **************************************************

Scanner::Scanner(istream &is)
  : is(is), lineNum(1)
{
  // ctor
}


void Scanner::skipWhiteSpace()
{
  // This method skips whitespace and keeps count of newlines
  // in the process
  
  int ic;
  char c=' ';
  while(isspace(c)) {
    ic=is.get();
    if(isEofChar(ic)) return;
    c=char(ic);
    if(c=='#') {
      do {
	ic=is.get();
	if(isEofChar(ic)) return;
	c=char(ic);
      } while(c!='\r' && c!='\n');
    }
    if(c=='\r' || c=='\n') ++lineNum;
  }
  is.putback(c);
}



bool Scanner::isEofChar(int ic) 
{ 
  return ic==EOF || ic==0; 
}



Token *Scanner::nextToken()
{
  skipWhiteSpace();
  while(!is.eof()) {
    int ic=is.get();
    if(isEofChar(ic)) break;
    char c=static_cast<char>(ic);
    switch(c)
      {
      case '\"': // string literal
	return scanStringLit();
      case '-': // - or negative number
	return scanMinus();
      case '#': // comment
	skipComment();
	skipWhiteSpace();
	continue;
      case '(': // (
	return new Token("(",TOK_LEFT_PAREN,lineNum);
      case ')': // )
	return new Token(")",TOK_RIGHT_PAREN,lineNum);
      default:
	if(isdigit(c)) {
	  is.putback(c);
	  return scanNumber();
	}
	else if(isalpha(c) || c=='_') {
	  is.putback(c);
	  return scanIdent();
	}
	is.putback(c);
	return scanFreeformString();
      }
  }
  return new Token("",TOK_EOF,lineNum);
}



Token *Scanner::scanMinus()
{
  // Precondition: a '-' has already been read

  int ic=is.get();
  char c=static_cast<char>(ic);
  if(isEofChar(ic)) return new Token("-",TOK_STRING_LIT,lineNum);
  if(isdigit(c))  {
    is.putback(c);
    Token *token=scanNumber();
    const String &lexeme=token->getLexeme();
    if(lexeme.contains('.')) { // float
      float value=lexeme.asFloat();
      value *= -1;
      token->changeLexeme(String(value));
    }
    else { // int
      int value=lexeme.asInt();
      value *= -1;
      token->changeLexeme(String((long)value));
    }
    return token;
  }
  else {
    is.putback(c);
    return scanFreeformString("-");
  }
}



Token *Scanner::scanNumber() { 
  char buffer[256];
  int i=0;
  bool periodSeen=false;
  while(!is.eof() && i<255) {
    int ic=is.get();
    char c=static_cast<char>(ic);
    if(isEofChar(ic)) break;
    if(!(isdigit(c) || c=='.'))	{
      is.putback(c);
      break;
    }
    buffer[i]=c;
    ++i;
    if(c=='.')
      if(periodSeen)
	lexicalError(c,lineNum);
      else
	periodSeen=true;
  }
  buffer[i]='\0';
  TokenType tokenType=periodSeen ? TOK_FLOAT_LIT : TOK_INT_LIT;
  return new Token(buffer,tokenType,lineNum);
}



void Scanner::lexicalError(char c,int lineNum)
{
  throw String("Lexical error in Essex file, on line number ")+lineNum;
}



bool Scanner::validIdentChar(char c)
{
  return isalpha(c) || isdigit(c) || c=='_' || c=='-' || c=='?';
}



Token *Scanner::scanIdent()
{
  char buffer[256];
  int i=0;
  while(!is.eof() && i<255) {
    int ic=is.get();
    char c=static_cast<char>(ic);
    if(isEofChar(ic)) break;
    if(!validIdentChar(c)) {
      is.putback(c);
      break;
    }
    buffer[i]=c;
    ++i;
  }
  buffer[i]='\0';
  return new Token(buffer,TOK_IDENT,lineNum);
}



Token *Scanner::scanStringLit()
{
  // Precondition: A '\"' has already been read

  ElasCharVect array;
  int i=0;

  // Read string literal into elastic array
  while(!is.eof()) {
    int ic=is.get();
    char c=static_cast<char>(ic);
    if(isEofChar(ic)) lexicalError(c,lineNum);
    if(c=='\"') break;
    if(c=='\\')	{
      ic=is.get();
      c=static_cast<char>(ic);
      if(isEofChar(ic)) lexicalError(c,lineNum);
      switch(c)
	{
	case 'n':
	  c='\n';
	  break;
	case 'r':
	  c='\r';
	  break;
	case 't':
	  c='\t';
	  break;
	}
    }
    array[i]=c;
    ++i;
  }
  const int len=i;

  // Copy elastic array into a char string
  char *buffer=new char[len+1];
  for(i=0 ; i<len ; ++i) buffer[i]=array[i];
  buffer[i]='\0';

  // Create token
  String lexeme=buffer;
  lexeme=lexeme.substitute("&lparen;","(");
  lexeme=lexeme.substitute("&rparen;",")");
  lexeme=lexeme.substitute("&tab;","\t");
  lexeme=lexeme.substitute("&space;"," ");
  Token *token=new Token(lexeme,TOK_STRING_LIT,lineNum);
  
  // Clean up and return
  delete [] buffer;
  return token;
}



void Scanner::skipComment()
{
  // Precondition: A '#' has already been read

  while(!is.eof()) {
    int ic=is.get();
    char c=static_cast<char>(ic);
    if(isEofChar(ic) || c=='\n') break;
  }
  ++lineNum;
}



Token *Scanner::scanFreeformString(String prefix)
{
  ElasCharVect array;
  int i=0;

  // Read string literal into elastic array
  while(!is.eof()) {
    int ic=is.get();
    char c=static_cast<char>(ic);
    if(isEofChar(ic)) break;
    if(isspace(c) || c==')' || c=='(') {is.putback(c);break;}
    if(c=='\\')	{
      ic=is.get();
      c=static_cast<char>(ic);
      if(isEofChar(ic)) lexicalError(c,lineNum);
      switch(c)
	{
	case 'n':
	  c='\n';
	  break;
	case 'r':
	  c='\r';
	  break;
	case 't':
	  c='\t';
	  break;
	}
    }
    array[i]=c;
    ++i;
  }
  const int len=i;

  // Copy elastic array into a char string
  char *buffer=new char[len+1];
  for(i=0 ; i<len ; ++i) buffer[i]=array[i];
  buffer[i]='\0';

  // Create token
  String lexeme=prefix+buffer;
  lexeme=lexeme.substitute("&lparen;","(");
  lexeme=lexeme.substitute("&rparen;",")");
  lexeme=lexeme.substitute("&tab;","\t");
  lexeme=lexeme.substitute("&space;"," ");
  Token *token=new Token(lexeme,TOK_STRING_LIT,lineNum);
  
  // Clean up and return
  delete [] buffer;
  return token;
}




// **************************************************
//                  Parser methods
// **************************************************

Parser::Parser(TokenStream *tokenStream)
  : tokenStream(*tokenStream)
{
  // ctor

  //is=tokenStream.getScanner().getIStream();
}



Parser::Parser(istream &is)
  : tokenStream(*new TokenStream(is))
{
  // ctor
}



Parser::Parser(const String &filename)
  : tokenStream(*new TokenStream(is))
{
  is.open(filename.c_str());
}


Parser::~Parser()
{
  // dtor

  delete &tokenStream;
}



Vector<Node*> *Parser::parseAll()
{
  return ppFile();
}



void Parser::match(TokenType tokenType)
{
  Token *token=tokenStream.nextToken();
  if(token->getTokenType()!=tokenType) {
    cerr<<"Expecting "<<tokenType<<endl;
    syntaxError(token);
  }
  delete token;
}



void Parser::syntaxError(Token *token)
{
  throw String("Syntax error in Essex file, near \"")+
    token->getLexeme()+"\"";
}



Vector<Node*> *Parser::ppFile()
{
  // First={TOK_LEFT_PAREN,TOK_IDENT,TOK_INT_LIT,TOK_FLOAT_LIT,
  //        TOK_STRING_LIT,}

  //File ::= SExpression File
  //File ::= 

  Vector<Node*> *forest=new Vector<Node*>;
  while(tokenStream.peekTokenType()!=TOK_EOF)
    forest->push_back(ppSExpression());
  return forest;
}



Node *Parser::ppSExpression()
{
  // First={TOK_LEFT_PAREN,TOK_IDENT,TOK_INT_LIT,TOK_FLOAT_LIT,
  //        TOK_STRING_LIT,}

  //SExpression ::= Combination 
  //SExpression ::= Literal 

  switch(tokenStream.peekTokenType())
    {
    case TOK_LEFT_PAREN:
      return ppCombination();
    case TOK_EOF:
      return NULL;
    default:
      return ppLiteral();
    }
}



bool Parser::canBeginSExpression(TokenType tokenType)
{
  switch(tokenType)
    {
    case TOK_LEFT_PAREN:
    case TOK_IDENT:
    case TOK_INT_LIT:
    case TOK_FLOAT_LIT:
    case TOK_STRING_LIT:
      return true;
    }
  return false;
}




Node *Parser::ppLiteral()
{
  // First={TOK_INT_LIT,TOK_FLOAT_LIT,TOK_STRING_LIT}

  //Literal ::= INT_LIT 
  //Literal ::= FLOAT_LIT 
  //Literal ::= STRING_LIT 
  //Literal ::= IDENT

  switch(tokenStream.peekTokenType())
    {
    case TOK_INT_LIT:
    case TOK_FLOAT_LIT:
      {
	Token *token=tokenStream.nextToken();
	Node *node=new NumericNode(token->getLexeme().asFloat());
	delete token;
	return node;
      }
    case TOK_STRING_LIT:
    case TOK_IDENT:
      {
	Token *token=tokenStream.nextToken();
	Node *node=new StringNode(token->getLexeme());
	delete token;
	return node;
      }
    default:
      throw "Internal error (Essex::Parser)";
    }
}



Node *Parser::ppCombination()
{
  // First={TOK_LEFT_PAREN}

  //Combination ::= TOK_LEFT_PAREN  ExprList  TOK_RIGHT_PAREN

  match(TOK_LEFT_PAREN);
  Token *token=tokenStream.nextToken();
  if(token->getTokenType()!=TOK_IDENT) syntaxError(token);
  String tag=token->getLexeme();
  delete token;
  CompositeNode *combo=new CompositeNode(tag);
  ppExprList(combo);
  match(TOK_RIGHT_PAREN);
  return combo;
}



void Parser::ppExprList(CompositeNode *combo)
{
  // First={TOK_LEFT_PAREN,TOK_IDENT,TOK_INT_LIT,TOK_FLOAT_LIT,
  //        TOK_STRING_LIT,}

  //ExprList ::= SExpression  ExprList
  //ExprList ::= 

  while(canBeginSExpression(tokenStream.peekTokenType()))
    combo->appendChild(ppSExpression());
}



Node *Parser::nextElem()
{
  return ppSExpression();
}



bool Parser::eof()
{
  return is.eof();
}



void Parser::close()
{
  is.close();
}


}
