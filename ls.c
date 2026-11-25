#include<stdio.h>//printf
#include<dirent.h>//打开、读取、关闭文件
#include<string.h>//strcpy（目标，来源），搬运字符串，带'\0'；对比memcpy（目标，来源，字节数）,不带'\0'
#include<stdlib.h>//用strbuf了，realloc之类的
//初始化什么呢？根据函数定义
    DIR *dir;//系统提前定义好的一个类型，代表"一个被打开的目录"的地址
    struct wenjian
    {
        char name[256];
    };
    struct dirent *du;//#include<dirent.h>里面定义的，直接用,不能加新的类
    
    /*
    struct dirent
    {
    __ino_t d_ino; (inode 编号)             
    __off_t d_off; (目录内部的偏移量)             
    unsigned short d_reclen; (这一条记录占多少字节)  
    unsigned char d_type; (类型：普通文件/目录/链接…)     
    char d_name[256]; (文件名字符串)         
    };
    */
   int a=0;
   int l=0;
   int R=0;
   int t=0;
   int r=0;
   int i=0;
   int s=0;

   static size_t strbuf_next_alloc(size_t alloc, size_t need)
{
    if (alloc == 0)
        alloc = 1;
    while (alloc < need) 
    {
        if (alloc > ((size_t)-1) / 2) //防溢出
        {
            alloc = need;
            break;
        }
        alloc *= 2;
    }
    return alloc;
}
int main(int argc,char *argv[])
{
    struct wenjian *wj=NULL;
    size_t alloc=0;
    int shu=0;//记录在wj存了多少个文件名
    for(int z=0;z<argc;z++)
    {
        if(argv[z][0]=='-')//argv本身就是两层，外层会用‘ ’隔开，内层可以再一个个看
        {
            int f=0;
            while(argv[z][f]!='\0')//例：-art 是一整块，整个词是外层的一块，以'\0'结尾(感觉可以看作字符串)
            {                      //每个块之间的‘ ’被shell吞掉(so 其实还不明白shell是什么)
                f++;
                if(argv[z][f]=='a') a=1;
                if(argv[z][f]=='l') l=1;
                if(argv[z][f]=='R') R=1;
                if(argv[z][f]=='t') t=1;
                if(argv[z][f]=='r') r=1;
                if(argv[z][f]=='i') i=1;
                if(argv[z][f]=='s') s=1;
            }
        }
    }
    const char*lujing=argc>1?argv[1]:".";//"."表示当前路径
    dir=opendir(lujing);
    if(dir==NULL)
    {
        return 1;
    }
    while((du=readdir(dir))!=NULL)
    {
        alloc=strbuf_next_alloc(alloc,shu+1);
        struct wenjian *newbuf = (struct wenjian *)realloc(wj,alloc*sizeof(*wj));//realloc(要被重新分配的内存块，分配大小字节数)
                                                                                //分配大小字节数：个数（size_t)*sizeof(类型)
        if (newbuf == NULL)
        {
            return 1;
        }
        wj=newbuf;
        strcpy(wj[shu].name,du->d_name);//可以把strbuf的动态扩容拿过来
        
        shu++;
    }
    closedir(dir);
    for(int j=0;j<shu;j++)
    {
        printf("%s\n",wj[j].name);
    }
    free(wj);
    return 0;
}