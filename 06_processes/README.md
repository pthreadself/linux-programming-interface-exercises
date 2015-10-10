#Linux Proramming Interface, Chapter 6 Exercises Answers

>**Q:** 6-1. Compile the program in Listing 6-1 (mem_segments.c), and list its size using ls –l.
Although the program contains an array (mbuf) that is around 10 MB in size, the
executable file is much smaller than this. Why is this?


 **A:**
 因为mbuf是uninitilized data，处于uninitialized data segment，这个segment中的数据仅仅在program file中保存了位置信息和大小信息，并没有实际的数据，而是由program loader在将program file加载到内存中成为process的时候才进行这些数据的空间分配和初始化的（初始化成0），在加载之前并没有进行实际的空间分配。

--- 

 >**Q:** 6-3. Implement setenv() and unsetenv() using getenv(), putenv(), and, where necessary, code that directly modifies environ. Your version of unsetenv() should check to see whether there are multiple definitions of an environment variable, and remove them all (which is what the glibc version of unsetenv() does).
 
 