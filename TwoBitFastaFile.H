/****************************************************************
 TwoBitFastaFile.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_TwoBitFastaFile_H
#define INCL_TwoBitFastaFile_H
#include <iostream>
#include "String.H"
#include "File.H"
#include "Sequence.H"
using namespace std;


/*
  A FASTA file represented compactly using 2.7 bits per symbol, allowing
  any of the five symbols {A,C,G,T,N} to occur, with constant-time random
  access to any base in the file.
 */
namespace BOOM {
class TwoBitFastaFile
{
public:
    TwoBitFastaFile(const String &filename,const String &mode="r");
    void setLength(long length);
    long getLength();
    void load(Sequence &into);
    void getSubsequence(long begin,long length,Sequence &into);
    void write(const Sequence &from);
    encodeFromFasta(const String &fastaFile,String &putDeflineInHere);
    decodeIntoFasta(const String &fastaFile,String &defline);
    void close();
    Symbol get(long pos);
    void set(long pos,Symbol);
private:
    File file;
    Symbol reg[3];

    unsigned char encode();  // encodes contents of reg[], returns result
    void decode(unsigned char);// decodes a byte into the reg[] array
    void loadReg(long regPos);
    void commitReg(long regPos);
    long nucPosToRegPos(long nucPos);
};
}

#endif

