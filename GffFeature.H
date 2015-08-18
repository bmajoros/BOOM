/***********************************************************************
 GffFeature.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_GffFeature_H
#define INCL_BOOM_GffFeature_H
#include "Vector.H"
#include "String.H"
#include "Regex.H"
#include "Comparator.H"
using namespace std;

namespace BOOM {
class GffFeature {
public:
  GffFeature(const String &rawLine,
	     const String &substrate,
	     const String &source,
	     const String &featureType,
	     int begin,int end,double score,bool hasScore,char strand,
	     int frame,bool hasFrame);
  GffFeature(const GffFeature &);
  GffFeature(const String &rawLine);
  String toGff() const;
  Vector<String> &getExtraFields();
  void getExtraKeys(Set<String> &into);
  String lookupExtra(const String &key);
  void setExtra(const String &key,const String &value);
  bool isExtraDefined(const String &key);
  bool hasExtraFields() const;
  bool isFramed() const;  
  bool isScored() const;  
  bool isStranded() const;  
  char getStrand() const;  
  void setStrand(char s) {strand=s;}
  const String &getFeatureType() const;
  void setFeatureType(const String &t) {featureType=t;}
  const String &getRawField(int index);
  const String &getRawLine() const;
  const String &getSource() const;
  const String &getSubstrate() const;
  void setSubstrate(const String &s) {substrate=s;}
  double getScore() const;
  int getBegin() const;  // 0-based, always less than end
  int getEnd() const;    // 0-based, always greater than begin
  void setBegin(int b) {begin=b;}
  void setEnd(int e) {end=e;}
  int getFrame() const;  
  void setFrame(int f) {frame=f;}
  int getTranscriptId() const;
  int overlapBases(const GffFeature &other) const;
  int length() const;
  int getLength() const { return length(); }
  void shiftCoords(int);
private:
  int frame;  
  int begin, end;  
  double score;
  char strand;  
  bool hasScore;  
  bool hasFrame;  
  Vector<String> extraFields, allFields;
  Map<String,String> extraMap;
  String substrate;  
  String source;  
  String rawLine;  
  String featureType;  
  static Regex transgrpRegex, assignRegex, pairRegex;
  void parseLine(const String &);
};


class GffFeatureComparator : public Comparator<GffFeature*>
{
public:
  bool less(GffFeature *&,GffFeature *&);
  bool greater(GffFeature *&,GffFeature *&); 
  bool equal(GffFeature *&,GffFeature *&);
};

}
using namespace BOOM;

ostream &operator<<(ostream&,GffFeature&);

#endif
