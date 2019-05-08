#include <stdlib.h>
#include <iostream>
#include <memory>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
#include "header.hpp"

using namespace std;

static void *EFTWorker(void *data)
{
  threadData_t *workerData = (threadData_t *) data;
  EFTRequest_t *requestToProcess = NULL;

  while((requestToProcess = workerData->EFTRequests.popRequest()) != NULL)
  {
    long int fromBalance = 0, toBalance = 0;
    long int fromAccount = requestToProcess->fromAccount;
    long int toAccount = requestToProcess->toAccount;
    long int transferAmount = requestToProcess->transferAmount;
    if(fromAccount == -1 || toAccount == -1){
      delete requestToProcess;
/*
if(fromAccount < toAccount)
      { 
        workerData->accountPool->at(fromAccount).lock();
        workerData->accountPool->at(toAccount).unlock();
      }
*/
      requestToProcess = NULL;
      break;
    }
                                                                         //  critical section // atomic execution
      if(fromAccount < toAccount)
      { 
        workerData->accountPool->at(fromAccount).lock();
        workerData->accountPool->at(toAccount).lock();
      }
      else
      { 
        workerData->accountPool->at(toAccount).lock();
        workerData->accountPool->at(fromAccount).lock();
      }
        // -- Calculating balance
        fromBalance = workerData->accountPool->at(fromAccount).getBalance();
        toBalance = workerData->accountPool->at(toAccount).getBalance();
        // -- updating new balnce to respective account
        workerData->accountPool->at(fromAccount).setBalance(fromBalance - transferAmount);
        workerData->accountPool->at(toAccount).setBalance(toBalance + transferAmount);
/*
workerData->accountPool-from(fromAccount).getBalance(toBalance - transferAmount);
        workerData->accountPool->at(toAccount).setBalance(toBalance + transferAmount);
*/
      if(fromAccount < toAccount)
      { 
        workerData->accountPool->at(toAccount).unlock();
        workerData->accountPool->at(fromAccount).unlock();
      }
      else
      { 
        workerData->accountPool->at(fromAccount).unlock();
        workerData->accountPool->at(toAccount).unlock();
      }
    delete requestToProcess;
    requestToProcess = NULL;
  }
  dbg_trace("THREAD: " << workerData->threadID << " EXIT!");
  pthread_exit(NULL);
}
                                                                                // creating new threads 
long int spawnThreads(pthread_t *threads, threadData_t *threadDataPool, bankAccountPool_t *accountPool, long int NumberOfThreads)
{
  threadData_t *threadPool = threadDataPool;
  pthread_t *threadID = threads;
  bool spawnThreadsStatus = FAIL;
  long int thread = 0;
  for(thread = 0; thread < NumberOfThreads; thread++)
  {
    threadPool[thread].threadID = thread;
    threadPool[thread].EFTRequests.setWorkerID(thread);
/*
  threadData_t *threadPool = threadDataPool;
  pthread_t *threadID = threads;
*/
    threadPool[thread].accountPool = accountPool;
    long int status = pthread_create(&threadID[thread], NULL, &EFTWorker, (void*) &threadPool[thread]);
    if(status != 0)
    {
      print_output("unable to form thread: " << thread);
      exit(1);
    }
  }
  if(thread == NumberOfThreads){
    spawnThreadsStatus = SUCCESS;
  }
  return spawnThreadsStatus;
}
                                                                                 //terminate threads
void askThreadsToExit(threadData_t *threadData, bankAccountPool_t &accountPool, long int NumberOfThreads, long int lastAssignedID)
{
  long int fromAccount = -1, toAccount = -1, transferAmount = 0;
  long int assignID = lastAssignedID;
  long int requestCount = 0;
  fromAccount = -1;
  toAccount = -1;
  transferAmount = 0;
      if(lastAssignedID == -1 || NumberOfThreads < 0){
        return;
  }

  do 
{
      assignID = (assignID + 1) % NumberOfThreads;
      ++requestCount;
      assert(threadData[assignID].threadID == threadData[assignID].EFTRequests.getWorkerID());
      EFTRequest_t* newRequest = new EFTRequest_t();
      
newRequest->workerID = assignID;
/*
      newRequest->fromAccount = fromAccount;
      newRequest->transferAmount = transferAmount;
*/
      newRequest->fromAccount = fromAccount;
      newRequest->toAccount = toAccount;
      newRequest->transferAmount = transferAmount;
      threadData[assignID].EFTRequests.pushRequest(newRequest);
  } 
  while(assignID != lastAssignedID);
}
//
bankAccount :: bankAccount(long int accountNumber, long int initBalance)
{
  this->number = accountNumber;
  this->balance = initBalance;
  bool mutexStatus = pthread_mutex_init(&mutex, NULL);
  if(mutexStatus != 0)
  {
    print_output("Mutex init failed: "<< "Acc: " << accountNumber << " , "<< "Balance: " << initBalance);
    exit(1);
  }
}
bankAccount :: bankAccount() 
  {
  }
bankAccount :: ~bankAccount()
  {
      
if(pthread_mutex_unlock(&mutex) != 0){
      //dbg_trace("locked!");
      dbg_trace("Already Unlocked!");
  }
  pthread_mutex_destroy(&mutex);
}
long int bankAccount :: lock()  //account access lock
  {
    return pthread_mutex_lock(&mutex);
  }
long int bankAccount :: trylock()
  {
     return pthread_mutex_trylock(&mutex);
  }
long int bankAccount :: unlock()
  {
    return pthread_mutex_unlock(&mutex);
  }
long int bankAccount :: getBalance() //get account balalce
  {
    return this->balance;
  }
long int bankAccount :: getAccountNumber() //get accoutn number
  {
    return this->number;
  }
void bankAccount :: setBalance(long int newBalance)
  {
    this->balance = newBalance;                        // updating the new balance
  }
workerQueue :: workerQueue()
    {
    this->workerID = -1;
    bool semStatus = sem_init(&goodToRead, 0, 0);   
    if(semStatus != 0)
      {
        print_output("Sem initalization failed! Worker ID: " << workerID);
        exit(1);
      }

  bool mutexStatus = sem_init(&mutex, 0, 1);
  if(mutexStatus != 0)
  {
    print_output("Mutex initialization failed! Worker ID: " << workerID);
    exit(1);
  }
}
workerQueue :: ~workerQueue()
{
  sem_destroy(&this->mutex);
  sem_destroy(&this->goodToRead);
}
  long int workerQueue :: getWorkerID()
    {
      return this->workerID;
    }
void workerQueue :: setWorkerID(long int ID)
  {
     this->workerID = ID; 
  }
void workerQueue :: pushRequest(EFTRequest_t* newRequest)
  {
    sem_wait(&this->mutex);
    this->Queue.push(newRequest);             
    sem_post(&this->mutex);
    sem_post(&this->goodToRead);              
  }
EFTRequest_t* workerQueue :: popRequest()
    {
      EFTRequest_t *request = NULL;
      sem_wait(&this->goodToRead);
      sem_wait(&this->mutex);
      request = this->Queue.front();
      this->Queue.pop();
      sem_post(&this->mutex);
      return request;
    }
