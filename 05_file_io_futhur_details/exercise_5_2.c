/*
    author: pthreadself (Wang Ke)
    email:  pthreadself at gmail dot com
    blog:   http://wangke.io    

    Write a program that opens an existing file for writing with the O_APPEND flag, and
    then seeks to the beginning of the file before writing some data. Where does the
    data appear in the file? Why?
*/

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    if (argc != 3) {
        printf("argc error.\nUsage: program <filename> <words_to_write>\n");
        return -1;
    }

    int fd = open(argv[1], O_RDWR|O_APPEND);
    if (fd < 0) {
        printf("open file %s error, %d=%s\n", argv[1], errno, strerror(errno));
        return -1;
    }

    off_t offset = lseek(fd, 0, SEEK_SET);
    if (offset == -1) {
        printf("lseek error, %d=%s\n", errno, strerror(errno));
        return -1;
    }

    ssize_t writenum = write(fd, argv[2], strlen(argv[2]));
    close(fd);
    return 0;
}