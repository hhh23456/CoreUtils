#include<iostream>
#include<ctime>
#include<iomanip>

/* 简化版 date: 打印当前日期时间 */
int main(){
    std::time_t now = std::time(NULL);
    char buf[128];
    std::strftime(buf, sizeof(buf), "%a %b %e %H:%M:%S %Y", std::localtime(&now));
    std::cout<<buf<<std::endl;
    return EXIT_SUCCESS;
}
