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

## Exercise 5-2

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

## Exercise 5-3

###Question:
    This exercise is designed to demonstrate why the atomicity guaranteed by opening
    a file with the O_APPEND flag is necessary. Write a program that takes up to three
    command-line arguments:
    $ atomic_append filename num-bytes [ x]
    This file should open the specified filename (creating it if necessary) and append
    num-bytes bytes to the file by using write() to write a byte at a time. By default, the
    program should open the file with the O_APPEND flag, but if a third command-line
    argument (x) is supplied, then the O_APPEND flag should be omitted, and instead the
    File I/O: Further Details 111
    program should perform an lseek(fd, 0, SEEK_END) call before each write(). Run
    two instances of this program at the same time without the x argument to write
    1 million bytes to the same file:
    $ atomic_append f1 1000000 & atomic_append f1 1000000
    Repeat the same steps, writing to a different file, but this time specifying the x
    argument:
    $ atomic_append f2 1000000 x & atomic_append f2 1000000 x
    List the sizes of the files f1 and f2 using ls –l and explain the difference.

###Answer: 
see exercise_5_3.c.

####Compile: 
gcc exercise_5_3.c -D_NDEBUG -g -Wno-format -o program


####Usage: 
program filename num-bytes [x]

For example: 

    program a.txt 1000000 & program a.txt 1000000 
    -rw------- 1 pthreadself pthreadself 1999738 Feb  6 15:52 a.txt

    program a.txt 1000000 x & program a.txt 1000000 x
    -rw------- 1 pthreadself pthreadself 2000000 Feb  6 15:53 a.txt




