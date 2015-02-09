/*
    author: pthreadself (Wang Ke)
    email:  pthreadself at gmail dot com
    blog:   http://wangke.io    

    Write a program to verify that duplicated file descriptors share a file offset value
    and open file status flags.
*/

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        printf("argc error.\nUsage: %s <filename>\n", argv[0]);
        return -1;
    }

    int fd = open(argv[1], O_RDWR|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        printf("open/create file %s error, %d=%s\n", argv[1], errno, strerror(errno));
        return -1;
    }

    int fd2 = dup(fd);
    write(fd, "abc", 3);
    write(fd2, "def", 3);

    int flags = fcntl(fd, F_GETFL);
    int flags2 = fcntl(fd2, F_GETFL);
    if (flags == flags2 && flags != -1) {
        printf("file status flags verifed!\n");
    }

    char buf[6+1];
    lseek(fd, 0, SEEK_SET);
    read(fd, buf, 6);
    buf[6] = 0;
    if (strcmp(buf, "abcdef") == 0) {
        printf("file offset verifed!\n");
    }
    close(fd);
    close(fd2);
    return 0;
}