/* ===============================================================
 MPI.H 
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ===============================================================*/
#ifndef INCL_BOOMMPI_H
#define INCL_BOOMMPI_H
#define MPICH_SKIP_MPICXX
#include <mpi.h>
using namespace std;
#include "String.H"
#include "Vector.H"
#include "ListQueue.H"
#include "GSL/Vector.H"
using namespace std;

namespace BOOM { 

// Define your message tags as enums numbered starting at zero; cast
// them to MPI_MESSAGE_TAG:
typedef int MPI_MESSAGE_TAG;

// Negative message tags are reserved by the framework:
const MPI_MESSAGE_TAG SLAVE_READY=-1;
const MPI_MESSAGE_TAG TERMINATE_SLAVE=-2;



/****************************************************************
                        class MpiFixedMsg
 ****************************************************************/
class MpiFixedMsg {
public:
  // Use this ctor when sending messages:
  MpiFixedMsg(MPI_MESSAGE_TAG,unsigned numBytes); // numBytes not include tag
  // Use this ctor when receiving messages:
  MpiFixedMsg(unsigned totalBufferSize); // size includes tag
  virtual ~MpiFixedMsg();
  virtual MpiFixedMsg &operator<<(int);
  virtual MpiFixedMsg &operator<<(short int);
  virtual MpiFixedMsg &operator<<(long);
  virtual MpiFixedMsg &operator<<(char);
  virtual MpiFixedMsg &operator<<(unsigned char);
  virtual MpiFixedMsg &operator<<(unsigned int);
  virtual MpiFixedMsg &operator<<(unsigned long);
  virtual MpiFixedMsg &operator<<(const String &);
  virtual MpiFixedMsg &operator<<(const char *);
  virtual MpiFixedMsg &operator<<(float);
  virtual MpiFixedMsg &operator<<(double);
  virtual MpiFixedMsg &operator<<(const GSL::Vector &);
  void beginExtraction(); // call before using any of the >> operators!
  MpiFixedMsg &operator>>(int&);
  MpiFixedMsg &operator>>(short int&);
  MpiFixedMsg &operator>>(long&);
  MpiFixedMsg &operator>>(char&);
  MpiFixedMsg &operator>>(unsigned char&);
  MpiFixedMsg &operator>>(unsigned int&);
  MpiFixedMsg &operator>>(unsigned long&);
  MpiFixedMsg &operator>>(String &);
  MpiFixedMsg &operator>>(float&);
  MpiFixedMsg &operator>>(double&);
  MpiFixedMsg &operator>>(GSL::Vector &);
  void *getBuffer();
  unsigned int getBufferSize(); // total size, including tag
  MPI_MESSAGE_TAG getTag();
  void setSource(int ID);
  int getSource() const;
protected:
  MPI_MESSAGE_TAG tag;
  void *buffer, *nextSlot;
  unsigned bufferSize;
  int sourceID;
  void allocate(int bytes);
  void deallocate();
  void extract(void *into,size_t bytes);
};



/****************************************************************
                         MpiManipulator
 ****************************************************************/
class MpiVariableMsg;
typedef MpiVariableMsg& (*MpiManipulator)(MpiVariableMsg&);



/****************************************************************
                       class MpiVariableMsg
 ****************************************************************/
class MpiVariableMsg : public MpiFixedMsg {
  friend MpiVariableMsg &endmsg(MpiVariableMsg &);
public:
  MpiVariableMsg(MPI_MESSAGE_TAG);
  virtual ~MpiVariableMsg();
  virtual MpiFixedMsg &operator<<(int);
  virtual MpiFixedMsg &operator<<(short int);
  virtual MpiFixedMsg &operator<<(long);
  virtual MpiFixedMsg &operator<<(char);
  virtual MpiFixedMsg &operator<<(unsigned char);
  virtual MpiFixedMsg &operator<<(unsigned int);
  virtual MpiFixedMsg &operator<<(unsigned long);
  virtual MpiFixedMsg &operator<<(const String &);
  virtual MpiFixedMsg &operator<<(const char *);
  virtual MpiFixedMsg &operator<<(float);
  virtual MpiFixedMsg &operator<<(double);
  virtual MpiFixedMsg &operator<<(MpiManipulator);
  void close(); // not necessary if you use <<endmsg
protected:
  void append(void*,size_t);
  Vector<unsigned char> varBuf;
};

MpiVariableMsg &endmsg(MpiVariableMsg &);



/****************************************************************
                             class MPI
 ****************************************************************/
class MPI {
public:
  MPI(int &argc,char ***argv); // ### DO NOT CHANGE ARGV TO char** !!!!!!!!
  virtual ~MPI();
  int getNumProcesses() const;
  int getNumSlaves() const;
  int getProcessID() const;
  int getIthSlaveID(int i) const;
  void send(MpiFixedMsg &,int toProcessID);
  void send(MPI_MESSAGE_TAG,int toProcessID);
  MpiFixedMsg *waitForMessage();
  bool amItheMaster() {return processID==0;}
protected:
  int numProcesses;
  int processID;
};



/****************************************************************
                       class MpiSlaveDriver
 ****************************************************************/
class MpiSlaveDriver {
public:
  MpiSlaveDriver(MPI &);

  // For use by the MASTER:
  void addWork(MpiFixedMsg *); // add some more work to the pile
  void waitForResults(Vector<MpiFixedMsg*> &results); // block till all done
  void terminateSlaves(); // terminate all slave processes
  
  // For use by the SLAVES:
  MpiFixedMsg *acceptWork(); // may be a TERMINATE_SLAVE message
  void replyToMaster(MpiFixedMsg *); // SlaveDriver deletes msg object
  bool amItheMaster() {return mpi.amItheMaster();}
  int getNumSlaves() {return numSlaves;}
  MPI &getMPI() {return mpi;}

protected:
  MPI &mpi;
  ListQueue<MpiFixedMsg*> work;
  ListQueue<int> slaves; // slave ID's
  Vector<MpiFixedMsg*> results;
  int numSlaves;
  int getAvailableSlave();
};



}


#endif
