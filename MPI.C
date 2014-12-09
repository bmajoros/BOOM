/* ===============================================================
 MPI.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ===============================================================*/
#include "MPI.H"
using namespace BOOM;



MpiVariableMsg &BOOM::endmsg(MpiVariableMsg &msg)
{
  msg.close();
  return msg;
}



/****************************************************************
                        MpiFixedMsg methods
 ****************************************************************/

MpiFixedMsg::MpiFixedMsg(MPI_MESSAGE_TAG t,unsigned numBytes)
  : tag(t)
{
  bufferSize=numBytes+sizeof(t);
  allocate(bufferSize);
  memcpy(buffer,(void*)&t,sizeof(t));
  nextSlot=(numBytes>0) ? buffer+sizeof(t) : NULL;
}



MpiFixedMsg::MpiFixedMsg(unsigned totalBufferSize)
  : bufferSize(totalBufferSize)
{
  allocate(totalBufferSize);
  nextSlot=buffer+sizeof(tag);
}



MpiFixedMsg::~MpiFixedMsg()
{
  deallocate();
}



unsigned int MpiFixedMsg::getBufferSize()
{
  return bufferSize;
}



void MpiFixedMsg::beginExtraction()
{
  memcpy((void*)&tag,buffer,sizeof(tag));
  nextSlot=buffer+sizeof(tag);
}



void MpiFixedMsg::allocate(int bytes)
{
  buffer=(void*) new unsigned char[bytes];
  bufferSize=bytes;
}



void MpiFixedMsg::deallocate()
{
  delete [] (unsigned char*) buffer;
}



