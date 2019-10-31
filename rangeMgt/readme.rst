.. highlight:: rst


Readme for Range Management Library
############################################################
Jack Lee (jacklee032016@gmail.com)  10, 2019



Design and Algorithm
============================================

Design principles
---------------------------------------------------------
In my opinion, the readability and easiness of maitenance of source code are more inportant
than the algorithm optimization, because the fact that CPU is good enough for most of 
applications.


Algorithm
---------------------------------------------------------

Based on the fact that all ranges are ordered, Binary Search Tree is used to store 
and search the ranges, and no duplicated or overlapped item can be stored in the 
tree, the **start** field of range is used as the key to sort all ranges. 

The Average search depth of binary tree is O(logN).

Design and implementation
---------------------------------------------------------

For the comparason of 2 ranges, normally the relationship between ant 2 ranges can be divided
into following type:

#. Smaller
#. Left Overlapped;
#. Right Overlapped;
#. Full Overlapped;
#. Underlapped;
#. Equal;
#. Bigger;

All operations of Add/Delete/Get are based on the result of comparason between the new range
and current range. 

Maybe some relation types can be merged into one type in some operations, but for easiness of 
understanding of the code and simplifying the complexity of implementation, I always explicitly
keep all these types in all of the 3 operations.


| **C Language Version**


In the version of C language, a Binary Tree is implemented. The key of algorithm 
performance is how to find the first available item in the range tree, which is 
first item related the argument of **start** and **end**;  after the first 
available range has been found (may be null), all operations (Add/Delete/Get) will 
only need to access the neighbour items with simple iterating loop to a few items,
which is not involving in performance of algorithm.

So algorithm optimization only need to be focused on how to find first available item.
Now only a simple access from beginning of tree is implemented because of limitation 
of time. 

To improve performance, more data about the tree can be defined. For example, we can
track some important points of the tree, such as the postion of head, middle and last
items of the tree when range is added and deleted; when searching the first available
item, we compare these points at first with argument of ``start``, then begin the next
search from these point in right direct.

| **C++ Language Version**

I think, there are 3 advantages in using c++;

* First, we don't need invent a wheel again. There are a lot of optimized algorithms has been implemented in STL, and can be easily used directly, simplfying the complexity of implementation.

* Second, With support of template, the Range can be anything, which can be compared and sorted, such as integer, float, complex number and other things. So I insist on using template in definition of range and its operation in C++.

* Third, C++ is very easy to implement and test the functions, there are a lot of easily used test framework to implement TDD (Test Driving Development). For example, catch2 is used in my software to finish the unit tests and integrated tests, without knowing the details about catch2.


For C++ version, all operations and data structures are based on the **map** from STL.
**map** is the enhancement of binary tree which provides the unique key management
and sort/search services.

So I use C++ and map in implementing searching and add/delete basic data structure 
in this software.

Of course, I can't say the so-called optimized algorithm of binary tree from STL is 
most suitable for implementing range management. So, I also provide another version
of C language, which nearly implementing everything from scratch.
 


Usage
============================================

All source code can be built without any warns with GCC 4.9 in ubuntu 14.4(64bit)
and GCC 6.4.1 in Fedora 25.32_1(32bit).

Only ANCI C and STL C++11 are used, so it can be easy to be built with other 
toolchains, such as VC++.

| **APIs**:

* C version: refer to c/include/rangeSet.h
* C++ version: refer to cpp/include/rangesApi.h


In this root directory of this package:

Build
---------------------------------------------------------

::
 
    make
	

Tests
---------------------------------------------------------

::
 
	./Linux.bin.X86/usr/bin/testTree: run test program for C language version
	
	./test.sh: run all test cases for C++ version
	
	./Linux.bin.X86/usr/bin/demo: run C demo program which call C++ libariy



Variables define the compilation
---------------------------------------------------------
Modify following variables can define how to compile the software:

| **EDITION**

In **$ROOT/Rules.mak**, modify as **EDITION=debug** or **EDITION=release** to compile 
the debug or release version;

| **ARCH**

In **$ROOT/Makefile**, define as **ARCH** or **ARCH=arm** to compile native or cross
platform version. For native version, all libaries are compiled as static to make 
test/debug more easier; for cross platform version, they are shared libaries to 
save space.
