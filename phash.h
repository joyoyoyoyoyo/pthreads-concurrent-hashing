// Course grain locking based concurrent hash table.
// Created by Angel Ortega on 4/25/17.
//
#ifndef PTHREADS_CONCURRENT_HASHING_PHASH_H
#define PTHREADS_CONCURRENT_HASHING_PHASH_H
#include <pthread.h>
#include "rwlock.h"

class LinkedHashEntry {
private:
  int key;
  int value;
  LinkedHashEntry *next;
public:
  LinkedHashEntry(int key, int value);
  int getKey();
  int getValue();
  void setValue(int value);

  LinkedHashEntry *getNext();
  void setNext(LinkedHashEntry *next);
};


class HashMap {
private:
  LinkedHashEntry **table;
#ifdef RWLOCK
  #ifdef FINEGRAIN
    RWLock **locks;
  #else
    RWLock *lock;
  #endif
#else
  #ifdef FINEGRAIN
    pthread_mutex_t* fine_mutexes;
  #else
    pthread_mutex_t coarse_mutex = PTHREAD_MUTEX_INITIALIZER;
  #endif
#endif
public:
  HashMap();
  int get(int key);
  void put(int key, int value);
  void remove(int key);
  ~HashMap();
};

#endif //PTHREADS_CONCURRENT_HASHING_PHASH_H
