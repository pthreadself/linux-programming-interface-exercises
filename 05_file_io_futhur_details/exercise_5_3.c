/*
    author: pthreadself (Wang Ke)
    email:  pthreadself at gmail dot com
    blog:   http://wangke.io    

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
*/

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    printf("start!!\n");
    if (argc != 3 && argc != 4) {
        printf("argc error.\nUsage: %s filename num-bytes [x]\n", argv[0]);
        return -1;
    }

    int append = 0;
    int flags = O_RDWR|O_CREAT;
    if (argc == 4 && argv[3][0] == 'x') {
        append = 1;
        flags |= O_APPEND;
    }

    int fd = open(argv[1], flags, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        printf("open file %s error, %d=%s\n", argv[1], errno, strerror(errno));
        return -1;
    }

    int num_bytes = atoi(argv[2]);

    int i = 0;
    off_t offset;
    if (append == 0) {
        while (i++ < num_bytes) {
            offset = lseek(fd, 0, SEEK_END);
            if (-1 == offset) {
                printf("lseek failed.\n");
                break;
            } 

            if (-1 == write(fd, "a", 1)) {
                printf("write error, %d=%s\n", errno, strerror(errno));
                break;
            }
        }

    } else {
        while (i++ < num_bytes) {
            if (-1 == write(fd, "a", 1)) {
                printf("write error, %d=%s\n", errno, strerror(errno));
                break;
            }
        }
    }
    
    close(fd);
    return 0;
}