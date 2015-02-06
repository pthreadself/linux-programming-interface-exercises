# ReadMe

Worked out solutions to exercises in chapter5: FILE I/O: Futhur Details 

## Exercise 5-1

###Question:
    Modify the program in Listing 5-3 to use standard file I/O system calls (open() and
    lseek()) and the off_t data type. Compile the program with the _FILE_OFFSET_BITS
    macro set to 64, and test it to show that a large file can be successfully created.

###Answer: 
see exercise_5_1.c.

####Compile: 
gcc exercise_5_1.c -D_NDEBUG -g -Wno-format `-m32` -o program 


####Usage: 
program filename offset 

For example: program file.txt 10111222333

## Exercise 4-2

###Question:
    Write a program that opens an existing file for writing with the O_APPEND flag, and
    then seeks to the beginning of the file before writing some data. Where does the
    data appear in the file? Why?

###Answer: 
see exercise_5_2.c.

####Compile: 
gcc exercise_5_2.c -D_NDEBUG -g -Wno-format -o program


####Usage: 
program filename words_to_append

For example: program a.txt hello