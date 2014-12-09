/*
 TestScanner.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "Scanner.H"
#include <iostream.h>

int main(int,char **)
{
  Scanner scanner(cin);

  while(!cin.eof())
    {
      Token *token=scanner.nextToken();
      if(!token) break;
      cout << *token << endl;
      delete token;
    }
  cout << "EOF" << endl;
}
