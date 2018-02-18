# Databases

Our teacher had implemented a small relational database stub, where each table has exactly one column.  
This database followed *Volcano* architecture: each operation (select, join, project, ...) in the query tree
is an iterator that erupts resulting tuples. 

Students were to implement several features (I give links to code written by me):
- instead of iterating tuple-by-tuple, results are sent in batches. Each operation can control it's batch size. It 
improves overall performance. Relevant code is in every implementation of relational operations (files `p(something)node.cpp`) [here](https://github.com/ItsLastDay/academic_university_2016-2018/tree/master/Databases/task23/dbmse/engine/demo));
- add variations of `join` operation:
   - [merge join](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/Databases/task23/dbmse/engine/demo/pmergejoin.cpp): if both relations are sorted, we can do O(n + m) join
   - [hash join](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/Databases/task23/dbmse/engine/demo/phashjoin.cpp): hash one relation, then iterate over the other. Takes O(n) memory, O(n + m) time
   - [loop join](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/Databases/task23/dbmse/engine/demo/pjoinnode.cpp): simple O(n*m) join
- select variation of `join` operation based on additional information about input relations: [code](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/Databases/task23/dbmse/engine/demo/demo.cpp#L83-L92)
- calculate *histogram* for `select` operations: i.e. approximate distribution of values in the relation: [code](https://github.com/ItsLastDay/academic_university_2016-2018/blob/master/Databases/task23/dbmse/engine/demo/histogram.cpp)
