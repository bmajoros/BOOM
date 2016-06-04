/***********************************************************************
 GffExon.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "GffTranscript.H"
#include "GffExon.H"
#include "ProteinTrans.H"
using namespace std;
using namespace BOOM;


BOOM::Map<BOOM::String,ExonType> BOOM::GffExon::exonTypeNames;
ExonTypeInitializer ExonTypeInitializer::e;



bool BOOM::isUTR5(ExonType t)
{
  switch(t) {
  case ET_UTR5:// nonspecific
  case ET_INITIAL_UTR5: // TSS to GT
  case ET_INTERNAL_UTR5:// AG to GT
  case ET_FINAL_UTR5:   // AG to ATG
  case ET_SINGLE_UTR5:  // TSS to ATG
    return true;
  }
  return false;
}



bool BOOM::isUTR3(ExonType t)
{
  switch(t) {
  case ET_UTR3:// nonspecific
  case ET_INITIAL_UTR3: // TGA to GT
  case ET_INTERNAL_UTR3:// AG to GT
  case ET_FINAL_UTR3:   // AG to TES
  case ET_SINGLE_UTR3:  // TGA to TES
    return true;
  }
  return false;
}



BOOM::GffExon::GffExon(BOOM::GffFeature &feature,
		       BOOM::GffTranscript &parent)
  : parent(parent)
{
  exonType=exonTypeNames[feature.getFeatureType()];
  begin=feature.getBegin();
  end=feature.getEnd();
  score=feature.getScore();
  hasScore=feature.isScored();
  frame=feature.getFrame();
  hasFrame=feature.isFramed();
}



BOOM::GffExon::GffExon(ExonType type,int begin,int end,
			 BOOM::GffTranscript &parent,bool hasScore,
			 double score,bool hasFrame,int frame)
  : exonType(type), begin(begin), end(end), parent(parent), 
    hasScore(hasScore), score(score), hasFrame(hasFrame), frame(frame)
{
}



GffExon::GffExon(const GffExon &other,GffTranscript &parent)
  : frame(other.frame), begin(other.begin), end(other.end), 
    score(other.score), hasScore(other.hasScore), hasFrame(other.hasFrame),
    parent(parent), exonType(other.exonType), sequence(other.sequence)
{
  // ctor
}



ostream &BOOM::operator<<(ostream &os,ExonType t)
{
  switch(t)
    {
    case ET_EXON:          os<<"exon";            break;
    case ET_INITIAL_EXON:  os<<"initial-exon";    break;
    case ET_INTERNAL_EXON: os<<"internal-exon";   break;
    case ET_FINAL_EXON:    os<<"final-exon";      break;
    case ET_SINGLE_EXON:   os<<"single-exon";     break;

    case ET_UTR:           os<<"UTR";             break;

    case ET_INITIAL_UTR5:  
    case ET_INTERNAL_UTR5: 
    case ET_FINAL_UTR5:    
    case ET_SINGLE_UTR5:   
    case ET_UTR5:          os<<"five_prime_UTR";  break;

    case ET_INITIAL_UTR3:  
    case ET_INTERNAL_UTR3: 
    case ET_FINAL_UTR3:    
    case ET_SINGLE_UTR3:   
    case ET_UTR3:          os<<"three_prime_UTR"; break;

      /*
    case ET_UTR:           os<<"UTR";             break;
    case ET_UTR5:          os<<"UTR5";            break;
    case ET_UTR3:          os<<"UTR3";            break;
    case ET_INITIAL_UTR5:  os<<"initial-UTR5";    break;
    case ET_INTERNAL_UTR5: os<<"internal-UTR5";   break;
    case ET_FINAL_UTR5:    os<<"final-UTR5";      break;
    case ET_SINGLE_UTR5:   os<<"single-UTR5";     break;
    case ET_INITIAL_UTR3:  os<<"initial-UTR3";    break;
    case ET_INTERNAL_UTR3: os<<"internal-UTR3";   break;
    case ET_FINAL_UTR3:    os<<"final-UTR3";      break;
    case ET_SINGLE_UTR3:   os<<"single-UTR3";     break;
      */
    }
  return os;
}



const char *BOOM::toString(ExonType t)
{
  switch(t)
    {
    case ET_EXON:          return "exon";            
    case ET_INITIAL_EXON:  return "initial-exon";    
    case ET_INTERNAL_EXON: return "internal-exon";   
    case ET_FINAL_EXON:    return "final-exon";      
    case ET_SINGLE_EXON:   return "single-exon";     

    case ET_UTR:           return "UTR";            

    case ET_INITIAL_UTR5:  
    case ET_INTERNAL_UTR5: 
    case ET_FINAL_UTR5:    
    case ET_SINGLE_UTR5:   
    case ET_UTR5:          return "five_prime_UTR";

    case ET_INITIAL_UTR3:  
    case ET_INTERNAL_UTR3: 
    case ET_FINAL_UTR3:    
    case ET_SINGLE_UTR3:   
    case ET_UTR3:          return "three_prime_UTR";
    }
  INTERNAL_ERROR;
}



