/****************************************************************
 GCF.C
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "GCF.H"
#include "File.H"
using namespace std;
using namespace BOOM;



GCF::GCF(const String &filename)
{
  parse(filename);
}



void GCF::parse(const String &filename)
{
  File file(filename);

  // Parse header
  String header=file.getline();
  header.trimWhitespace();
  Vector<String> fields; header.getFields(fields);
  for(Vector<String>::const_iterator cur=fields.begin(), end=fields.end() ;
      cur!=end ; ++cur) {
    const String &field=*cur;
    Vector<String> fields; field.getFields(fields,":");
    if(fields.size()!=5) throw field+" : bad entry in GCF header - "+filename;
    const String id=fields[0];
    const String chr=fields[1];
    const int pos=fields[2].asInt();
    const String ref=fields[3], alt=fields[4];
    variants.push_back(GcfVariant(id,chr,pos,ref,alt));
  }
  const int numVariants=variants.size();

  // Parse individuals
  while(!file.eof()) {
    String line=file.getline();
    line.trimWhitespace();
    Vector<String> fields; line.getFields(fields);
    if(fields.size()==0) continue;
    if(fields.size()!=numVariants+1) throw "wrong number of fields in GCF file "+filename;
    const String &id=fields[0];
    individuals.push_back(GcfIndividual(id,numVariants));
    GcfIndividual &indiv=individuals[individuals.size()-1];
    for(int i=0 ; i<numVariants ; ++i) {
      const String &field=fields[i+1];
      if(field.length()!=3) throw field+" : invalid genotype in GCF file "+filename;
      indiv.chrom[0][i]=field[0]-'0';
      indiv.chrom[1][i]=field[2]-'0';
    }
  }
}



int GCF::numVariants() const
{
  return variants.size();
}



int GCF::numIndividuals() const
{
  return individuals.size();
}



const GcfVariant &GCF::getIthVariant(int i) const
{
  return variants[i];
}



const GcfIndividual &GCF::getIthIndividual(int i) const
{
  return individuals[i];
}