MpiFixedMsg &MpiFixedMsg::operator<<(int x)
{
  memcpy(nextSlot,(void*)&x,sizeof(x));
  nextSlot+=sizeof(x);
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator<<(short int x)
{
  memcpy(nextSlot,(void*)&x,sizeof(x));
  nextSlot+=sizeof(x);
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator<<(long x)
{
  memcpy(nextSlot,(void*)&x,sizeof(x));
  nextSlot+=sizeof(x);
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator<<(char x)
{
  memcpy(nextSlot,(void*)&x,sizeof(x));
  nextSlot+=sizeof(x);
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator<<(unsigned char x)
{
  memcpy(nextSlot,(void*)&x,sizeof(x));
  nextSlot+=sizeof(x);
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator<<(unsigned int x)
{
  memcpy(nextSlot,(void*)&x,sizeof(x));
  nextSlot+=sizeof(x);
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator<<(unsigned long x)
{
  memcpy(nextSlot,(void*)&x,sizeof(x));
  nextSlot+=sizeof(x);
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator<<(const String &x)
{
  return (*this)<<x.c_str();
}



MpiFixedMsg &MpiFixedMsg::operator<<(const char *x)
{
  int len=strlen(x)+1;
  memcpy(nextSlot,(void*)x,len);
  nextSlot+=len;
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator<<(float x)
{
  memcpy(nextSlot,(void*)&x,sizeof(x));
  nextSlot+=sizeof(x);
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator<<(double x)
{
  memcpy(nextSlot,(void*)&x,sizeof(x));
  nextSlot+=sizeof(x);
  return *this;
}



void *MpiFixedMsg::getBuffer()
{
  return buffer;
}



void MpiFixedMsg::extract(void *into,size_t bytes)
{
  memcpy(into,nextSlot,bytes);
  nextSlot+=bytes;
}



MpiFixedMsg &MpiFixedMsg::operator>>(int &x)
{
  extract((void*)&x,sizeof(x));
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator>>(short int &x)
{
  extract((void*)&x,sizeof(x));
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator>>(long &x)
{
  extract((void*)&x,sizeof(x));
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator>>(char &x)
{
  extract((void*)&x,sizeof(x));
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator>>(unsigned char &x)
{
  extract((void*)&x,sizeof(x));
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator>>(unsigned int &x)
{
  extract((void*)&x,sizeof(x));
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator>>(unsigned long &x)
{
  extract((void*)&x,sizeof(x));
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator>>(String &x)
{
  x=(const char*) buffer;
  nextSlot+=x.length()+1;
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator>>(float &x)
{
  extract((void*)&x,sizeof(x));
  return *this;
}



MpiFixedMsg &MpiFixedMsg::operator>>(double &x)
{
  extract((void*)&x,sizeof(x));
  return *this;
}



MPI_MESSAGE_TAG MpiFixedMsg::getTag()
{
  return tag;
}



void MpiFixedMsg::setSource(int ID)
{
  sourceID=ID;
}



int MpiFixedMsg::getSource() const
{
  return sourceID;
}



MpiFixedMsg &MpiFixedMsg::operator<<(const GSL::Vector &v)
{
  MpiFixedMsg &self=*this;
  int n=v.getDim();
  self<<n;
  for(int i=0 ; i<n ; ++i) self<<v[i];
  return self;
}



MpiFixedMsg &MpiFixedMsg::operator>>(GSL::Vector &v)
{
  MpiFixedMsg &self=*this;
  int n;
  self>>n;
  v.resize(n);
  for(int i=0 ; i<n ; ++i) self>>v[i];
  return self;
}




/****************************************************************
                      MpiVariableMsg methods
 ****************************************************************/

MpiVariableMsg::MpiVariableMsg(MPI_MESSAGE_TAG t)
  : MpiFixedMsg(t,0)
{
  // ctor
}



MpiVariableMsg::~MpiVariableMsg()
{
  // dtor
}



void MpiVariableMsg::append(void *p,size_t n)
{
  unsigned char *cp=p;
  for(int i=0 ; i<n ; ++i, ++cp)
    varBuf.push_back(*cp);
}



MpiFixedMsg &MpiVariableMsg::operator<<(int x)
{
  append((void*)&x,sizeof(x));
  return *this;
}



MpiFixedMsg &MpiVariableMsg::operator<<(short int x)
{
  append((void*)&x,sizeof(x));
  return *this;
}



MpiFixedMsg &MpiVariableMsg::operator<<(long x)
{
  append((void*)&x,sizeof(x));
  return *this;
}



MpiFixedMsg &MpiVariableMsg::operator<<(char x)
{
  append((void*)&x,sizeof(x));
  return *this;
}



MpiFixedMsg &MpiVariableMsg::operator<<(unsigned char x)
{
  append((void*)&x,sizeof(x));
  return *this;
}



MpiFixedMsg &MpiVariableMsg::operator<<(unsigned int x)
{
  append((void*)&x,sizeof(x));
  return *this;
}



MpiFixedMsg &MpiVariableMsg::operator<<(unsigned long x)
{
  append((void*)&x,sizeof(x));
  return *this;
}



MpiFixedMsg &MpiVariableMsg::operator<<(const String &x)
{
  return (*this)<<x.c_str();
  return *this;
}



MpiFixedMsg &MpiVariableMsg::operator<<(const char *x)
{
  append((void*)x,strlen(x)+1);
  return *this;
}



MpiFixedMsg &MpiVariableMsg::operator<<(float x)
{
  append((void*)&x,sizeof(x));
  return *this;
}



MpiFixedMsg &MpiVariableMsg::operator<<(double x)
{
  append((void*)&x,sizeof(x));
  return *this;
}



MpiFixedMsg &MpiVariableMsg::operator<<(MpiManipulator f)
{
  return f(*this);
}



void MpiVariableMsg::close()
{
  deallocate();
  int len=varBuf.size()+sizeof(tag);
  allocate(len);
  memcpy(buffer,(void*)&tag,sizeof(tag));
  unsigned char *p=(unsigned char *) buffer;
  p+=sizeof(tag);
  len-=sizeof(tag);
  for(int i=0 ; i<len ; ++i, ++p)
    *p=varBuf[i];
  varBuf.clear();
}



/****************************************************************
                           MPI methods
 ****************************************************************/

MPI::MPI(int &argc,char ***argv)
{
  MPI_Init(&argc,argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numProcesses);
  MPI_Comm_rank(MPI_COMM_WORLD,&processID);
}



MPI::~MPI()
{
  MPI_Finalize();
}



int MPI::getNumProcesses() const
{
  return numProcesses;
}



int MPI::getNumSlaves() const
{
  return numProcesses-1;
}



int MPI::getProcessID() const
{
  return processID;
}



int MPI::getIthSlaveID(int i) const
{
  return i+1;
}



void MPI::send(MpiFixedMsg &message,int toProcessID)
{
  void *buffer=message.getBuffer();
  int bufferSize=message.getBufferSize();
  //cout<<"YYY "<<bufferSize<<endl;
  const int TAG=0;
  MPI_Send(buffer,bufferSize,MPI_BYTE,toProcessID,TAG,MPI_COMM_WORLD);
}



void MPI::send(MPI_MESSAGE_TAG tag,int toProcessID)
{
  MpiFixedMsg msg(tag,0);
  send(msg,toProcessID);
}



MpiFixedMsg *MPI::waitForMessage()
{
  MPI_Status status;
  MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
  int numBytes;
  MPI_Get_count(&status,MPI_CHAR,&numBytes);
  int sourceID=status.MPI_SOURCE;
  MpiFixedMsg *message=new MpiFixedMsg((unsigned)numBytes);
  //cout<<"XXX "<<message<<" "<<numBytes<<endl;
  message->setSource(sourceID);
  //cout<<"XXX2 "<<message->getBufferSize()<<endl;
  void *buffer=message->getBuffer();
  MPI_Recv(buffer,numBytes,MPI_BYTE,MPI_ANY_SOURCE,MPI_ANY_TAG,
	   MPI_COMM_WORLD,&status);
  //cout<<"XXX3 "<<message->getBufferSize()<<endl;
  message->beginExtraction();
  //cout<<"XXX4 "<<message->getBufferSize()<<endl;
  return message;
}



/****************************************************************
                      MpiSlaveDriver methods
 ****************************************************************/

MpiSlaveDriver::MpiSlaveDriver(MPI &mpi)
  : mpi(mpi), results(NULL)
{
  numSlaves=mpi.getNumSlaves();
}



void MpiSlaveDriver::addWork(MpiFixedMsg *msg)
{
  work.enqueue(msg);
}



int MpiSlaveDriver::getAvailableSlave()
{
  if(!slaves.isEmpty()) return slaves.dequeue();
  while(1) {
    MpiFixedMsg *incoming=mpi.waitForMessage();
    if(incoming->getTag()==SLAVE_READY) {
      int slaveID=incoming->getSource();
      delete incoming;
      return slaveID;
    }
    else results.push_back(incoming);
  }
}



void MpiSlaveDriver::waitForResults(Vector<MpiFixedMsg*> &r)
{
  results.clear();
  while(!work.isEmpty()) {
    MpiFixedMsg *outgoing=work.dequeue();
    int slaveID=getAvailableSlave();
    mpi.send(*outgoing,slaveID);
    delete outgoing;
  }
  while(slaves.size()<numSlaves) {
    MpiFixedMsg *incoming=mpi.waitForMessage();
    if(incoming->getTag()==SLAVE_READY) {
      int slaveID=incoming->getSource();
      delete incoming;
      slaves.enqueue(slaveID);
    }
    else {
      results.push_back(incoming);
    }
  }
  r=results;
}



void MpiSlaveDriver::replyToMaster(MpiFixedMsg *msg)
{
  mpi.send(*msg,0);
  delete msg;//###
}



void MpiSlaveDriver::terminateSlaves()
{
  for(int i=0 ; i<numSlaves ; ++i) {
    int slaveID=mpi.getIthSlaveID(i);
    mpi.send(TERMINATE_SLAVE,slaveID);
  }
}



MpiFixedMsg *MpiSlaveDriver::acceptWork()
{
  mpi.send(SLAVE_READY,0);
  return mpi.waitForMessage();
}