ExonType BOOM::GffExon::getExonType() const
{
  return exonType;
}



ExonTypeInitializer::ExonTypeInitializer() 
{
  BOOM::GffExon::initExonTypeNames();
}



BOOM::GffTranscript &BOOM::GffExon::getParent()
{
  return parent;
}



Strand BOOM::GffExon::getStrand() const
{
  return parent.getStrand();
}



const BOOM::String &BOOM::GffExon::getSource() const
{
  return parent.getSource();
}



const BOOM::String &BOOM::GffExon::getSubstrate() const
{
  return parent.getSubstrate();
}



void BOOM::GffExon::setBegin(int b)
{
  begin=b;
}



void BOOM::GffExon::setEnd(int e)
{
  end=e;
}



int BOOM::GffExon::getBegin()  
{
  return begin;
}



int BOOM::GffExon::getEnd()  
{
  return end;
}



void BOOM::GffExon::initExonTypeNames()
{
  exonTypeNames["exon"]=ET_EXON;
  exonTypeNames["initial-exon"]=  ET_INITIAL_EXON;
  exonTypeNames["internal-exon"]= ET_INTERNAL_EXON;
  exonTypeNames["single-exon"]=   ET_SINGLE_EXON;
  exonTypeNames["final-exon"]=    ET_FINAL_EXON;

  exonTypeNames["UTR"]=ET_UTR;
  exonTypeNames["UTR5"]=ET_UTR5;
  exonTypeNames["UTR3"]=ET_UTR3;
  exonTypeNames["five_prime_UTR"]= ET_UTR5;
  exonTypeNames["three_prime_UTR"]=ET_UTR3;
  exonTypeNames["initial-UTR5"]=   ET_INITIAL_UTR5;
  exonTypeNames["internal-UTR5"]=  ET_INTERNAL_UTR5;
  exonTypeNames["final-UTR5"]=     ET_FINAL_UTR5;
  exonTypeNames["single-UTR5"]=    ET_SINGLE_UTR5;
  exonTypeNames["initial-UTR3"]=   ET_INITIAL_UTR3;
  exonTypeNames["internal-UTR3"]=  ET_INTERNAL_UTR3;
  exonTypeNames["final-UTR3"]=     ET_FINAL_UTR3;
  exonTypeNames["single-UTR3"]=    ET_SINGLE_UTR3;
}



void BOOM::GffExon::toGff(ostream &os)
{
  os << parent.getSubstrate() << "\t"
     << parent.getSource() << "\t"
     << exonType << "\t"
     << begin+1 << "\t"
     << end << "\t";
  if(hasScore) os << score; else os << ".";
  os << "\t" << parent.getStrand() << "\t";
  if(hasFrame) os << frame; else os << ".";
  os << "\ttranscript_id=" << parent.getTranscriptId() 
     << "; gene_id=" << parent.getGeneId()
     << endl;
}



void BOOM::GffExon::changeExonType(ExonType e)
{
  exonType=e;
}



void BOOM::GffExon::loadSequence(BOOM::IndexedFasta &substrate)
{
  if(begin<0 || end>substrate.getSize())
    throw BOOM::String("BOOM::GffExon::loadSequence() : bad coords: ")+
      begin+" "+end;
  substrate.load(begin,end-begin,sequence);
  sequence.toupper();
  if(getStrand()=='-')
    sequence=BOOM::ProteinTrans::reverseComplement(sequence);
}



void BOOM::GffExon::loadSequence(const BOOM::String &substrate)
{
  if(end>substrate.length()) end=substrate.length();
  if(begin<0) begin=0;
  sequence=substrate.substring(begin,end-begin);
  sequence.toupper();
  if(getStrand()=='-')
    sequence=BOOM::ProteinTrans::reverseComplement(sequence);
}



BOOM::String &BOOM::GffExon::getSequence()
{
  return sequence;
}



bool GffExon::hasDonor()
{
  switch(exonType) {
  case ET_INITIAL_EXON:
  case ET_INTERNAL_EXON:
  case ET_INITIAL_UTR5:
  case ET_INTERNAL_UTR5:
  case ET_INITIAL_UTR3:
  case ET_INTERNAL_UTR3:
    return true;
  }
  return false;
}



bool GffExon::hasAcceptor()
{
  switch(exonType) {
  case ET_INTERNAL_EXON:
  case ET_FINAL_EXON:
  case ET_INTERNAL_UTR5:
  case ET_FINAL_UTR5:
  case ET_INTERNAL_UTR3:
  case ET_FINAL_UTR3:
    return true;
  }
  return false;
}



bool GffExon::contains(int coordinate)
{
  return coordinate>=begin && coordinate<end;
}



bool GffExon::identical(const GffExon &other) const
{
  return begin==other.begin && end==other.end;
}



bool GffExon::overlaps(const GffExon &other) const
{
  return begin<other.end && other.begin<end;
}



bool GffExon::overlaps(const Interval &other) const
{
  return begin<other.getEnd() && other.getBegin()<end;
}



void GffExon::reverseComplement(const int L)
{
  begin=L-begin;
  end=L-end;
}


