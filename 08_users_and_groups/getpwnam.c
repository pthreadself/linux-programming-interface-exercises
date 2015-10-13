#include <pwd.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>

struct passwd * the_getpwnam(const char * name) {
    if (NULL == name) return NULL; 

    struct passwd * pwdent;

    setpwent();
    while ((pwdent = getpwent()) != NULL) {
        if (strcmp(pwdent->pw_name, name) == 0) {
            break;
        }
    }
    endpwent();
    return pwdent;
}

void print_pwent(struct passwd * ent) {
    if (ent) {
        printf("\npasswd entry:\n");
        printf("login name:%s\n", ent->pw_name);
        printf("login shell:%s\n", ent->pw_shell);
        printf("UID:%d\n", ent->pw_uid);
        printf("GID:%d\n", ent->pw_gid);
    }
}


int main(int argc, char * argv[]) 
{
    if (argc < 2)
        printf("argc error. Usage: %s <entryname>\n", argv[0]);

    unsigned i = 1;
    const char * entry;
    struct passwd * pwdent;
    while((entry = argv[i++]) != NULL) {
        pwdent = the_getpwnam(entry);
        if (pwdent)
            print_pwent(pwdent);
        else
            printf("login name %s not found.\n", entry);
    }

    return 0;
}

