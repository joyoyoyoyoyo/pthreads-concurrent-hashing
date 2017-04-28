//
// Created by Angel Ortega on 4/25/17.
//

#include "rwlock.h"

#ifdef RWLOCK

RWLock::RWLock(): bucket_counter(0), num_readers(0) {

}

RWLock::~RWLock() {
  // clean up our resources
  pthread_mutex_unlock(&counter);
  pthread_mutex_destroy(&counter);
  pthread_cond_destroy(&reading);
  pthread_cond_destroy(&writing);
  pthread_exit(NULL);
}

// Enter Critical Section
void RWLock::startRead() {
  // start reading, check if conditions hold first
  pthread_mutex_lock(&counter);  // lock
  while (bucket_counter == -1)  // make sure we are at a state to begin reading
    pthread_cond_wait(&reading, &counter);  // a final resource_counter check will be performed in case a writer has beat the current thread to the write operation
  bucket_counter++;
  pthread_mutex_unlock(&counter);  // unlock
}

// Exit Critical Section
void RWLock::doneRead(){
  // done reading, now update counter and conditions
  pthread_mutex_lock(&counter);  // lock
  bucket_counter--;
  if (num_readers == 0) // is this the last reader?
    pthread_cond_wait(&writing, &counter);  // signal to the writer that there are no readers performing read operations
  // broadcast would not make sense, because only one writer can go at a time
  // any writers in queue will come out of the wait operation
  pthread_mutex_unlock(&counter);  //unlock
}

// Enter Critical Section
void RWLock::startWrite(){
  // Before we write: make sure that conditions to write hold;
  pthread_mutex_lock(&counter);  // lock
  while (bucket_counter != 0)
    pthread_cond_wait(&writing, &counter); // mutex is released and the writer is on a queue to wait to write
  bucket_counter--;
  pthread_mutex_unlock(&counter);  // unlock
}

// Exit Critical Section
void RWLock::doneWrite(){
  // terminate write and update conditions, that a write operation has terminated
  pthread_mutex_lock(&counter);
  bucket_counter = 0;
  pthread_cond_signal(&writing); // implicitly a priority to writer, but not guaranteed because of race conditions
  pthread_cond_broadcast(&reading);
  pthread_mutex_unlock(&counter);  // unlock
}


void testReaderWriterSameTimeAccess();

void testReadersMultipleReadAllow();

void testWriterZeroOrMoreAccess();

// mutex only allow one thread at a time to access a critical section at a time
// resources is free and accessible or the resources is not and you must wait
/**
// for readers, we cannot express that multiple readers can access the same resources
//        at the same time with a mutex
**/

// we will simplify based of how many


//  if (read_conter == 0 && write_counter == 0) {
//    // Reading is OK, Writing is OK
//  }
//  if (read_counter > 0) {
//    // Reading is OK
//  }
//  if (writer.count == 1) {
//    // Reading is NO, Writing is NO
//  }

#else

#endif