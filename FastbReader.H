/****************************************************************
 FastbReader.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.

 FastbReader : reads FASTB files

 A FASTAB file is a concatenation of FASTA and FASTC files.  The
 sections starting with a >-type defline are FASTA sequences 
 (alphabetic data like DNA or proteins), and the sections starging 
 with a %-type defline are FASTC sequences (continuous data).

 This is open-source software, governed by the ARTISTIC LICENSE 
 (see www.opensource.org).
 ****************************************************************/
#ifndef INCL_FastbReader_H
#define INCL_FastbReader_H
#include <iostream>
#include "File.H"
#include "String.H"
#include "Vector.H"
#include "Regex.H"
using namespace std;

namespace BOOM {

// ---------------------------------------------------------------

enum FastbSeqType { 
  FASTB_UNKNOWN,
  FASTB_DISCRETE,
  FASTB_CONTINUOUS
};

// ---------------------------------------------------------------

struct FastbSequence { 
  FastbSequence(FastbSeqType,const String &defline,const String &ID);
  FastbSeqType getType() const;
  bool isDiscrete() const;
  bool isContinuous() const;
  const String &getDefline() const;
  const String &getID() const;
  virtual void printOn(ostream &) const=0;
protected:
  FastbSeqType type;
  String defline, ID;
};

ostream &operator<<(ostream &,const FastbSequence &);

// ---------------------------------------------------------------

struct FastbDiscreteSeq : public FastbSequence { 
  FastbDiscreteSeq(const String &,const String &defline,const String &ID);
  const String &getSeq() const;
  virtual void printOn(ostream &) const;
protected:
  String seq;
};

// ---------------------------------------------------------------

struct FastbContinuousSeq : public FastbSequence { 
  FastbContinuousSeq(const DoubleVector &,const String &defline,
		     const String &ID);
  const DoubleVector &getSeq() const;
  DoubleVector &getSeq() {return seq;}
  virtual void printOn(ostream &) const;
protected:
  DoubleVector seq;
};

// ---------------------------------------------------------------

typedef Vector< pair<String,String> > StringPairVector;

// ---------------------------------------------------------------

class FastbReader { 
public:
  FastbReader();
  FastbReader(const String &filename);
  void open(const String &filename);
  FastbSequence *nextSequence();
  bool eof();
  static StringPairVector *parseDefline(const String &defline,String &ID);
protected:
  static Regex discreteRegex, continuousRegex, keyValueRegex;
  File file;
  String nextDefline;
};

}
#endif

