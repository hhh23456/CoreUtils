#include<errno.h>
#include<iostream>
#include<unistd.h>
#include<pwd.h>
#include<cstring>

/* 简化版 whoami: 打印有效用户名的用户名 */
int main(){
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if(pw == NULL){
        std::cerr<<"whoami: cannot find username for uid "<<uid<<": "<<strerror(errno)<<std::endl;
        return EXIT_FAILURE;
    }
    std::cout<<pw->pw_name<<std::endl;
    return EXIT_SUCCESS;
}
