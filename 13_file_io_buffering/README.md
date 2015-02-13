# ReadMe

Worked out solutions to exercises in chapter13: FILE I/O Buffering

## Exercise 13-5

###Question:
    The command tail [ –n num ] file prints the last num lines (ten by default) of the
    named file. Implement this command using I/O system calls (lseek(), read(), write(),
    and so on). Keep in mind the buffering issues described in this chapter, in order to
    make the implementation efficient.

###Answer: 
see exercise_13_5.c.

####Compile: 
gcc exercise_13_5.c -D_NDEBUG -g -Wno-format -o program 


####Usage: 
program [-n num] filename


