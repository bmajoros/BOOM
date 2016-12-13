/****************************************************************
 TwoBitFastaFile.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include <ctype.h>
#include <fstream>
#include "TwoBitFastaFile.H"
#include "DnaAlphabet.H"
using namespace std;
using namespace BOOM;


TwoBitFastaFile::TwoBitFastaFile(const String &filename,const String &mode)
    : file(filename,mode)
{
}



void TwoBitFastaFile::setLength(long length)
{
    file.seek(0);
    file.write(length);
}



long TwoBitFastaFile::getLength()
{
    file.seek(0);
    return file.readLong();
}



void TwoBitFastaFile::load(Sequence &into)
{
    long len=getLength(); // seek() not necessary after this
    long numRegs=len/3;
    unsigned char c;
    for(long i=0 ; i<numRegs ; ++i)
    {
        file.read(1,(void*)&c);
        decode(c);
        for(int j=0 ; j<3 ; ++j)
            into.append(reg[j]);
    }
    int leftovers=len%3;
    if(leftovers>0)
    {
        file.read(1,(void*)&c);
        decode(c);
        for(int j=0 ; j<leftovers ; ++j)
            into.append(reg[j]);
    }
}



void TwoBitFastaFile::write(const Sequence &from)
{
    long len=from.getLength();
    setLength(len); // seek() not necessary after this
    long numRegs=len/3;
    long seqPos=0;
    for(long i=0 ; i<numRegs ; ++i)
    {
        for(int j=0 ; j<3 ; ++j)
            reg[j]=from[seqPos++];
        unsigned char c=encode();
        file.write(1,(void*)&c);
    }
    int leftovers=len%3;
    if(leftovers>0)
    {
      int j=0;
      for(; j<leftovers ; ++j)
	reg[j]=from[seqPos++];
      for(; j<3 ; ++j)
	reg[j]=0;
      unsigned char c=encode();
      file.write(1,(void*)&c);
    }
}



void TwoBitFastaFile::close()
{
    file.close();
}



Symbol TwoBitFastaFile::get(long pos)
{
    long regPos=nucPosToRegPos(pos);
    loadReg(regPos);
    int regIndex=pos%3;
    return reg[regIndex];
}



void TwoBitFastaFile::set(long pos,Symbol s)
{
    long regPos=nucPosToRegPos(pos);
    loadReg(regPos);
    int regIndex=pos%3;
    reg[regIndex]=s;
    commitReg(regPos);
}



unsigned char TwoBitFastaFile::encode()
{
    return reg[0]*25+reg[1]*5+reg[2];
}



void TwoBitFastaFile::decode(unsigned char c)
{
    reg[2]=c%5;
    c/=5;
    reg[1]=c%5;
    c/=5;
    reg[0]=c;
}



void TwoBitFastaFile::loadReg(long regPos)
{
    file.seek(sizeof(long)+regPos);
    unsigned char c;
    file.read(1,(void*)&c);
    decode(c);
}



void TwoBitFastaFile::commitReg(long regPos)
{
    file.seek(regPos);
    unsigned char c=encode();
    file.write(1,(void*)&c);
}



long TwoBitFastaFile::nucPosToRegPos(long nucPos)
{
    return nucPos/3;
}



void TwoBitFastaFile::encodeFromFasta(const String &fastaFilename,
                                 String &defline)
{
    file.seek(sizeof(long));
    long seqLength=0;
    Alphabet &alphabet=DnaAlphabet::global();
    File fastaFile(fastaFilename);
    defline=fastaFile.getline();
    int regIndex=0;
    while(!fastaFile.eof())
    {
        char nucleotide=fastaFile.readChar();
        if(fastaFile.eof()) break;
        if(isspace(nucleotide)) continue;
        ++seqLength;
        nucleotide=toupper(nucleotide);
        Symbol s;
        switch(nucleotide)
        {
            case 'A': case 'C': case 'G': case 'T':
                s=alphabet.lookup(nucleotide); break;
            default:
                s=alphabet.lookup('N');
        }
        reg[regIndex++]=s;
        if(regIndex==3)
        {
            unsigned char c=encode();
            file.write(1,(void*)&c);
            regIndex=0;
        }
    }
    if(regIndex>0)
    {
        for(; regIndex<3 ; ++regIndex)
            reg[regIndex]=0;
        unsigned char c=encode();
        file.write(1,(void*)&c);
    }
    setLength(seqLength);
}



void TwoBitFastaFile::decodeIntoFasta(const String &fastaFile,String &defline)
{
  Alphabet &alphabet=DnaAlphabet::global();
    ofstream os(fastaFile.c_str());
    os<<defline<<endl;
    long len=getLength(); // seek() not necessary after this
    long numRegs=len/3;
    unsigned char c;
    int lineLength=0;
    for(long i=0 ; i<numRegs ; ++i)
    {
        file.read(1,(void*)&c);
        decode(c);
        for(int j=0 ; j<3 ; ++j)
            os<<alphabet.lookup(reg[j]);
        lineLength+=3;
        if(lineLength>=60) {os<<endl; lineLength=0;}
    }
    int leftovers=len%3;
    if(leftovers>0)
    {
        file.read(1,(void*)&c);
        decode(c);
        for(int j=0 ; j<leftovers ; ++j)
            os<<alphabet.lookup(reg[j]);
    }
    os<<endl;
}



void TwoBitFastaFile::getSubsequence(long firstPos,long length,Sequence &into)
{
    long lastPos=firstPos+length-1;
    long firstReg=nucPosToRegPos(firstPos);
    long lastReg=nucPosToRegPos(lastPos);
    loadReg(firstReg);
    int firstIndex=firstPos%3, lastIndex=lastPos%3;
    if(firstReg==lastReg)
        for(int i=firstIndex ; i<=lastIndex ; ++i)
            into.append(reg[i]);
    else
    {
        for(int i=firstIndex ; i<3 ; ++i)
            into.append(reg[i]);
        for(long regPos=firstReg+1 ; regPos<lastReg ; ++regPos)
        {
            loadReg(regPos);
            for(int i=0 ; i<3 ; ++i)
                into.append(reg[i]);
        }
        loadReg(lastReg);
        for(int i=0 ; i<=lastIndex ; ++i)
            into.append(reg[i]);
    }
}





