/***********************************************************************
 ProteinTrans.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "ProteinTrans.H"
#include "DnaAlphabet.H"
#include "AminoAlphabet.H"
using namespace std;



BOOM::Map<const char*,char,BOOM::ProteinTrans::mapstrcmp> 
  BOOM::ProteinTrans::codonMap;
BOOM::ProteinTrans BOOM::ProteinTrans::initializer;



BOOM::ProteinTrans::ProteinTrans()
{
  setupCodonMap();
}



BOOM::String BOOM::ProteinTrans::reverseComplement(const BOOM::String &s)
{
  int length=s.length();
  char *buf=new char[length+1];
  buf[length]='\0';
  for(int i=0,j=length-1 ; i<length ; ++i,--j)
    buf[i]=complement(s[j]);
  BOOM::String r=buf;
  delete [] buf;
  return r;
}



BOOM::Sequence *BOOM::ProteinTrans::reverseComplement(const BOOM::Sequence &s)
{
  s.reverseComplement(DnaAlphabet::global());
}



BOOM::String BOOM::ProteinTrans::translate(const BOOM::String &transcript,
				       int frame)
{
  BOOM::String translation;
  const char *pTranscript=transcript.c_str();

  int length=transcript.length();
  char buf[4];
  buf[3]='\0';
  for(int i=frame ; i+2<length ; i+=3)
    {
      for(int j=0 ; j<3 ; ++j)
	buf[j]=pTranscript[i+j];
      if(!codonMap.isDefined(buf)) translation+="*";
      else
	translation+=codonMap[buf];
    }

  return translation;
}



char BOOM::ProteinTrans::complement(char c)
{
    switch(c)
    {
        case 'N': return 'N';
        case 'R': return 'Y';
        case 'Y': return 'R';
        case 'A': return 'T';
        case 'T': return 'A';
        case 'C': return 'G';
        case 'G': return 'C';
        case ' ': return ' ';
        case '-': return '-';
        default:
            throw BOOM::String("Can't complement base: ")+c;
    }
}



char BOOM::ProteinTrans::mapCodon(const char *codon)
{
  return codonMap.find(codon)==codonMap.end() ? 'X' : codonMap[codon];
}



void BOOM::ProteinTrans::setupCodonMap()
{
  codonMap["ACA"]='T';
  codonMap["GAA"]='E';
  codonMap["CTG"]='L';
  codonMap["TGY"]='C';
  codonMap["GGY"]='G';
  codonMap["GGG"]='G';
  codonMap["CTA"]='L';
  codonMap["AGY"]='S';
  codonMap["TTC"]='F';
  codonMap["GGN"]='G';
  codonMap["GCG"]='A';
  codonMap["GAT"]='D';
  codonMap["TAT"]='Y';
  codonMap["CGT"]='R';
  codonMap["GGT"]='G';
  codonMap["AGC"]='S';
  codonMap["ACG"]='T';
  codonMap["TCT"]='S';
  codonMap["TAA"]='*';
  codonMap["TAC"]='Y';
  codonMap["AGA"]='R';
  codonMap["CGC"]='R';
  codonMap["CAC"]='H';
  codonMap["ATG"]='M';
  codonMap["GTR"]='V';
  codonMap["TCG"]='S';
  codonMap["TAR"]='*';
  codonMap["TAG"]='*';
  codonMap["CAR"]='Q';
  codonMap["CTT"]='L';
  codonMap["ATC"]='I';
  codonMap["GAC"]='D';
  codonMap["TCC"]='S';
  codonMap["CCR"]='P';
  codonMap["AAR"]='K';
  codonMap["GCC"]='A';
  codonMap["GGR"]='G';
  codonMap["CGA"]='R';
  codonMap["TGG"]='W';
  codonMap["CTN"]='L';
  codonMap["GTY"]='V';
  codonMap["CAG"]='Q';
  codonMap["GCR"]='A';
  codonMap["TTG"]='L';
  codonMap["CGG"]='R';
  codonMap["GAY"]='D';
  codonMap["TTY"]='F';
  codonMap["AGT"]='S';
  codonMap["TAY"]='Y';
  codonMap["ATT"]='I';
  codonMap["TGC"]='C';
  codonMap["CCY"]='P';
  codonMap["TAG"]='*';
  codonMap["GTA"]='V';
  codonMap["AAA"]='K';
  codonMap["CCT"]='P';
  codonMap["CCG"]='P';
  codonMap["GCN"]='A';
  codonMap["AGG"]='R';
  codonMap["GCT"]='A';
  codonMap["GGA"]='G';
  codonMap["AAC"]='N';
  codonMap["CGN"]='R';
  codonMap["GAG"]='E';
  codonMap["CAY"]='H';
  codonMap["AAY"]='N';
  codonMap["CAT"]='H';
  codonMap["CCC"]='P';
  codonMap["GTN"]='V';
  codonMap["GTG"]='V';
  codonMap["AAG"]='K';
  codonMap["GGC"]='G';
  codonMap["ATA"]='I';
  codonMap["GAR"]='E';
  codonMap["TTA"]='L';
  codonMap["CCN"]='P';
  codonMap["GCY"]='A';
  codonMap["CAA"]='Q';
  codonMap["TGT"]='C';
  codonMap["AAT"]='N';
  codonMap["YTR"]='L';
  codonMap["ACT"]='T';
  codonMap["TTR"]='L';
  codonMap["GCA"]='A';
  codonMap["TCA"]='S';
  codonMap["CCA"]='P';
  codonMap["AGY"]='R';
  codonMap["TGA"]='*';
  codonMap["TTT"]='F';
  codonMap["ACC"]='T';
  codonMap["GTT"]='V';
  codonMap["GTC"]='V';
  codonMap["TCN"]='S';
  codonMap["ATH"]='I';
  codonMap["CTC"]='L';
  codonMap["ACN"]='T';
}



Sequence *BOOM::ProteinTrans::translate(const Sequence &source,
				      Alphabet &dnaAlphabet,
				      Alphabet &proteinAlphabet,
				      int frame)
{
  BOOM::String *sourceString=source.toString(dnaAlphabet);
  BOOM::String destString=translate(*sourceString,frame);
  delete sourceString;
  return new Sequence(destString,proteinAlphabet);
}



Sequence *BOOM::ProteinTrans::translate(const Sequence &source,
				      int frame)
{
  BOOM::String *sourceString=source.toString(DnaAlphabet::global());
  BOOM::String destString=translate(*sourceString,frame);
  delete sourceString;
  return new Sequence(destString,AminoAlphabet::global());
}



void BOOM::ProteinTrans::translate(const Sequence &source,
				 Alphabet &dnaAlphabet,
				 Alphabet &proteinAlphabet,
				 Sequence &dest,
				 int frame)
{
  BOOM::String *sourceString=source.toString(dnaAlphabet);
  BOOM::String destString=translate(*sourceString,frame);
  delete sourceString;
  dest.copyFrom(destString,proteinAlphabet);
}



void BOOM::ProteinTrans::translate(const Sequence &source,
				 Sequence &dest,
				 int frame)
{
  BOOM::String *sourceString=source.toString(DnaAlphabet::global());
  BOOM::String destString=translate(*sourceString,frame);
  delete sourceString;
  dest.copyFrom(destString,AminoAlphabet::global());
}


