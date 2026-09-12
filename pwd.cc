#include<errno.h>
#include<iostream>
#include<unistd.h>
#include<cstring>

/* 简化版 pwd: 打印当前工作目录 */
int main(){
    char buf[4096];
    if(getcwd(buf, sizeof(buf)) == NULL){
        std::cerr<<"pwd: cannot get current directory: "<<strerror(errno)<<std::endl;
        return EXIT_FAILURE;
    }
    std::cout<<buf<<std::endl;
    return EXIT_SUCCESS;
}
