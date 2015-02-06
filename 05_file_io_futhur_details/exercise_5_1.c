/*
    author: pthreadself (Wang Ke)
    email:  pthreadself at gmail dot com
    blog:   http://wangke.io    

    Modify the program in Listing 5-3 to use standard file I/O system calls (open() and
    lseek()) and the off_t data type. Compile the program with the _FILE_OFFSET_BITS
    macro set to 64, and test it to show that a large file can be successfully created.
*/

/* define this before include fcntl.h */ 
#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char const *argv[])
{
    if (argc != 3 || 0 == strcmp(argv[1], "--help")) { 
        printf("Usage: %s <pathname> <offset>\n", argv[0]);
        return -1;
    }

    int fd = open(argv[1], O_CREAT|O_RDWR, S_IWUSR|S_IRUSR);   

    if (fd == -1) {
        printf("open error,errno=%d,errmsg=%s\n", errno, strerror(errno));
        return -1;
    }
    long long size = atoll(argv[2]);
    off_t offset = size;
    if (-1 == lseek(fd, offset, SEEK_SET)) {
        printf("lseek error,errno=%d,errmsg=%s\n", errno, strerror(errno));
        close(fd);
        return -1;
    }

    if (-1 == write(fd, "test", 4)) {
        printf("write error,errno=%d,errmsg=%s\n", errno, strerror(errno));
        close(fd);
        return -1;
    }

    close(fd);
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        printf("open error,errno=%d,errmsg=%s\n", errno, strerror(errno));
        return -1;
    } else {
        printf("file opened successfully.\n");
        offset = lseek(fd, 0, SEEK_END);
        if (-1 == offset) {
            printf("lseek error,errno=%d,errmsg=%s\n", errno, strerror(errno));
        } else {
            printf("%s's file size is %lld bytes\n", argv[1], offset);
        }
        close(fd);
    }

    return 0;
}