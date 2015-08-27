/****************************************************************
 Process.H
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_Process_H
#define INCL_Process_H
#include <sys/types.h>
#include <unistd.h>
using namespace std;
namespace BOOM {


class Process {
public:
  static pid_t pid();
};

}
#endif

