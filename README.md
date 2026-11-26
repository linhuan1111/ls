# ls
|ls 命令行参数|意思|
|:------:|:------:|
|-a|显示所有文件，包括隐藏文件|
|-l|显示详细信息|
|-R|递归列出子数列|
|-t|按时间顺序排序|
|-r|逆序|
|-i|显示文件的inode编号(文件元信息)|
|-s|显示文件大小|

文件名存放在目录当中，但Linux系统内部不使用文件名，而是使用inode号码识别文件。对于系统来说文件名只是inode号码便于识别的别称。
#### ls -i:
直接查看文件inode号码，也可以通过stat查看文件inode信息(包括inode号、文件类型、硬连接数、权限、所有权、大小、时间戳...)查看inode号码。

#### 表面上，用户通过文件名打开文件，实际上，系统内部将这个过程分为三步：
1. 系统找到这个文件名对应的inode号码；
2. 通过inode号码，获取inode信息；
3. 根据inode信息，找到文件数据所在的block，并读出数据。
4. 其实系统还要根据inode信息，看用户是否具有访问的权限，有就指向对应的数据block，没有就返回权限拒绝。

## 基础知识：
1.命令行参数：
argc 和 argv 是 C 语言 main 函数用来接收命令行输入的参数
argc：参数的个数。
argv：参数内容组成的字符串数组。

例如：

./my_ls -a /tmp

argc=3

argv[0]=./my_ls
argv[1]=-a
argv[2]=/temp

2.读取目录：
opendir() 打开目录，readdir()每次读取一个目录项，closedir()关闭目录。
readdir() 每次返回一个目录项，但返回顺序不一定按文件名字母排列。
如果想让 ls 按名称排序，就要把读到的文件名先保存起来，读完后再排序，然后输出。
如果不要求排序，也可以读到一个名字就立刻打印；这样更简单，也少占内存。

C 语言标准库本身没有目录读取函数，跨平台或类 Unix（Linux / macOS）系统通常使用 POSIX 的 <dirent.h> 接口：opendir()、readdir() 和 closedir()。

### 核心函数

• opendir(const char *name)：打开目录，成功返回 DIR * 目录流指针，失败返回 NULL。
• readdir(DIR *dirp)：读取下一个目录项，返回 struct dirent *；到达末尾或出错返回 NULL。常见字段：d_name（文件名）、d_type（文件类型，如 DT_REG 普通文件、DT_DIR 目录，部分文件系统可能为 DT_UNKNOWN）。
• closedir(DIR *dirp)：关闭目录流，释放资源。
3.查询属性：
要显示文件类型、权限、大小、修改时间等信息，通常还要调用 stat() 或 lstat()。
stat()：跟随符号链接，查看链接指向的目标。
lstat()：查看符号链接本身。

4.筛选与排序：
程序可以跳过隐藏文件，也可以收集文件名后按字母顺序排序。目录读取接口本身不保证结果按字母排序。

5.输出格式：
最简单的是逐行打印名称。长格式则需要把权限、链接数、所有者、大小、时间和名称排成一行。格式化输出时还要考虑列宽和终端显示。

6.错误处理与资源管理:
目录打不开、属性查询失败时，程序应报告错误并继续或退出。成功打开的目录应关闭；动态分配的内存也应释放。

### 查找文件信息

头文件：<sys/types.h>，<sys/stat.h>，<unistd.h>

stat函数返回与此命名文件有关的信息结构
fstat函数获取已在描述符fields上打开文件的有关信息
lstat函数类似于stat但是当命名的文件是一个符号链接时，lstat返回该符号链接的有关信息，而不是由该符号链接引用文件的信息。

stat：获取链接文件的信息时，具有穿透能力，直接穿越链接文件，获取所被链接文件的信息。
lstat：获取链接文件的信息，无穿透能力

```
int stat(const char *path, struct stat *buf);//不是要const类型，是保证不改内容
int fstat(int filedes, struct stat *buf);
int lstat(const char *path, struct stat *buf);
```
参数buf是指针，它指向一个用于保存文件描述信息的结构，由函数填写结构内容。即下面这个结构体的指针，根据现实情况变化

```
struct stat { 
    dev_t         st_dev;       //文件的设备编号 
    ino_t         st_ino;       //节点 
    mode_t        st_mode;      //文件的类型和存取的权限 
    nlink_t       st_nlink;     //连到该文件的硬连接数目，刚建立的文件值为1 
    uid_t         st_uid;       //用户ID 
    gid_t         st_gid;       //组ID 
    dev_t         st_rdev;      //(设备类型)若此文件为设备文件，则为其设备编号 
    off_t         st_size;      //文件字节数(文件大小) 
    unsigned long st_blksize;   //块大小(文件系统的I/O 缓冲区大小) 
    unsigned long st_blocks;    //块数 
    time_t        st_atime;     //最后一次访问时间 
    time_t        st_mtime;     //最后一次修改时间 
    time_t        st_ctime;     //最后一次改变时间(指属性) 
}; 
```
#### 文件类型：
普通文件，目录文件，块特殊文件，字符特殊文件，套接字，FIFO，符号链接.
文件类型信息包含在stat结构的st_mode成员中,可以用如下的宏确定文件类型,这些宏是stat结构中st_mode的成员

S_ISREG();

S_ISDIR();

S_ISBLK();

S_ISCHR();

S_ISSOCK();

S_ISFIFO();

S_ISLNK();

#### 返回说明：
成功执行时，返回0。失败返回-1，errno被设为以下的某个值
EBADF： 文件描述词无效
EFAULT： 地址空间不可访问
ELOOP： 遍历路径时遇到太多的符号连接
ENAMETOOLONG：文件路径名太长
ENOENT：路径名的部分组件不存在，或路径名是空字串
ENOMEM：内存不足
ENOTDIR：路径名的部分组件不是目录

### 拼接路径
snprintf：格式化输出字符串，并将结果写入到指定的缓冲区
```
int snprintf ( char * str, size_t size, const char * format, ... );
```
char * str:目标地址；
size_t size：限定长度；
const char * format：格式化
...:来源，依旧指针
