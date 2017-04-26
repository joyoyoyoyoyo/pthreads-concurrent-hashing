// Course grain locking based concurrent hash table.
// Created by Angel Ortega on 4/24/17.


#include <iostream>
#include "phash.h"

pthread_mutex_t coarse_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t coarse_condition = PTHREAD_COND_INITIALIZER;

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
  pthread_mutex_unlock(&coarse_mutex);

}


