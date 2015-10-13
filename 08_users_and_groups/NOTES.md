# The Linux Programming Interface Chapter 8  Users and Groups

##8.1 The Password File: /etc/passwd

>**mtk:x:1000:100:Michael Kerrisk:/home/mtk:/bin/bash**
`login name : password : UID : GID : Comment : Home Dir : Login Shell`

In order, these fields are as follows:
* **login name**
* **encrypted password**
如果shadow password功能开启则这里一般显示x; 如果为空则表示没有密码
如果shadow password功能关闭则这里显示的是加密后的密码，加密方法可能是DES,MD5等
* **User ID(UID)**
如果值为0表示**superuser privilege**，一般只有一个账户该值为0，即root
* **Group ID(GID)**
用户所属的**第一个**Group的ID（用户可以属于多个Group）
* Comment
* **Home Directory**，这里配置的值之后就成为了`$HOME`环境变量
* **Login Shell**，这里配置的值之后就称为了`$SHELL`环境变量

##8.2 The Shadow Password File: /etc/shadow
`
ls -lrth /etc/passwd /etc/shadow
-rw------- 1 root root  728 Sep  9 15:18 /etc/shadow
-rw-r--r-- 1 root root 1.1K Sep  9 15:38 /etc/passwd
`
/etc/passwd文件所有用户可读，/etc/shadow只有privilege权限可读

##8.3 The Group File: /etc/group
一个用户属于那些组呢，一个是在/etc/passwd中列出的GID对应的Group，还属于/etc/group中列出的组

` cat /etc/group `
`
root:x:0:root
bin:x:1:root,bin,daemon
daemon:x:2:root,bin,daemon
sys:x:3:root,bin
adm:x:4:root,daemon
`
每行含有：
* Group name
* Encrypted password
即可以为每个group设置一个group password
> `newgrp [group]`命令将当前用户的权限切换成组*group*的权限。如果当前用户并不属于组*group*，则需要这里的**组密码(group password)**。
> 组密码也有shadow功能，shadow文件为**/etc/gshadow**

* Group ID
* User list
列出了这个组下有哪些用户

##8.4 Retrieving User and Group Information
### Retriving records from the apssword file

```c
#include <pwd.h>
struct passwd *getpwnam(const char *name);
struct passwd *getpwuid(uid_t uid);

struct passwd {
char *pw_name; /* Login name (username) */
char *pw_passwd; /* Encrypted password */
uid_t pw_uid; /* User ID */
gid_t pw_gid; /* Group ID */
char *pw_gecos; /* Comment (user information) */
char *pw_dir; /* Initial working (home) directory */
char *pw_shell; /* Login shell */
};
```

返回的struct passwd是statically allocated memory，所以这两个函数不是reentrant的。
对应的MT-Safe版本的函数为**getpwnam_r(), getpwuid_r(), getgrnam_r(), and getgrgid_r()**

**关于如何区分/etc/passwd中没找到某条记录还是找的过程中出错了:**
> According to SUSv3, if a matching passwd record can’t be found, then getpwnam() and getpwuid() should return NULL and leave errno unchanged. This means that we should be able to distinguish the error and the “not found” cases using code such as the following:

```c
struct passwd *pwd;
errno = 0;
pwd = getpwnam(name);
if (pwd == NULL) {
if (errno == 0)
/* Not found */;
else
/* Error */;
}
```

然而这并不是可移植的，因为很多UNIX implementations没有完全遵循SUSv3。
> However, a number of UNIX implementations don’t conform to SUSv3 on this
point. If a matching passwd record is not found, then these functions return NULL and set errno to a nonzero value, such as ENOENT or ESRCH. Before version 2.7, glibc produced the error ENOENT for this case, but since version 2.7, glibc conforms to the SUSv3 requirements. 

### Retrieving records from the group file
```c
#include <grp.h>
struct group *getgrnam(const char *name);
struct group *getgrgid(gid_t gid);

struct group {
char *gr_name; /* Group name */
char *gr_passwd; /* Encrypted password (if not password shadowing) */
gid_t gr_gid; /* Group ID */
char **gr_mem; /* NULL-terminated array of pointers to names
of members listed in /etc/group */
};

```

###Scanning all records in the password and group files

```c
#include <pwd.h>
struct passwd *getpwent(void);
//Returns pointer on success, or NULL on end of stream or error
void setpwent(void);
void endpwent(void);
```

endpwent用来关闭文件，这样下一次的getpwent会重新打开文件并从头开始扫描；
直接调用setpwent也可以从头开始扫描。

###Retrieving records from the shadow password file
```c
#include <shadow.h>
struct spwd *getspnam(const char *name);
//Returns pointer on success, or NULL on not found or error
struct spwd *getspent(void);
//Returns pointer on success, or NULL on end of stream or error
void setspent(void);
void endspent(void);
```

```c
struct spwd {
char *sp_namp; /* Login name (username) */
char *sp_pwdp; /* Encrypted password */
/* Remaining fields support "password aging", an optional
feature that forces users to regularly change their
passwords, so that even if an attacker manages to obtain
a password, it will eventually cease to be usable. */
long sp_lstchg; /* Time of last password change
(days since 1 Jan 1970) */
long sp_min; /* Min. number of days between password changes */
long sp_max; /* Max. number of days before change required */
long sp_warn; /* Number of days beforehand that user is
warned of upcoming password expiration */
long sp_inact; /* Number of days after expiration that account
is considered inactive and locked */
long sp_expire; /* Date when account expires
(days since 1 Jan 1970) */
unsigned long sp_flag; /* Reserved for future use */
};
```

##8.5 Password Encryption and User Authentication
* 加密方式是one-way encryption的算法，如DES,MD5，因此authentication是通过调用库函数根据明文参数计算出密文后，比较/etc/shadow中的密码（即struct spwd中的sp_pwdp成员），相同则认证通过。
```c
#define _XOPEN_SOURCE
#include <unistd.h>
char *crypt(const char *key, const char *salt);
```
>Both the salt argument and the encrypted password are composed of characters selected from the 64-character set [a-zA-Z0-9/.]. Thus, the 2-character salt argument can cause the encryption algorithm to vary in any of 64 * 64 = 4096 different ways. 

* salt也保存在crypt返回的statically allocated string中的前两个字符。实际上，crypt也直接忽略掉salt参数中2个字符后面的字符，只关心前两个字符，所以调用crypt的时候，第二个参数可以直接指定为从/etc/shadow中获取的密码密文
































