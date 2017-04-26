//
// Created by Angel Ortega on 4/25/17.
//

#ifndef PTHREADS_CONCURRENT_HASHING_PHASH1_H
#define PTHREADS_CONCURRENT_HASHING_PHASH1_H
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
public:
  HashMap();
  int get(int key);
  void put(int key, int value);
  void remove(int key);
  ~HashMap();
};

#endif //PTHREADS_CONCURRENT_HASHING_PHASH1_H
