# C++

From basic topics to fairly advanced, our C++ course was tough. I wrote a lot of code for seminar tasks as well as homeworks and tests.

Topics covered: compiling and linking code, overload resolution, calling conventions, SFINAE, move semantics, decltype, metaprogramming, variadic templates, ,
visitor and singleton design patterns, multithreading, serialization, static and dynamic libraries, memory management, smart pointers, 
exception guarantees, ... .

Interesting tasks (with links to my implementation):
- [thread pool](https://github.com/ItsLastDay/academic_university_2016-2018/tree/master/C%2B%2B/semester_2/hw3/src): submit tasks to fixed pool of threads
- [caching allocator](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/C%2B%2B/semester_2/exam2/src/au_allocator.h) for small objects
- [big integer](https://github.com/ItsLastDay/academic_university_2016-2018/tree/master/C%2B%2B/semester_1/hw2) library. Supports sharing of internal buffers between instances
- [any](https://github.com/ItsLastDay/academic_university_2016-2018/tree/master/C%2B%2B/semester_1/hw3/src) (like boost::any)
- [graph](https://github.com/ItsLastDay/academic_university_2016-2018/tree/master/C%2B%2B/semester_2/hw1/src) library. Supports filtering edges\vertices by predicate, finding shortest paths and invoke callback along it.
- [huffman algorithm](https://github.com/ItsLastDay/academic_university_2016-2018/tree/master/C%2B%2B/semester_1/hw1/src) with serialization of resulting tree
- simple [associative array](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/C%2B%2B/semester_1/test_3/src/au_map.h) (like std::map), but with worse complexity
- simple [deque](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/C%2B%2B/semester_1/test_4/src/deque.h) with strict requirements to time\space complexity
- simple [signal](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/C%2B%2B/semester_2/test_5/3/src/signal.hpp) library: register and invoke callbacks
- simple [serialization](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/C%2B%2B/semester_2/test_6/src/serialize_args.hpp) library
