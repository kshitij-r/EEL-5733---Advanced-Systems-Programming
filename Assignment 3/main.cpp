#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include "header.hpp"


using namespace std;

std::vector<long int> accountList;
static long int assignWorkers(const char *fileName, threadData_t *threadData,bankAccountPool_t &accountPool, long int NumberOfThreads, long int &requestCount)
{
  std::ifstream fileStream;
  std::stringstream stringParser;
  char line[buffer] = { 0 };
  long int accountNumber = -1, initBalance = 0;
  long int fromAccount = -1, toAccount = -1, transferAmount = 0;
  std::string transferString;
/*if(!fileStream.is_open())
  {
    dbg_trace("Failed to open the file: " << fileName);
    return FAIL;
  }
*/

  bool initDone = false;
  long int assignID = -1;
  fileStream.open(fileName, std::ifstream::in);
  if(!fileStream.is_open())
  {
    dbg_trace("Failed to open the file: " << fileName);
    return FAIL;
  }
  while(fileStream.good())
  {
    fileStream.getline(line, buffer);
   //stringParser >> accountNumber >> initBalance;          
    dbg_trace("String: " << line);
    if (isalpha(line[0]) && line[0]=='T' )
    {
      initDone = true;
    }
/*
long int fromAccount = -1, toAccount = -1, transferAmount = 0;
  long int assignID = lastAssignedID;
  long int requestCount = 0;
  fromAccount = -1;
*/
    stringParser.str(line);       
    if(!initDone)
    {
      stringParser >> accountNumber >> initBalance;
      dbg_trace("Account Number: " << accountNumber << " , " << "Init Balance: " << initBalance);

      if(accountNumber == -1)
      {
        goto CLEAR;
      }
      accountList.push_back(accountNumber);
      accountPool.emplace(std::make_pair(accountNumber,bankAccount(accountNumber, initBalance)));

      dbg_trace("POOL: Account Number: " << accountPool[accountNumber].getAccountNumber() << " , " << "Init Balance: " << accountPool[accountNumber].getBalance());
    }
    else
    {
      
      stringParser >> transferString >> fromAccount >> toAccount >> transferAmount;
      dbg_trace("From: " << fromAccount <<  " To: " << toAccount << " Amount: " << transferAmount);

      
      if(fromAccount == -1 || toAccount == -1)
      {
        goto CLEAR;
      }

      
      assignID = (assignID + 1) % NumberOfThreads;
      ++requestCount;

      assert(threadData[assignID].threadID == assignID);   
      assert(threadData[assignID].threadID  == threadData[assignID].EFTRequests.getWorkerID());
      EFTRequest_t* newRequest = new EFTRequest_t();
      newRequest->workerID = assignID;
/*
typedef std::map<long int, bankAccount_t> bankAccountPool_t;
typedef bankAccountPool_t::iterator bankAccountIterator_t;
newRequest->fromAccount = toAccount;
        newRequest->toAccount = fromAccount;
      newRequest->transferAmount = transferAmount;
      
*/
     				 newRequest->fromAccount = fromAccount;
     				 newRequest->toAccount = toAccount;
     			 newRequest->transferAmount = transferAmount;
      	threadData[assignID].EFTRequests.pushRequest(newRequest);
    }

CLEAR:
    memset(line, '\0', buffer);
    stringParser.str("");
    stringParser.clear();
    accountNumber = fromAccount = toAccount = -1;
/*
bankAccount :: bankAccount() 
  {
  }
*/
    initBalance = transferAmount = 0;
  }
  if(fileStream.eof())
  {
    dbg_trace("Reached End-of-File!");
    dbg_trace("Total Transfer Requests: " << requestCount);
    askThreadsToExit(threadData, accountPool, NumberOfThreads, assignID);
  }
  else 
  {
    dbg_trace("Error while reading!");
  }
  fileStream.close();
  return SUCCESS;
}


#if 0
#endif

static void printAccounts(bankAccountPool_t &accountPool)
{
  std::vector<long int>::iterator i;
  for(i = accountList.begin(); i != accountList.end(); ++i)
  {
    print_output(*i << " " << accountPool[*i].getBalance());
  }
}


int main(int argc, char const *argv[])
{
  if(argc != 3)
  {
    print_output("Please enter arguments in the following formats:");
    print_output("\t./transfProg <PathToInputFile> <NumberOfThreads>");
    return 0;
  }
  long int fileStatus = access(argv[1], F_OK | R_OK);
  if(fileStatus != 0)
  {
    print_output("Failed to access the input file or file doesn't exist!");
    print_output("Please check the path to the input file is correct.");
    return 0;
  }
/*
long int threadid = atoi((const char *) argv[2]);
  if(workerThreads < 1 || workerThreads > max_threads){
    print_output("Invalid number of workers: " << workerThreads << "\nEnter buffer size between 1 to " << max_threads);
    return 0;
  }
  bankAccountPool_t accountPool;                
  threadData_t threadData[workerThreads];       
  

*/
  long int workerThreads = atoi((const char *) argv[2]);
  if(workerThreads < 1 || workerThreads > max_threads){
    print_output("Invalid number of workers: " << workerThreads << "\nEnter buffer size between 1 to " << max_threads);
    return 0;
  }
  bankAccountPool_t accountPool;                
  threadData_t threadData[workerThreads];       
  pthread_t threads[workerThreads];             
  long int EFTRequestsCount = 0;
/*
 long int parseStatus = assignWorkers(argv[1], threadData, accountPool, workerThreads, EFTRequestsCount);
  if(parseStatus == FAIL)
  {
 
*/

  bool status = spawnThreads(threads, threadData, &accountPool, workerThreads);
  if(status == FAIL)
  {
    dbg_trace("Failure in creating threads!");
    return 0;
  }

 
  long int parseStatus = assignWorkers(argv[1], threadData, accountPool, workerThreads, EFTRequestsCount);
  if(parseStatus == FAIL)
  {
    print_output("Eror during parsing!");
    return 0;
  }

 
  for(long int i=0; i<workerThreads; i++)
  {
    pthread_join(threads[i], NULL);
  }
  printAccounts(accountPool);
  return 0;
}















