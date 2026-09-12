#include<iostream>
#include<sys/utsname.h>

/* 简化版 uname: 打印系统信息 */
int main(){
    struct utsname u;
    if(uname(&u) != 0)
        return EXIT_FAILURE;
    std::cout<<u.sysname<<" "<<u.nodename<<" "<<u.release<<" "<<u.version<<" "<<u.machine<<std::endl;
    return EXIT_SUCCESS;
}
