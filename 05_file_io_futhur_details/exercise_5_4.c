/*
    author: pthreadself (Wang Ke)
    email:  pthreadself at gmail dot com
    blog:   http://wangke.io    

    Implement dup() and dup2() using fcntl() and, where necessary, close(). (You may
    ignore the fact that dup2() and fcntl() return different errno values for some error
    cases.) For dup2(), remember to handle the special case where oldfd equals newfd. In
    this case, you should check whether oldfd is valid, which can be done by, for example,
    checking if fcntl(oldfd, F_GETFL) succeeds. If oldfd is not valid, then the function
    should return –1 with errno set to EBADF.
*/

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

int _dup(int oldfd) {
    return fcntl(oldfd, F_DUPFD, 0);
}

/*
       dup2() makes newfd be the copy of oldfd, closing newfd first if necessary, but note the following:

       *  If oldfd is not a valid file descriptor, then the call fails, and newfd is not closed.

       *  If oldfd is a valid file descriptor, and newfd has the same value as oldfd, then dup2() does nothing, 
          and returns newfd.

       After  a  successful  return from one of these system calls, the old and new file descriptors may be 
       used interchangeably.  They refer to the same open file description (see open(2)) and thus
       share file offset and file status flags; 

       for example, if the file offset is modified by using lseek(2) on one of the descriptors, 
       the offset is also changed for the other.
 */
int _dup2(int oldfd, int newfd) {

    int fd;
    if (oldfd == newfd) {
        if (-1 == fcntl(oldfd, F_GETFL)) {
            fd = -1;
            errno = EBADF;
        } else {
            fd = oldfd;
        }
    } else {
        close(newfd);
        fd = fcntl(oldfd, F_DUPFD, newfd);
    }

    return fd;
}

int main(int argc, char const *argv[])
{
    return 0;
}