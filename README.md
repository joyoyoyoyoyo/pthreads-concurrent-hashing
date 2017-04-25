# pthreads-concurrent-hashing

## Goals:
- Implement a Concurrent Hash Table with Pthreads
    - add synchronization to:
        - put()
        - get()
        - remove()

- Course-grain Locking on a Concurrent Hash Table
- Fine-grain Locking on a Concurrent Hash Table
- Use a mutex 
- Implement and use a read-write lock for t he entire hash table using a Pthread mutex and condition variable
following the RWLock design in the text book.
- Allocate an array of mutex locks and use each to a corresponding hash bucket (this synchronizes access to the linked list in the bucket)
- Allocate an array of mutex locks for concurrent access to each hash table entry (multiple iterators/readers+writers)

## Objective: 
Understand pthreads and basic synchronization primitives.


Goals: 