/***********************************************************************
 File.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include "File.H"
#include "Vector.H"
#include "Random.H"
using namespace std;



BOOM::File::File(const BOOM::String &filename,const BOOM::String &mode)
  : mode(mode), filename(filename), fp(NULL) 
{
  if(mode.length()>0) 
    if(mode[0]=='r') 
      updateStats();

  if(!open()) 
    if(mode[0]=='w')
      throw BOOM::String("Cannot write file: \"")+filename+"\"";
    else
      throw BOOM::String("Cannot open file: \"")+filename+"\"";
}



BOOM::File::File(FILE *fp)
  : fp(fp)
{
}



BOOM::File::File()
  : fp(NULL)
{
}



BOOM::File::~File()
{
  close();
}



BOOM::String BOOM::File::getPath(const BOOM::String &filename)
{
  const char *begin=filename.c_str();
  const char *end=findEndOfString(begin);
  const char *slash=findLastSlash(begin,end);
  int len=slash-begin;
  String path(begin,len);
  if(!path.empty() && path.lastChar()=='/') path.chop();
  return path;
}



BOOM::String BOOM::File::getFilenameNoPath(const BOOM::String &filename)
{
  const char *begin=filename.c_str();
  const char *end=findEndOfString(begin);
  const char *slash=findLastSlash(begin,end);
  return BOOM::String(slash);
}



const char *BOOM::File::findLastSlash(const char *begin,const char *end)
{
  const char *slash=end;
  while(slash>begin && *slash!='/') 
    --slash;
  if(*slash=='/') 
    ++slash;
  return slash;
}



const char *BOOM::File::findEndOfString(const char *end)
{
  while(*end) ++end;
  return end;
}



BOOM::String BOOM::File::getline()
{
  char buf[1025];
  BOOM::String s;
  while(1)
    {
      if(!fgets(buf,sizeof(buf),fp)) break;
      size_t len=strlen(buf);
      if(buf[len-1]=='\n')
	{
	  buf[len-1]='\0';
	  s+=buf;
	  break;
	}
      s+=buf;
    }
  return s;
}



bool BOOM::File::copy(const BOOM::String &from,const BOOM::String &to)
{
  BOOM::String command="cp "+from+" "+to;
  int rval=system(command.c_str())==0;
  return rval;
}



bool BOOM::File::eof()
{
  return feof(fp);
}



bool BOOM::File::exists(const BOOM::String &filename)
{
  struct stat fInfo;
  int status=stat(filename.c_str(),&fInfo);
  bool doesExist=(status==0);
  return doesExist;
}



bool BOOM::File::isOpen() const
{
  return fp!=NULL;
}



bool BOOM::File::open()
{
  if(mode.length()>0)
    if(mode[0]=='r') 
      stat(filename.c_str(),&fileInfo);

  if(!fp)
    fp=fopen(filename.c_str(),mode.c_str());
  return fp!=NULL;
}



bool BOOM::File::open(const BOOM::String &fn,const BOOM::String &md)
{
  close();
  if(md.length()>0)
    if(md[0]=='r') 
      stat(fn.c_str(),&fileInfo);
  filename=fn;
  mode=md;
  return open();
}



void BOOM::File::flush()
{
  fflush(fp);
}



long BOOM::File::read(long numbytes,void *buffer)
{
  //return fread(buffer,1,numbytes,fp);
  return fread(buffer,numbytes,1,fp);
}



bool BOOM::File::seek(long pos)
{
  return fseek(fp,pos,SEEK_SET)==0;
}



bool BOOM::File::write(long numbytes,const void *buffer)
{
  return fwrite(buffer,1,numbytes,fp)==numbytes;
}



long BOOM::File::countLines()
{
  long numlines=0;
  while(!eof())
    {
      getline();
      ++numlines;
    }
  return numlines;
}



long BOOM::File::getPosition()
{
  return ftell(fp);
}



long BOOM::File::getSize() const
{
  return (long) fileInfo.st_size;
}



time_t BOOM::File::lastAccessTime()
{
  return fileInfo.st_atime;
}



time_t BOOM::File::lastChangeTime()
{
  return fileInfo.st_ctime;
}



time_t BOOM::File::lastModifyTime()
{
  return fileInfo.st_mtime;
}



void BOOM::File::close()
{
  if(fp) fclose(fp);
  fp=(FILE*)0;
}



void BOOM::File::print(const BOOM::String &s)
{
  if(!fputs(s.c_str(),fp))
    throw BOOM::String("Error printing into file in BOOM::File::print()");
}



void BOOM::File::rewind()
{
  ::rewind(fp);
}



void BOOM::File::updateStats()
{
  stat(filename.c_str(),&fileInfo);
}



inline BOOM::File::operator FILE*() 
{
  return fp;
}



void BOOM::File::write(const int &x)
{
  write(sizeof(x),static_cast<const void*>(&x));
}



void BOOM::File::write(const short &x)
{
  write(sizeof(x),static_cast<const void*>(&x));
}



void BOOM::File::write(const long &x)
{
  write(sizeof(x),static_cast<const void*>(&x));
}



void BOOM::File::write(char c)
{
  write(sizeof(c),static_cast<const void*>(&c));
}



void BOOM::File::write(const float &x)
{
  write(sizeof(x),static_cast<const void*>(&x));
}



void BOOM::File::write(const double &x)
{
  write(sizeof(x),static_cast<const void*>(&x));
}



void BOOM::File::write(unsigned char x)
{
  write(sizeof(x),static_cast<const void*>(&x));
}



void BOOM::File::write(const BOOM::String &x)
{
  long len=x.length();
  write(len);
  write(len,x.c_str());
}



void BOOM::File::write(const char *p)
{
  long len=strlen(p);
  write(len);
  write(len,p);
}



int BOOM::File::readInt()
{
  int x;
  read(sizeof(x),static_cast<void*>(&x));
  return x;
}



short BOOM::File::readShort()
{
  short x;
  read(sizeof(x),static_cast<void*>(&x));
  return x;
}



long BOOM::File::readLong()
{
  long x;
  read(sizeof(x),static_cast<void*>(&x));
  return x;
}



unsigned char BOOM::File::readByte()
{
  unsigned char x;
  read(sizeof(x),static_cast<void*>(&x));
  return x;
}



char BOOM::File::readChar()
{
  char x;
  read(sizeof(x),static_cast<void*>(&x));
  return x;
}



float BOOM::File::readFloat()
{
  float x;
  read(sizeof(x),static_cast<void*>(&x));
  return x;
}



double BOOM::File::readDouble()
{
  double x;
  read(sizeof(x),static_cast<void*>(&x));
  return x;
}



BOOM::String BOOM::File::readString()
{
  int len=readLong();
  char *p=new char[len+1];
  p[len]='\0';
  read(len,p);

  //BOOM::String s(p); // ### doesn't work for encrypted strings!
  string s(len,' ');
  for(int i=0 ; i<len ; ++i) s[i]=p[i];

  delete [] p;
  return s;
}



char *BOOM::File::readCharString()
{
  int len=readLong();
  char *p=new char[len+1];
  p[len]='\0';
  read(len,p);
  return p;
}



void BOOM::File::readString(BOOM::String &str)
{
  int len=readLong();
  char *p=new char[len+1];
  p[len]='\0';
  read(len,p);
  str=p;
  delete [] p;
}



BOOM::String *BOOM::File::readStringPtr()
{
  int len=readLong();
  char *p=new char[len+1];
  p[len]='\0';
  read(len,p);
  BOOM::String *str=new BOOM::String(p);
  delete [] p;
  return str;
}



BOOM::File &BOOM::operator<<(BOOM::File &f,int x)
{
  f.write(x);
  return f;
}



BOOM::File &BOOM::operator<<(BOOM::File &f,short x)
{
  f.write(x);
  return f;
}



BOOM::File &BOOM::operator<<(BOOM::File &f,long x)
{
  f.write(x);
  return f;
}



BOOM::File &BOOM::operator<<(BOOM::File &f,float x)
{
  f.write(x);
  return f;
}



BOOM::File &BOOM::operator<<(BOOM::File &f,double x)
{
  f.write(x);
  return f;
}



BOOM::File &BOOM::operator<<(BOOM::File &f,char x)
{
  f.write(x);
  return f;
}



BOOM::File &BOOM::operator<<(BOOM::File &f,const BOOM::String &x)
{
  f.write(x);
  return f;
}



BOOM::File &BOOM::operator<<(BOOM::File &f,const char *x)
{
  f.write(x);
  return f;
}



BOOM::File &BOOM::operator>>(BOOM::File &f,int &x)
{
  x=f.readInt();
  return f;
}



BOOM::File &BOOM::operator>>(BOOM::File &f,short &x)
{
  x=f.readShort();
  return f;
}



BOOM::File &BOOM::operator>>(BOOM::File &f,long &x)
{
  x=f.readLong();
  return f;
}



BOOM::File &BOOM::operator>>(BOOM::File &f,float &x)
{
  x=f.readFloat();
  return f;
}



BOOM::File &BOOM::operator>>(BOOM::File &f,double &x)
{
  x=f.readDouble();
  return f;
}



BOOM::File &BOOM::operator>>(BOOM::File &f,char &x)
{
  x=f.readChar();
  return f;
}



BOOM::File &BOOM::operator>>(BOOM::File &f,BOOM::String &x)
{
  f.readString(x);
  return f;
}



bool BOOM::File::getFileList(const String &directory,Vector<String> &into)
{
  DIR *dir=opendir(directory.c_str());
  if(!dir) return false;
  while(1) {
    struct dirent *entry=readdir(dir);
    if(!entry) break;
    into.push_back(entry->d_name);
  }
  closedir(dir);
  return true;
}



void BOOM::File::mkdir(const String &path)
{
  String cmd("mkdir -p ");
  cmd+=path;
  system(cmd.c_str());
}



BOOM::String BOOM::File::getTempFilename(const String &dir,const String &prefix)
{
  while(1) {
    int r=RandomNumber(INT_MAX);
    String filename=dir+prefix+r;
    if(!BOOM::File::exists(filename)) return filename;
  }
}


