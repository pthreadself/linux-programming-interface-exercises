/*
    author: pthreadself
    email:  pthreadself at gmail dot com
    blog:   http://wangke.io    


    Linux Programming Interface, Chapter 13 File I/O Buffering
    Exercise 13-5

    The command 
                 tail [ -n num ] filename 
                 
    prints the last num lines (ten by default) of the
    named file. Implement this command using I/O system calls (lseek(), read(), write(),
    and so on). Keep in mind the buffering issues described in this chapter, in order to
    make the implementation efficient.

 */
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#ifndef BUF_SIZE
#define BUF_SIZE (1024 * 4)
#endif


char buf[BUF_SIZE + 1];
int last_n_lines = 10;
int fd = -1;
int block_num = 0;
int fragment = 0;
off_t filesize = 0;


ssize_t find_nth_lines_in_one_block(int blockid, int n)
{
    int read_num;
    int to_read = move_file_cursor(blockid);
    if (to_read < 0) {
        printf("move_file_cursor error\n");
        return -1;
    }

    int count = 1;
    int offset = 0;
    char * p = buf;
    
    while ((read_num = read(fd, buf, to_read)) > 0) {

        p = buf;
	    while ((p = strchr(p, '\n')) && count < n) {
	        count++;
	        p++;
	    }
	    if (count < n)
		    offset += read_num;
    }

    if (read_num < 0) {
        printf("read error, %d=%s\n", errno, strerror(errno));
        return -1;
    }

    return ((p - buf) + offset);

}


int main(int argc, char const *argv[])
{
    fd = parse_opts(argc, argv);
    if (fd < 0) {
        printf("fd=%d error\n", fd);
        return -1;
    }
    block_num = file_block_num(fd);
    if (block_num <= 0) {
        printf("block_num=%d error\n", block_num);
        return -1;
    }

    int lines_fetched = 0;
    int lines_to_fetch = last_n_lines - lines_fetched;
    int i = 0;
    size_t pos = 0;

    int count;
    
    do {
        count = count_lines_in_one_block(i);
        if (count < 0) {
			printf("process_one_block error\n");
			return -1;
        }
        
        lines_fetched += count;
        
        if (lines_fetched < last_n_lines) {
            lines_to_fetch -= count;
        }
        
    } while(lines_fetched < last_n_lines && i++ < block_num);

    ssize_t pos_to_file_end = 0;

    if (lines_fetched >= last_n_lines) {
       /* use i and pos */
       int to_read = move_file_cursor(i);
       ssize_t offset = find_nth_lines_in_one_block(i, count - lines_to_fetch);
       if (offset == (ssize_t)-1) {
           printf("find_nth_lines_in_one_block error\n");
           return -1;
       }

       pos_to_file_end = i * BUF_SIZE + (to_read - (offset + 1));
       
    } else {
    
        /* 
          reach end of file but still no enough lines as requested,
          output all the content 
        */
        pos_to_file_end = filesize;
    }

    lseek(fd, -pos_to_file_end, SEEK_END);
    ssize_t readnum;
    char tempbuf[1024];
    while ((readnum = read(fd, tempbuf, sizeof(tempbuf))) > 0 ) {
        write(STDOUT_FILENO, tempbuf, readnum);
    }


    
    return 0;
}

int parse_opts(int argc, char *argv[])
{
    int c;
    const char * filename = NULL;
    while ((c = getopt(argc, argv, ":n:")) != -1) {
        switch (c) {

        case 'n':
            last_n_lines = atoi(optarg);
            break;

        case ':':
            printf("Option -%c requires an argument.\n", optopt);
            break;

        case '?':
            if (isprint(optopt)) 
                printf("Unknown option -%c\n", optopt);
            else
                printf("Unknown option character %x\n", optopt);
            break;

        default:
            printf("getopt error.\n");
            break;
        }
    }

    if (optind + 1 != argc) {
        printf("missing filename\n");
        return -1;
    } else {
        filename = argv[optind];
    }

    int tmpfd = open(filename, O_RDONLY);
    if (tmpfd < 0) {
        printf("open %s error,%d=%s\n", filename, errno, strerror(errno));
        return -1;
    } else {
        return tmpfd;
    }
}


int file_block_num(int fd)
{
    filesize = lseek(fd, 0, SEEK_END);
    if (filesize == (off_t)-1) {
        printf("lseek error,%d=%s\n", errno, strerror(errno));
        return -1;
    }
    fragment = filesize % BUF_SIZE;
    return (filesize / BUF_SIZE + 1);
}

int count_lines_in_one_block(int blockid)
{
    int read_num;
    
    int already_read_total;
    
    int left_to_read;
    
    int to_read_in_this_block = move_file_cursor(blockid);
    if (to_read_in_this_block < 0) {
        printf("move_file_cursor error\n");
        return -1;
    }

    int count = 0;
    char * p = buf;

    already_read_total = 0;
    left_to_read = to_read_in_this_block;
    
    
    while (left_to_read > 0 && (read_num = read(fd, buf + already_read_total, left_to_read)) > 0) {
        left_to_read -= read_num;
        already_read_total += read_num;
    }
    
    if (read_num < 0) {
        printf("read error, %d=%s\n", errno, strerror(errno));
        return -1;
    }

    p = buf;
    buf[BUF_SIZE] = '\0';
    while (p = strchr(p, '\n')) {
        count++;
        p++;
    }
    
    return count;
}

/* output content starting from offset `pos` to standard output */
int output(off_t pos)
{
}

int move_file_cursor(int block)
{
    int to_read = 0;
    /* last block in reverse order */
    if (block == block_num - 1) {
        to_read = fragment;
        lseek(fd, 0, SEEK_SET);
    } else {
        to_read = BUF_SIZE;
        lseek(fd, -((block + 1) * BUF_SIZE), SEEK_END);
    }
    return to_read;
}































