cmake_minimum_required(VERSION 3.7)
project(pthreads_concurrent_hashing)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        hashchain.cc
        hashchain.h
        ptest.cc
        rwlock.h phash.cc)

add_executable(pthreads_concurrent_hashing ${SOURCE_FILES})