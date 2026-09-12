#include<errno.h>
#include<iostream>
#include<unistd.h>
#include<cstring>

/* 简化版 hostname: 打印主机名 */
int main(){
    char buf[256];
    if(gethostname(buf, sizeof(buf)) != 0){
        std::cerr<<"hostname: "<<strerror(errno)<<std::endl;
        return EXIT_FAILURE;
    }
    std::cout<<buf<<std::endl;
    return EXIT_SUCCESS;
}
