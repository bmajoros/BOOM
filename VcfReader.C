/****************************************************************
 VcfReader.C
 Copyright (C)2016 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "VcfReader.H"
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
{
  file(filename);
  advance();
}



const Vector<String> &VcfReader::getSampleIDs() const
{
  return sampleIDs;
}



void VcfReader::advance()
{
  while(!file.eof()) {
    String line=file.getline();
    
  }
}



bool VcfReader::nextVariant(Variant &,Vector<Genotype> &)
{
}






