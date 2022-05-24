/***********************************************************************
 File.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_File_H
#define INCL_BOOM_File_H
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include "String.H"
using namespace std;

namespace BOOM {
class File {
public:
  File(const BOOM::String &filename,const BOOM::String &mode="r"); // do a "man fopen" for info on the mode string: r, w, or r+
  File(FILE *fp);
  File();
  static bool getFileList(const String &directory,Vector<String> &into);
  virtual ~File();
  operator FILE*();
  long countLines();
  static BOOM::String getFilenameNoPath(const BOOM::String &filename);
  static BOOM::String getPath(const BOOM::String &filename);
  static BOOM::String getTempFilename(const String &dir,const String &prefix);
  static bool copy(const BOOM::String &from,const BOOM::String &to);
  static bool exists(const BOOM::String &filename);
  BOOM::String getline();  // newline is stripped from end
  static void mkdir(const String &path); // implemented as: system("mkdir -p path")
  inline BOOM::String readLine() { return getline(); }
  bool eof();
  bool isOpen() const;
  bool open();
  bool open(const BOOM::String &filename,const BOOM::String &mode="r");
  long read(long bytes,void *buffer);
  bool seek(long offset);
  bool write(long bytes,const void *buffer);
  long getPosition();
  long getSize() const;
  time_t lastAccessTime();
  time_t lastChangeTime();
  time_t lastModifyTime();
  virtual void close();
  void print(const BOOM::String &);
  void rewind();
  void updateStats();
  void write(const int &);
  void write(const short &);
  void write(const long &);
  void write(char);
  void write(unsigned char);
  void write(const float &);
  void write(const double &);
  void write(const BOOM::String &);
  void write(const char *);
  int readInt();
  short readShort();
  long readLong();
  char readChar();
  unsigned char readByte();
  float readFloat();
  double readDouble();
  BOOM::String readString();
  char *readCharString();
  void readString(BOOM::String &);
  BOOM::String *readStringPtr();
  void flush();
protected:
  struct stat fileInfo;
  BOOM::String filename, mode;
  FILE *fp;
  static const char *findLastSlash(const char *begin,const char *end);
  static const char *findEndOfString(const char *begin);
};


// Writing:
BOOM::File &operator<<(BOOM::File &,int);
BOOM::File &operator<<(BOOM::File &,short);
BOOM::File &operator<<(BOOM::File &,long);
BOOM::File &operator<<(BOOM::File &,float);
BOOM::File &operator<<(BOOM::File &,double);
BOOM::File &operator<<(BOOM::File &,char);
BOOM::File &operator<<(BOOM::File &,const BOOM::String &);
BOOM::File &operator<<(BOOM::File &,const char *);

// Reading:
BOOM::File &operator>>(BOOM::File &,int &);
BOOM::File &operator>>(BOOM::File &,short &);
BOOM::File &operator>>(BOOM::File &,long &);
BOOM::File &operator>>(BOOM::File &,float &);
BOOM::File &operator>>(BOOM::File &,double &);
BOOM::File &operator>>(BOOM::File &,char &);
BOOM::File &operator>>(BOOM::File &,BOOM::String &);
}

#endif
