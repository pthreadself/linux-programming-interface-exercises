#The Linux Programming Interface Chapter 8  Users and Groups, Exercises Answers

##8-1. Question
>When we execute the following code, we find that it displays the same number
twice, even though the two users have different IDs in the password file. Why is this?
```c
printf("%ld %ld\n", (long) (getpwnam("avr")->pw_uid),
(long) (getpwnam("tsr")->pw_uid));
```

##8-1. Answer
作者原先可能是想提醒读者getpwnam函数不是reentrant的。
但是就本题这段代码而言，在gcc 5.2.0编译后，不会出现题中所说的情况，编译后的代码如下：
```asm
0000000000400460 <main>:
  400460:	53                   	push   %rbx
  400461:	bf 14 06 40 00       	mov    $0x400614,%edi
  400466:	e8 d5 ff ff ff       	callq  400440 <getpwnam@plt>
  40046b:	bf 19 06 40 00       	mov    $0x400619,%edi
  400470:	8b 58 10             	mov    0x10(%rax),%ebx
  400473:	e8 c8 ff ff ff       	callq  400440 <getpwnam@plt>
  400478:	8b 70 10             	mov    0x10(%rax),%esi
  40047b:	bf 1d 06 40 00       	mov    $0x40061d,%edi
  400480:	31 c0                	xor    %eax,%eax
  400482:	48 89 da             	mov    %rbx,%rdx
  400485:	e8 96 ff ff ff       	callq  400420 <printf@plt>
  40048a:	31 c0                	xor    %eax,%eax
  40048c:	5b                   	pop    %rbx
  40048d:	c3                   	retq   
  40048e:	66 90                	xchg   %ax,%ax
```

可以看到，两次调用getpwnam后，pw_uid的值分别存到了寄存器%ebx和%esi里，然后调用printf的时候，%ebx和%esi作为函数调用的第二个参数和第三个参数(详见[System V Application Binary Interface AMD64 Architecture Processor Supplement](http://x86-64.org/documentation/abi.pdf))。

##8-2. Question:
>Implement getpwnam() using setpwent(), getpwent(), and endpwent().

##8-2. Answer
See **getpwnam.c**

**Usage**
./program <login_names...>

**Example**
./program root bin 

>**Output**
passwd entry:
login name:bin
login shell:/usr/bin/nologin
UID:1
GID:1

>passwd entry:
login name:root
login shell:/bin/bash
UID:0
GID:0





