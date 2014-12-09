/****************************************************************
 LabeledMatrixLoader.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "LabeledMatrixLoader.H"
#include "../Constants.H"
#include "../File.H"
using namespace std;
using BOOM::Regex;
using BOOM::String;
using BOOM::File;
using namespace GSL;


Regex LabeledMatrixLoader::commentRegex("^\\s*#");
Regex LabeledMatrixLoader::headerRegex("^[A-Z \\t\\n\\r*]+$");
Regex LabeledMatrixLoader::tableRegex(
                               "^\\s*[A-Z*]\\s+[. \\t\\r\\n01-9e\\+-]+$");
Regex LabeledMatrixLoader::errorRegex("\\S");



void LabeledMatrixLoader::load(const Alphabet &alphabet,
			       const AlphabetMap &alphabetMap,
			       const String &filename,
			       GSL::Matrix &M,double defaultValue)
{
  M.setAllTo(defaultValue);
  int n=alphabetMap.getRangeSize();
  if(M.getNumRows()!=n || M.getNumColumns()!=n)
    M.resize(n,n);
  BOOM::Vector<Symbol> headerFields;
  File file(filename);
  while(!file.eof())
    {
      String line=file.getline();
      if(file.eof()) break;
      if(commentRegex.search(line)) continue;
      
      if(headerRegex.search(line))
	{
	  BOOM::Vector<String> &fields=*line.getFields();
	  int n=fields.size();
	  for(int i=0 ; i<n ; ++i)
	    {
	      char headerChar=fields[i][0];
	      Symbol s=INVALID_SYMBOL;
	      if(alphabet.isDefined(headerChar))
		 s=alphabet.lookup(headerChar);
	      headerFields.push_back(s);
	    }
	  delete &fields;
	}
      else if(tableRegex.search(line))
	{
	  BOOM::Vector<String> &fields=*line.getFields();
	  char cRowSymbol=fields[0][0];
	  if(alphabet.isDefined(cRowSymbol))
	    {
	      Symbol rowSymbol=alphabet.lookup(cRowSymbol);
	      int n=fields.size();
	      if(n!=headerFields.size()+1)
		throw String("Wrong number of entries on row \"")+line+
		  "\" of matrix "+filename;
	      for(int i=1 ; i<n ; ++i)
		{
		  Symbol columnSymbol=headerFields[i-1];
		  if(columnSymbol!=INVALID_SYMBOL)
		    M(alphabetMap(rowSymbol),alphabetMap(columnSymbol))=
		      fields[i].asDouble();
		}
	    }
	  delete &fields;
	}
      else if(errorRegex.search(line))
	throw String("Error in matrix file ")+filename+":\n"+line;
    }
}



GSL::Matrix *LabeledMatrixLoader::load(const Alphabet &alphabet,
				       const AlphabetMap &alphabetMap,
				       const String &filename,
				       double defaultValue)
{
  int n=alphabetMap.getRangeSize();
  GSL::Matrix *M=new GSL::Matrix(n,n);
  M->setAllTo(defaultValue);
  load(alphabet,alphabetMap,filename,*M);
  return M;
}




