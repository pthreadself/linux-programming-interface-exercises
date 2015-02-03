/* 
	author: pthreadself
	email:  pthreadself at gmail dot com
    blog:   http://wangke.io	

	Write a program like cp that, when used to copy a regular file that contains holes
	(sequences of null bytes), also creates corresponding holes in the target file.
*/

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    if (argc != 3) {
        printf("argc error.\nUsage: program src_file dest_file\n");
        return -1;
    }

    int srcfd = open(argv[1], O_RDONLY);
    if (srcfd == -1) {
        printf("open src file %s failed, %d=%s.\n", argv[1], errno, strerror(errno));
        return -1;
    }


    int dstfd = open(argv[2], O_RDWR|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);
    if (dstfd == -1) {
        printf("open dst file %s failed, %d=%s.\n", argv[2], errno, strerror(errno));
        return -1;
    }

    ssize_t numread;
    char buff[1024];
    while ((numread = read(srcfd, buff, sizeof(buff))) > 0) {
        int i = 0;
        size_t holesize = 0;
        /* using variable start and end to record the starting and ending position of 
           non-null byte, instead of write each non-null byte one at a time, 
           to reduce calls to write() */
        const char * start = NULL;
        const char * end = NULL;
        while (i < numread) {
            /* use lseek to go across the hold instead of writing null bytes into file */
            if (buff[i] == 0) {
                holesize++;
                if (NULL != start) {
                    int num_to_write = end - start + 1;
                    if (num_to_write != write(dstfd, start, num_to_write)) {
                        printf("write failed\n");
                        return -1;
                    }

                    start = NULL;
                    end = NULL;
                }
            } else if (holesize > 0) {
                lseek(dstfd, holesize, SEEK_CUR);
                write(dstfd, &buff[i], 1);
                holesize = 0;
                start = &buff[i];
                end = start;
            } else {
                if (NULL == start) {
                    start = &buff[i];
                }
                end = &buff[i];
                if (i == numread -1) {
                    write(dstfd, start, end - start + 1);
                }
            }
            i++;
        }
    }
    return 0;
}