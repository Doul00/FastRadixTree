Project TMLN 1
==============

Group Members
=============

See AUTHORS.

Language & dependencies
=======================

Language:
    * C++

No extern dependencies.

Type "make" to compile and generate the two binaries.

Questions
=========

------ 1. Project's choices

At first we started developping the project in Golang. Mainly because we
wanted to dive in this language, but also because Sylvain Utard was eager to
see someone tackle this problem in Golang, and finaly because we were
expecting Golang to be very efficient.

We did the project, from A to Z, in Golang but even with all possible
optimizations we couldn't get under the limit of 512mo of RAM: At first we were
around 2go with a basic Trie. Then we wrote a word insertion allowing a node
to store more than one character, the RAM usage dropped down to 1go. Finaly
we made some minor optimizations to the Trie structure and we landed ourselves
around 700-800mo of RAM. No way to get under that. Probably because of the
internal duplication made by its garbage collector.

Thus, we decided to rewrite everything in C++. At first we wanted to use
boost::serialization but the search was too slow because of the long time
taken by the `dict.bin` loading. Therefore we wrote our own serialization,
octet by octet. Then we wrote some kind of API to access the underlying Trie
with the help of `mmap` syscall.

To sum up, the steps are:
 - Creation of a a compressed trie
 - Custom serialization of the trie into a file
 - When the App is started, the binary file is mmaped
 - For each query, pointer arithmetic is used to read the values quickly and
 return the results. 

------ 2. Tests

One python script, called `benchmark.py`, that give an estimate of the qps.
Another script, called `testsuite.py`, that compare the output of the refence
binary with ours. The words used for the tests are found randomly in
`words.txt`.
- type "make test" to run the ouput comparison.
 By default, 100 words are picked and tested for distance 0, 1 and 2.
 Please see the Makefile to specify the args.

------ 3. Found case not working?

No, not really. Should we?

------ 4. Data structure

Trie
 |  uint32_t frequency          # If frequency is == 0, the node isn't a word.
 |  std::string value
 |  std::vector<Trie> *children
 |  unsigned long offset        # Used only for the serialization's part.

We choose `uint32_t` for the frequency because that was the small type that is
able to store any frequency found in `words.txt`.

An `std::string` may not be the most efficient choice for the value, maybe a
`char*` would have been better, but its use was easier.

A pointer to a vector of Trie, quite basic. Pointer to avoid performance loss
when resizing the vector.

An `unsigned long` offset to store the offset for the serialization.

For the serialization:
The following structure is used for each node.
The fields are written one after the other.

| frequency -> uint32_t
| child_number -> uint32_t
| value -> char* of variable length
| brother's offset -> unsigned long 

For each node, its first child is written just after its struct.
The brother's offset is used to iterate through the child.
The structure is a kind of left-child right-sibling Tree.

------ 5. Automatic distance choice
 For the 0 distance, we used an exact search algorithm.
 The algorithmwas going from node to child if the child matched.
 If the word was not found, an empty array was returned.

 For the other distances, a modified version of the algorithm seen in class was used.
 At each node, if the current distance was below the maximum one,
 the insertion, deletion ,substitution and transposition operations were performed.
 If a node containing a word was reached and the distance was below the max,
 the value was inserted.

------ 6. Enhance performances

 In order to perform better, the binary file should be directly written, instead of
 building the trie first.
 Also, we should separate the words from the rest of the structure in the serialization. 

------ 7. What's missing to be state-of-the-art?
 All the optimizations cited above would be necessary.
 Maybe using a Bloom Filter for the exact search would be more
 performant.