#include<stdio.h>
#include<dirent.h>
//初始化什么呢？根据函数定义
    DIR *dir;
    struct wenjian
    {
        char name[256];
    };
int main(int argc,char *argv[])
{
    const char*lujing=argc>1?argv[1]:".";
    dir=opendir(lujing);
    if(dir==NULL)
    {
        return 1;
    }
    readdir(dir);
    closedir(DIR *dirp)
    //这三个函数怎么用呢
    //读取完了之后判断类型吗？
}