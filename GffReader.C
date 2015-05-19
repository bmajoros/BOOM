/***********************************************************************
 GffReader.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "VectorSorter.H"
#include "GffReader.H"
using namespace std;
using namespace BOOM;


GffReader::GffReader(const String &filename)
  : commentPattern("\\s*#.*"), 
    transgrpRegex("(transgrp|transcript_id)\\s*=?\\s*([^; \t]+)"),
    geneIdRegex("(gene|gene_id)\\s*=?\\s*([^; \t]+)")
{
  if(!file.open(filename.c_str(),"rb"))
    throw String("Can't open file ")+filename;

  exonTypes.insert("CDS");
  exonTypes.insert("exon");
  exonTypes.insert("initial-exon");
  exonTypes.insert("internal-exon");
  exonTypes.insert("final-exon");
  exonTypes.insert("single-exon");
}



GffFeature *GffReader::nextFeature()
{
  while(true)
    {
      String line=file.readLine();
      if(file.eof()) break;
      if(line.length()==0) continue;
      if(commentPattern.match(line)) continue;
      return new GffFeature(line);
    }
  return NULL;
}



Vector<GffTranscript*> *GffReader::loadTranscripts(const String &filename)
{
  GffReader reader(filename);
  TranscriptList *t=reader.loadTranscripts();
  return t;
}



Vector<GffTranscript*> *GffReader::loadTranscripts()
{
  // Read the features from the GFF file
  Map<String,GffTranscript*> transHash;
  while(GffFeature *f=nextFeature())
    {
      // If this feature is an exon...
      if(f->hasExtraFields() && exonTypes.isMember(f->getFeatureType()))
	{
	  // Parse out the gene ID, if present
	  String geneId;
	  Vector<String> &extraFields=f->getExtraFields();
	  int n=extraFields.size();
	  for(int i=0 ; i<n ; ++i)  
	    if(geneIdRegex.search(extraFields[i])) {
	      geneId=geneIdRegex[2];
	      geneId=geneId.substitute("\"","");
	    }

	  // Parse out transcript ID
	  String transcriptId=extraFields[0];
	  if(transgrpRegex.search(transcriptId)) {
	    transcriptId=transgrpRegex[2];
	    transcriptId=transcriptId.substitute("\"","");
	  }
	  if(!transHash.isDefined(transcriptId))
	    transHash[transcriptId]=
	      new GffTranscript(transcriptId,f->getSubstrate(),
				f->getStrand(),f->getSource());

	  // Add this exon to the appropriate transcript
	  GffTranscript *transcript=transHash[transcriptId];
	  GffExon *exon=new GffExon(*f,*transcript);
	  transcript->addExon(exon);
	  if(transcript->getGeneId()=="") transcript->setGeneId(geneId);
	}
      delete f;
    }

  // Sort exons and resolve exon types; also convert the hash table
  // to a vector of transcripts
  Vector<GffTranscript*> *transcriptList=
    new Vector<GffTranscript*>;
  Map<String,GffTranscript*>::iterator 
    cur=transHash.begin(),
    end=transHash.end();
  for(; cur!=end ; ++cur)
    {
      GffTranscript *transcript=(*cur).second;
      transcript->sortExons();
      transcript->setExonTypes();
      transcriptList->push_back(transcript);
    }

  // Sort the transcripts by position
  TranscriptComparator comp;
  VectorSorter<GffTranscript*> sorter(*transcriptList,comp);
  sorter.sortAscendInPlace();

  return transcriptList;
}



Map<String,GffReader::TranscriptList*> *GffReader::loadByContig()
{
  Map<String,TranscriptList*> &M=*
    new Map<String,TranscriptList*>;
  TranscriptList *allTranscripts=loadTranscripts();
  typename TranscriptList::iterator cur=allTranscripts->begin(),
    end=allTranscripts->end();
  for(; cur!=end ; ++cur)
    {
      GffTranscript *transcript=*cur;
      if(!transcript) break;
      const String &substrate=transcript->getSubstrate();
      if(!M.isDefined(substrate)) M[substrate]=new TranscriptList;
      TranscriptList *transcripts=M[substrate];
      transcripts->push_back(transcript);
    }
  delete allTranscripts;
  return &M;
}



Map<String,Vector<GffFeature*> > *GffReader::featuresBySubstrate()
{
  Map<String,Vector<GffFeature*> > &M=*
    new Map<String,Vector<GffFeature*> >;
  Vector<GffFeature*> *features=loadFeatures();
  const int numFeatures=features->size();
  for(int i=0 ; i<numFeatures ; ++i) {
    GffFeature *feature=(*features)[i];
    const String &substrate=feature->getSubstrate();
    Vector<GffFeature*> &features=M[substrate];
    features.push_back(feature);
  }
  delete features;
  return &M;
}



Vector<GffFeature*> *GffReader::loadFeatures()
{
    Vector<GffFeature*> *features=new Vector<GffFeature*>;
    GffFeature *feature;
    while(feature=nextFeature())
        features->push_back(feature);
    GffFeatureComparator comp;
    VectorSorter<GffFeature*> sorter(*features,comp);
    sorter.sortAscendInPlace();
    return features;
}



void GffReader::addNoncodingFeatures(Vector<GffFeature*> &codingFeatures,
                                     int substrateLen,const String &substrate,
                                     const String &source,
                                     int maxFeatureLength)
{
    Vector<GffFeature*> newFeatures;
    int n=codingFeatures.size();
    for(int i=0 ; i<n-1 ; ++i)
    {
        GffFeature *thisFeature=codingFeatures[i];
        GffFeature *nextFeature=codingFeatures[i+1];
        int begin=thisFeature->getEnd(), end=nextFeature->getBegin();
        if(end-begin>maxFeatureLength) end=begin+maxFeatureLength;
        GffFeature *feature=
            new GffFeature("",substrate,source,"noncoding",begin,end,1.0,
                           false,".",0,false);
        newFeatures.push_back(feature);
    }
    codingFeatures.append(newFeatures);
    GffFeatureComparator comp;
    VectorSorter<GffFeature*> sorter(codingFeatures,comp);
    sorter.sortAscendInPlace();
}



void GffReader::filterBySource(Vector<GffFeature*> &features,
                               const String &targetSource)
{
    Vector<GffFeature*> keep;
    Vector<GffFeature*>::iterator cur=features.begin(), end=features.end();
    for(; cur!=end ; ++cur)
    {
        GffFeature *feature=*cur;
        if(feature->getSource()==targetSource)
            keep.push_back(feature);
    }
    features=keep;
}



Vector<GffGene> *GffReader::loadGenes()
{
  Vector<GffGene> &genes=*new Vector<GffGene>;
  Map<String,GffGene*> byID;
  Vector<GffTranscript*> &allTrans=*loadTranscripts();
  for(Vector<GffTranscript*>::iterator cur=allTrans.begin(), end=
	allTrans.end() ; cur!=end ; ++cur) {
    GffTranscript *trans=*cur;
    const String &geneID=trans->getGeneId();
    if(!byID.isDefined(geneID)) byID[geneID]=new GffGene(geneID);
    byID[geneID]->addTranscript(trans);
  }
  delete &allTrans;
  return &genes;
}



Vector<GffGene> *GffReader::loadGenes(const String &filename)
{
  Map<String,GffGene*> byID;
  Vector<GffTranscript*> &allTrans=*loadTranscripts(filename);
  for(Vector<GffTranscript*>::iterator cur=allTrans.begin(), end=
	allTrans.end() ; cur!=end ; ++cur) {
    GffTranscript *trans=*cur;
    const String &geneID=trans->getGeneId();
    if(!byID.isDefined(geneID)) byID[geneID]=new GffGene(geneID);
    byID[geneID]->addTranscript(trans);
  }
  delete &allTrans;
  Set<String> IDs;
  byID.getKeys(IDs);
  Vector<GffGene> &genes=*new Vector<GffGene>;
  for(Set<String>::iterator cur=IDs.begin(), end=IDs.end() ; cur!=end ; ++cur) {
    cout<<*cur<<endl;
    cout<<byID[*cur]<<endl;
    cout<<byID[*cur]->getID()<<endl;
    genes.push_back(*byID[*cur]);
    TRACE
  }
TRACE
  return &genes;
}



Map<String,Vector<GffGene> > *GffReader::genesByChrom(const String &filename)
{
  // Load transcripts and populate genes with them
  Vector<GffGene> genes;
  Map<String,GffGene*> byID;
  Vector<GffTranscript*> &allTrans=*loadTranscripts(filename);
  for(Vector<GffTranscript*>::iterator cur=allTrans.begin(), end=
	allTrans.end() ; cur!=end ; ++cur) {
    GffTranscript *trans=*cur;
    const String &geneID=trans->getGeneId();
    if(!byID.isDefined(geneID)) byID[geneID]=new GffGene(geneID);
    byID[geneID]->addTranscript(trans);
  }
  delete &allTrans;

  // Now organize genes by chromosome
  Map<String,Vector<GffGene> > &byChrom=
    *new Map<String,Vector<GffGene> >;
  for(Vector<GffGene>::iterator cur=genes.begin(), end=genes.end() ;
      cur!=end ; ++cur) {
    GffGene &gene=*cur;
    const String &id=gene.getID();
    //if(!byChrom.isDefined(id)) byChrom[id]=Vector<GffGene>();
    byChrom[id].push_back(gene);
  }
  return &byChrom;

}


