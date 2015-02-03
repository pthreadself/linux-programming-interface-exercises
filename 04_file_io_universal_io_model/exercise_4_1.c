/* 
    author: pthreadself
    blog:   http://wangke.io

    description:

    read from standard input until end-of-file and write a copy of the input
    to standard outpout and to the file named in its command-line argument.
    By default, this program overwrites any existing file with the given name.
    -a command line option append text to the end of a file if it already exists.

    program [-a] [-b <buffer size>] -f <filename> 

*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define DEF_BUF_SIZE 128


int main(int argc, char * const argv[])
{
    int is_append = 0;
    const char * filename = NULL;
    int opt;
    int buffsize = DEF_BUF_SIZE;

    while ((opt = getopt(argc, argv, ":af:b:")) != -1) {
    
        switch (opt) {
        
        case 'a':
            is_append = 1;
            break;
            
        case 'f':
            filename = optarg;
            break;
            
        case 'b':
	        buffsize = atoi(optarg);
	        printf("buff size is %d\n", buffsize);
	        break;
            
        case ':':
            printf("missing argument on option %c\n", optopt);
            break;
            
        case '?':
            printf("unknown argument %c\n", optopt);
            break;
            
        default:
	        printf("unknown argument %c\n", optopt);
            break;
        }
    }

    if (!filename) {
        printf("missing filename\n");
        return -1;
    }

    int flags = is_append ? (O_CREAT|O_RDWR|O_APPEND) : (O_CREAT|O_RDWR|O_TRUNC);
    int fd = open(filename, flags, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        printf("open %s failed %d, due to %s\n", filename, errno, strerror(errno));
        return -1;
    }


    ssize_t num_read;
    ssize_t num_write;
    int left;
    
    void *buff = malloc(buffsize);
    if (!buff) {
		printf("malloc failed, due to %d,%s\n", errno, strerror(errno));
		return -1;
    }
    
    while ((num_read = read(STDIN_FILENO, buff, buffsize)) > 0) {
    
#ifndef _NDEBUG
        printf("[DEBUG]read %d bytes.\n", num_read);
#endif

        /* write to output file */
        left = num_read;
        num_write = 0;
        
        while (left > 0) {
            num_write = write(fd, buff + num_write, left);
            
            if (num_write < 0) {
                printf("write to file error, errno=%d, errmsg=%s\n", errno, strerror(errno));
                return -1;
            }
            left -= num_write;
            
#ifndef _NDEBUG
            printf("[DEBUG]write %d bytes, left %d bytes\n", num_write, left);
#endif
        }

        /* write to stdout */
        left = num_read;
        num_write = 0;
        
        while (left > 0) {
            num_write = write(STDOUT_FILENO, buff + num_write, left);
            
            if (num_write < 0) {
                printf("write to stdout error, errno=%d, errmsg=%s\n", errno, strerror(errno));
                return -1;
            }
            left -= num_write;
            
#ifndef _NDEBUG
            printf("[DEBUG]write %d bytes, left %d bytes\n", num_write, left);
#endif
        }
        
    }

    if (num_read == -1) {
        printf("read error, errno=%d, errmsg=%s\n", errno, strerror(errno));
        return -1;
    }


    return 0;
}