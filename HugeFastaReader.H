/***********************************************************************
 HugeFastaReader.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_HugeFastaReader_H
#define INCL_BOOM_HugeFastaReader_H
using namespace std;

#include "String.H"
#include "File.H"

// THIS CLASS HAS BEEN FULLY VERIFIED -- IT IS ALMOST *CERTAINLY*
// BUG FREE!

/****************************************************************
  This class can be used to read arbitrarily large FASTA files.
  The file is read in chunks.  You specify a chunk size (C) and an
  overlap size (O), and this class gives you successive chunks in
  the file.  The first chunk will be of the full chunk size C and
  is accessible via getBuffer(), but all subsequent chunks will be
  of size C-O and are accessed through getRemainder().  However,
  for the default overlap size of zero, you can just use getBuffer()
  for all chunks.  Incidentally, the buffer pointer that is returned
  does not change after calling loadNextChunk(), so you can just
  call it once and store it locally; loadNextChunk() will change
  the contents of that buffer for you each time.  The same is true
  of getRemainder(); it is just an offset into the buffer.

  The overlap feature is useful if you want to slide a window over
  the sequence without treating the ends of the chunks differently.
 */
namespace BOOM {
class HugeFastaReader
{
public:
  HugeFastaReader(const BOOM::String &filename,int chunkSize,
		      int overlapSize=0);
  virtual ~HugeFastaReader();
  const char *getBuffer();
  const char *getRemainder() {return remainder;}
  bool loadNextChunk();
  bool isMore();
  const BOOM::String &getDefline();
private:
  BOOM::String defline;
  BOOM::File file;
  char *buffer, *overlap, *remainder, *end;
  int chunkSize, overlapSize, remainderSize;
  void loadFirstChunk();
  void filterSpaces(char *);
};
}

#endif
