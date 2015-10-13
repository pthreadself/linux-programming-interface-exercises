#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>



int verify_pid_dir(const char * pid, long uid) {
    char base[] = "/proc/";
    char full[256];
    strcpy(full, base);
    strcat(full, pid);
    strcat(full, "/status");
    FILE * fp = fopen(full, "r");
    if (fp == NULL) {
        fprintf(stderr, "open %s error, errno=%d, errmsg=%s\n",
                full, errno, strerror(errno));
        return -1;
    }

    char line[256];
    char process_name[256];
    process_name[0] = 0;
    long parsed_uid = -1;

    while(fgets(line, sizeof(line), fp)) {

        const char * key = strtok(line, ":");

        if (strcmp(key, "Uid") == 0) {
            const char * value = strtok(NULL, ":");
            parsed_uid = strtol(value, NULL, 10);

        } else if (strcmp(key, "Name") == 0) {
            const char * pname = strtok(NULL, ":");
            if (pname)
                strcpy(process_name, pname);
        }

        if (parsed_uid != -1 && process_name[0] != '\0')
            break;
    }

    if (parsed_uid == uid) {
        printf("%s %s", pid, process_name); 
    }
    return 0;
}



void walk_through_processes(uid_t _uid) {
    DIR * procdir = opendir("/proc");
    if (NULL == procdir) {
        fprintf(stderr, "open /proc error, errno=%d, errmsg=%s\n", 
                errno, strerror(errno));
        return;
    }

    struct dirent * ent;
    while ((ent = readdir(procdir)) != NULL) {
        char * endptr = NULL;
        strtol(ent->d_name, &endptr, 10);
        if (*endptr != '\0') //entry name not numeric
            continue;
        verify_pid_dir(ent->d_name, _uid);
    }
}


int main(int argc, char * argv[])
{
    if (argc != 2) {
        printf("argc error. Usage: %s <login_name>\n", argv[0]);
        return -1;
    }

    struct passwd * pwd;
    pwd = getpwnam(argv[1]);
    if (pwd) {
        walk_through_processes(pwd->pw_uid);

    } else {
        printf("login name %s not found.\n", argv[1]);
        return -1;
    }


    return 0;
}
