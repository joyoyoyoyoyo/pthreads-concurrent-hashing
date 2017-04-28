//#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

class RWLock {
private:
#ifdef RWLOCK
  pthread_mutex_t rwlock;
#else
  pthread_mutex_t counter = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t reading = PTHREAD_COND_INITIALIZER;  // 0 or more
  pthread_cond_t writing = PTHREAD_COND_INITIALIZER;  // 0 or 1
  int bucket_counter;
  int num_readers;
#endif
//  // Express state of shared resources
//  free: resource_counter = 0;
//  reading: resource_counter > 0;
//  writing: resources_counter = -1;
//  // reflect the state of the current resources
//  // who gets to update this proxy variable
public:
  RWLock();

  ~RWLock();

  //Reader
  void startRead();

  void doneRead();

  // Writer
  void startWrite();

  void doneWrite();
};

