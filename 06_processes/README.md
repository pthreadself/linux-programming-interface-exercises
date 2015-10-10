#Linux Proramming Interface, Chapter 6 Exercises Answers

>**Q:** 6-1. Compile the program in Listing 6-1 (mem_segments.c), and list its size using ls �Cl.
Although the program contains an array (mbuf) that is around 10 MB in size, the
executable file is much smaller than this. Why is this?


 **A:**
 ��Ϊmbuf��uninitilized data������uninitialized data segment�����segment�е����ݽ�����program file�б�����λ����Ϣ�ʹ�С��Ϣ����û��ʵ�ʵ����ݣ�������program loader�ڽ�program file���ص��ڴ��г�Ϊprocess��ʱ��Ž�����Щ���ݵĿռ����ͳ�ʼ���ģ���ʼ����0�����ڼ���֮ǰ��û�н���ʵ�ʵĿռ���䡣

--- 

 >**Q:** 6-3. Implement setenv() and unsetenv() using getenv(), putenv(), and, where necessary, code that directly modifies environ. Your version of unsetenv() should check to see whether there are multiple definitions of an environment variable, and remove them all (which is what the glibc version of unsetenv() does).
 
 