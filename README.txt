 The list of files in the tar ball.

1. hashchain.h
This file contains class specifications for the hashmap implementations.
No locking implementation.

2. hashchain.cc
This file contains HashMap implementations. No locking declarations.

3. phash.h
This file is same as hashchain.h but with one difference. This file should
defines a RWLOCK as a member of class HashMap

4. rwlock.h
This file contains declaration for RW lock.

5. rwlock.cc
This file contains definitions for RW lock

6. ptest.cc
This is the test program which tests synchronized access to HashMap.

7. Makefile
This is used to produce four binaries as described previously.




