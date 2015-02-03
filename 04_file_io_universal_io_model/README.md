# ReadMe
===================

Worked out solutions to exercises in chapter4: FILE I/O: The Universal I/O Model

## Exercise 4-1

###Question:
The tee command reads its standard input until end-of-file, writing a copy of the input to standard output and to the file named in its command-line argument. (We show an example of the use of this command hwne we discuss FIFOs in Section 44.7.) Implement tee using I/O system calls. By default, tee overwites any existing file with the given name. Impelment the -a command-line option (tee -a file), which casuses te to append text to the end of a file if it already exists. (Refer to Appendix B for desription ofthe getopt() function, which can be used to parse command-line options.)

###Answer: 
see exercise_4_1.c.

####Compile: 
gcc exercise_4_1.c -D_NDEBUG -g -Wno-format -o mytee


####Usage: 
mytee [-a] [-b buffersize] -f <filename>
`-a` : append output to file
`-b` : specify buffer size used to read from standard input
`-f` : full file/path/name used to save the output