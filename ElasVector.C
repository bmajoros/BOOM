/*
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 */
#include "ElasVector.H"


ElasDblVect::ElasDblVect() : ElasVector<double>(0) 
{
}



ElasFloatVect::ElasFloatVect() : ElasVector<float>(0) 
{
}



ElasIntVect::ElasIntVect() : ElasVector<int>(0) 
{
}



ElasCharVect::ElasCharVect() : ElasVector<char>('\0') 
{
}



ElasLongVect::ElasLongVect() : ElasVector<long>(0) 
{
}

