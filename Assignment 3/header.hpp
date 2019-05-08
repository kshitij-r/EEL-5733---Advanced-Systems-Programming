#ifndef __DEBUG_MACROS__
#define __DEBUG_MACROS__
#ifndef __EFT_TRANSFER__
#define __EFT_TRANSFER__

#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#define max_threads 50
#define buffer 50

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif
#define SUCCESS 0
#define FAIL    1
#define dbg_trace( print_message ) \
        do { if(DEBUG_TEST) \
            std::cerr << \
            "DEBUG" << "(" << __LINE__ << "):" << __func__ << "():" \
            << std::setw(40) << print_message << std::endl; \
        } while(0)
#define print_output( message ) \
        do { std::cout << message << std::endl; \
        } while(0)
#endif
typedef class bankAccount
{
public:
  bankAccount();         
  bankAccount(long int acc, long int bal); 
  ~bankAccount();                   
  long int lock();                  
  long int trylock();               
  long int unlock();                
  long int getAccountNumber();      
  long int getBalance();            
  void setBalance(long int newBalance);

private:
  long int number;    
  long int balance;   
  pthread_mutex_t mutex; 

} bankAccount_t;

typedef struct EFTRequest {
  long int workerID;
  long int fromAccount;
  long int toAccount;
  long int transferAmount;
} EFTRequest_t;

typedef class workerQueue
{
public:
  workerQueue();                           
  ~workerQueue();                          
  long int getWorkerID();                  
  void setWorkerID(long int ID);           
  void pushRequest(EFTRequest_t *request); 
  EFTRequest_t *popRequest();              

private:
  long int workerID;
  std::queue<EFTRequest_t*> Queue;
  sem_t goodToRead;                        
  sem_t mutex;                             

} workerQueue_t;

typedef std::map<long int, bankAccount_t> bankAccountPool_t;
typedef bankAccountPool_t::iterator bankAccountIterator_t;


typedef struct threadData {
  long int threadID;                             
  workerQueue_t EFTRequests;                
  bankAccountPool_t *accountPool;           
} threadData_t;

long int spawnThreads(pthread_t *threads, threadData_t *threadDataPool, \
  bankAccountPool_t *accountPool, long int NumberOfThreads);
void askThreadsToExit(threadData_t *threadData, bankAccountPool_t &accountPool,\
   long int NumberOfThreads, long int lastAssignedID);

#endif
