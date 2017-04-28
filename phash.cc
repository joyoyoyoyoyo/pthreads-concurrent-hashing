// Course grain locking based concurrent hash table.
// Created by Angel Ortega on 4/24/17.

#include <iostream>
#include "phash.h"

LinkedHashEntry::LinkedHashEntry(int key, int value) {
  this->key = key;
  this->value = value;
  this->next = NULL;
}

int
LinkedHashEntry::getKey() {
  return key;
}

int
LinkedHashEntry::getValue() {
  return value;
}

void
LinkedHashEntry::setValue(int value) {
  this->value = value;
}


LinkedHashEntry *
LinkedHashEntry::getNext() {
  return next;
}

void
LinkedHashEntry::setNext(LinkedHashEntry *next) {
  this->next = next;
}


const int TABLE_SIZE = 128;



#ifdef RWLOCK

// Coarse Read-Write Lock
HashMap::HashMap() {
  lock = new RWLock();
  table = new LinkedHashEntry *[TABLE_SIZE];
  for (int i = 0; i < TABLE_SIZE; i++)
    table[i] = NULL;
}

int
HashMap::get(int key) {
  lock->startRead();
  int hash = (key % TABLE_SIZE);
  if (table[hash] == NULL) {
    lock->doneRead();
    return -1;
  }
  else {
    LinkedHashEntry *entry = table[hash];
    while (entry != NULL && entry->getKey() != key)
      entry = entry->getNext();
    if (entry == NULL) {
      lock->doneRead();
      return -1;
    }
    else {
      lock->doneRead();
      return entry->getValue();
    }
  }
}

void
HashMap::put(int key, int value) {
  lock->startWrite();
  int hash = (key % TABLE_SIZE);
  if (table[hash] == NULL) {
    table[hash] = new LinkedHashEntry(key, value);
    lock->doneWrite();
  }
  else {
    LinkedHashEntry *entry = table[hash];
    while (entry->getNext() != NULL)
      entry = entry->getNext();
    if (entry->getKey() == key)
      entry->setValue(value);
    else
      entry->setNext(new LinkedHashEntry(key, value));
    lock->doneWrite();
  }
}


void
HashMap::remove(int key) {
  lock->startWrite();
  int hash = (key % TABLE_SIZE);
  if (table[hash] != NULL) {
    LinkedHashEntry *prevEntry = NULL;
    LinkedHashEntry *entry = table[hash];
    while (entry->getNext() != NULL && entry->getKey() != key) {
      prevEntry = entry;
      entry = entry->getNext();
    }
    if (entry->getKey() == key) {
      if (prevEntry == NULL) {
        LinkedHashEntry *nextEntry = entry->getNext();
        delete entry;
        table[hash] = nextEntry;
      } else {
        LinkedHashEntry *next = entry->getNext();
        delete entry;
        prevEntry->setNext(next);
      }
    }
  }
  lock->doneWrite();
}

HashMap::~HashMap() {
  lock->startWrite();
  for (int i = 0; i < TABLE_SIZE; i++)
    if (table[i] != NULL) {
      LinkedHashEntry *prevEntry = NULL;
      LinkedHashEntry *entry = table[i];
      while (entry != NULL) {
        prevEntry = entry;
        entry = entry->getNext();
        delete prevEntry;
      }
    }
  delete[] table;

  // clean up our resources
  lock->doneWrite();
  delete lock;
}

#else
// Coarse without Read-Write lock
HashMap::HashMap() {
  table = new LinkedHashEntry *[TABLE_SIZE];
  for (int i = 0; i < TABLE_SIZE; i++)
    table[i] = NULL;
}

int
HashMap::get(int key) {
  pthread_mutex_lock(&coarse_mutex);
  int hash = (key % TABLE_SIZE);
  if (table[hash] == NULL) {
    pthread_mutex_unlock(&coarse_mutex);
    return -1;
  }
  else {
    LinkedHashEntry *entry = table[hash];
    while (entry != NULL && entry->getKey() != key)
      entry = entry->getNext();
    if (entry == NULL) {
      pthread_mutex_unlock(&coarse_mutex);
      return -1;
    }
    else {
      pthread_mutex_unlock(&coarse_mutex);
      return entry->getValue();
    }
  }
}

void
HashMap::put(int key, int value) {
  pthread_mutex_lock(&coarse_mutex);
  int hash = (key % TABLE_SIZE);
  if (table[hash] == NULL) {
    table[hash] = new LinkedHashEntry(key, value);
    pthread_mutex_unlock(&coarse_mutex);
  }
  else {
    LinkedHashEntry *entry = table[hash];
    while (entry->getNext() != NULL)
      entry = entry->getNext();
    if (entry->getKey() == key)
      entry->setValue(value);
    else
      entry->setNext(new LinkedHashEntry(key, value));
    pthread_mutex_unlock(&coarse_mutex);
  }
}


void
HashMap::remove(int key) {
  pthread_mutex_lock(&coarse_mutex);
  int hash = (key % TABLE_SIZE);
  if (table[hash] != NULL) {
    LinkedHashEntry *prevEntry = NULL;
    LinkedHashEntry *entry = table[hash];
    while (entry->getNext() != NULL && entry->getKey() != key) {
      prevEntry = entry;
      entry = entry->getNext();
    }
    if (entry->getKey() == key) {
      if (prevEntry == NULL) {
        LinkedHashEntry *nextEntry = entry->getNext();
        delete entry;
        table[hash] = nextEntry;
      } else {
        LinkedHashEntry *next = entry->getNext();
        delete entry;
        prevEntry->setNext(next);
      }
    }
  }
  pthread_mutex_unlock(&coarse_mutex);
}

HashMap::~HashMap() {
  pthread_mutex_lock(&coarse_mutex);
  for (int i = 0; i < TABLE_SIZE; i++)
    if (table[i] != NULL) {
      LinkedHashEntry *prevEntry = NULL;
      LinkedHashEntry *entry = table[i];
      while (entry != NULL) {
        prevEntry = entry;
        entry = entry->getNext();
        delete prevEntry;
      }
    }
  delete[] table;

  // clean up our resources
  pthread_mutex_unlock(&coarse_mutex);
  pthread_mutex_destroy(&coarse_mutex);
  pthread_exit(NULL);
}
#endif