#The Linux Programming Interface Chapter12: SYSTEM AND PROCESS INFORMATION

##12.1 The `/proc` File System

###12.1.1 Obtaining Information About a Process: /proc/`PID`

* status信息
`cat /proc/1/status`
```
Name:	systemd
State:	S (sleeping)
Tgid:	1
Ngid:	0
Pid:	1
PPid:	0
TracerPid:	0
Uid:	0	0	0	0
Gid:	0	0	0	0
FDSize:	64
Groups:	
NStgid:	1
NSpid:	1
NSpgid:	1
NSsid:	1
VmPeak:	   43948 kB
VmSize:	   34460 kB
VmLck:	       0 kB
VmPin:	       0 kB
VmHWM:	    4820 kB
VmRSS:	    4820 kB
VmData:	    1064 kB
VmStk:	     136 kB
VmExe:	    1336 kB
VmLib:	    2976 kB
VmPTE:	      80 kB
VmPMD:	      12 kB
VmSwap:	       0 kB
Threads:	1
SigQ:	0/3969
SigPnd:	0000000000000000
ShdPnd:	0000000000000000
SigBlk:	7be3c0fe28014a03
SigIgn:	0000000000001000
SigCgt:	00000001800004ec
CapInh:	0000000000000000
CapPrm:	0000003fffffffff
CapEff:	0000003fffffffff
CapBnd:	0000003fffffffff
Seccomp:	0
Cpus_allowed:	3
Cpus_allowed_list:	0-1
Mems_allowed:	00000000,00000001
Mems_allowed_list:	0
voluntary_ctxt_switches:	1855
nonvoluntary_ctxt_switches:	400
```
随着kernel版本的变更，status里字段数量可能有增减，因此不要以行号来获取参数，而应该以每行行首的关键词来获取信息。

* /porc/`PID`/fd 目录
包含了改进程打开的所有fd的symbolic link

* Threads: the /proc/`PID`/task 目录
包含进程下各个线程的信息，子目录的名字就是线程号，即gettid的返回值

###12.1.2 System information under `/proc`
`/proc` Various system information
`/proc/net` Status information about networking and sockets
`/proc/sys/fs` Settings related to file systems
`/proc/sys/kernel` Various general kernel settings
`/proc/sys/net` Networking and sockets settings
`/proc/sys/vm` Memory-management settings
`/proc/sysvipc` Information about System V IPC objects

###12.1.3 Accessing `/proc` Files
accessed by shell (cat /proc/sys/kernel/pid_max) or by normal file I/O

##12.2 System Identification: uname()

```c
#include <sys/utsname.h>
int uname(struct utsname *utsbuf);

#define _UTSNAME_LENGTH 65
struct utsname {
char sysname[_UTSNAME_LENGTH]; /* Implementation name */
char nodename[_UTSNAME_LENGTH]; /* Node name on network */
char release[_UTSNAME_LENGTH]; /* Implementation release level */
char version[_UTSNAME_LENGTH]; /* Release version level */
char machine[_UTSNAME_LENGTH]; /* Hardware on which system
is running */
#ifdef _GNU_SOURCE /* Following is Linux-specific */
char domainname[_UTSNAME_LENGTH]; /* NIS domain name of host */
#endif
};

```

Linux上，还有以下两种方法获取系统版本的有关信息：
* /proc/sys/kernel目录下的**ostype**, **osrelease**，**version**文件
* /proc/version文件

###nodename
`struct utsname`里的nodename值就是hostname，即可以由以下system call获取和修改：`gethostname()`，`sethostname()`，以及由command `hostname(1)`和`/proc/hostname`修改

###domainname
`struct utsname`里的domainname就是 Information Services(NIS) domain name，由以下system call获取和修改： `getdomainname()`，`setdomainname()`，以及由command `domainname(1)`和`/proc/domainname`修改



