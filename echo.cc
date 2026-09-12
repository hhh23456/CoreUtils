#include<iostream>

/* 简化版 echo: 打印参数,以空格分隔,末尾换行 */
int main(int argc, char *argv[]){
    for(int i=1; i<argc; i++){
        if(i > 1)
            std::cout<<" ";
        std::cout<<argv[i];
    }
    std::cout<<std::endl;
    return EXIT_SUCCESS;
}
