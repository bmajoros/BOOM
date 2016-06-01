/****************************************************************
 VcfReader.C
 Copyright (C)2016 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "VcfReader.H"
#include "Pipe.H"
using namespace std;
using namespace BOOM;


/****************************************************************
                            Variant
 ****************************************************************/

Variant::Variant(const String &id,const String &chr,int pos)
  : id(id), chr(chr), pos(pos)
{
}



void Variant::addAllele(const String &allele)
{
  alleles.push_back(allele);
}



int Variant::numAlleles() const
{
  return alleles.size();
}



const String &Variant::getAllele(int i) const
{
  return alleles[i];
}



const String &Variant::getID() const
{
  return id;
}



const String &Variant::getChr() const
{
  return chr;
}



int Variant::getPos() const
{
  return pos;
}



/****************************************************************
                             Genotype
 ****************************************************************/

void Genotype::addAllele(int allele)
{
  alleles.push_back(allele);
}



int Genotype::numAlleles() const
{
  return alleles.size();
}



int Genotype::getAllele(int i)
{
  return alleles[i];
}



/****************************************************************
                             VcfReader
 ****************************************************************/

VcfReader::VcfReader(const String &filename)
  : file(NULL), gzRegex("\\.gz$")
{
  file=gzRegex.search(filename) ?
    new GunzipPipe(filename) : new File(filename);
  advance();
}



VcfReader::~VcfReader()
{
  delete file;
}



const Vector<String> &VcfReader::getSampleIDs() const
{
  return sampleIDs;
}



void VcfReader::advance()
{
  genotypes.clear();
  while(!file->eof()) {
    String line=file->getline();
    if(line.empty() && file->eof()) break;
    line.getFields(fields);
    if(fields.size()<1) continue;
    String firstField=fields[0];
    firstField.trimWhitespace();
    if(firstField=="#CHROM") parseChromLine();
    else if(firstField.length()>0 && firstField[0]=='#') continue;
    else { parseVariant(); break; }
  }
}



void VcfReader::parseChromLine()
{
  // #CHROM  POS     ID      REF     ALT     QUAL    FILTER  INFO    FORMAT  200848168@1097100704
  const int numFields=fields.size();
  if(numFields<10 || fields[8]!="FORMAT") 
    throw "Error parsing #CHROM line in VCF file";
  for(int i=9 ; i<numFields ; ++i) sampleIDs.push_back(fields[i]);
}



void VcfReader::parseVariant()
{
  const int numFields=fields.size();
  if(numFields<10 || fields[6]!="PASS" || fields[8]!="GT") return;

  // Parse the variant
  const String chr=fields[0];
  const int pos=fields[1].asInt();
  const String id=fields[2];
  if(id==".") id=chr+"@"+pos;
  const String ref=fields[3];
  const String alt=fields[4];
  Variant v(id,chr,pos);
  v.addAllele(ref);
  Vector<String> alleles; alt.getFields(alleles,",");
  for(int i=0 ; i<alleles.size() ; ++i) v.addAllele(alleles[i]);
  variant=v;

  // Parse the genotypes
  genotypes.clear();
  for(int i=9 ; i<numFields ; ++i) {
    Genotype g;
    parseGenotype(fields[i],g);
    genotypes.push_back(g);
  }
}



void VcfReader::parseGenotype(const String &s,Genotype &genotype)
{
  Vector<String> fields;
  s.getFields(fields,"|/");
  const int n=fields.size();
  for(int i=0 ; i<n ; ++i)
    genotype.addAllele(fields[i].asInt());
}



bool VcfReader::nextVariant(Variant &v,Vector<Genotype> &g)
{
  if(genotypes.size()==0) return false;
  v=variant;
  g=genotypes;
  advance();
  return true;
}






