#include<stdio.h>//printf
#include<dirent.h>//打开、读取、关闭文件
#include<string.h>//strcpy（目标，来源），搬运字符串，带'\0'；对比memcpy（目标，来源，字节数）,不带'\0'
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

int main(int argc,char *argv[])
{
    struct wenjian wj;
    const char*lujing=argc>1?argv[1]:".";//"."表示当前路径
    dir=opendir(lujing);
    if(dir==NULL)
    {
        return 1;
    }
    while((du=readdir(dir))!=NULL)//加判断条件
    {
        strcpy(wj.name,du->d_name);
        printf("%s\n",wj.name);
    }
    closedir(dir);
    return 0;
}