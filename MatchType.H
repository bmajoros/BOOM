/***********************************************************************
 MatchType.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_MatchType_H
#define INCL_MatchType_H

namespace BOOM {
enum MatchType
  {
    MATCH,            // not necessarily identical!
    FIRST_UNMATCHED,  // insertion
    SECOND_UNMATCHED  // deletion
  };
}


#endif
