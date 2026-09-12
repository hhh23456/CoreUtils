#include<errno.h>
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<cstdlib>

/* 简化版 head: 打印文件前 10 行(可用 -n 指定行数)
   head [-n N] <file> ... */
int main(int argc, char *argv[]){
    int lines = 10;
    int idx = 1;
    if(argc > 1 && std::string(argv[1]) == "-n" && argc > 2){
        lines = std::atoi(argv[2]);
        idx = 3;
    }
    if(idx >= argc){
        std::cerr<<"Usage: "<<argv[0]<<" [-n N] <file> ..."<<std::endl;
        return EXIT_FAILURE;
    }
    for(; idx<argc; idx++){
        std::ifstream in(argv[idx]);
        if(!in){
            std::cerr<<"head: cannot open '"<<argv[idx]<<"': "<<strerror(errno)<<std::endl;
            return EXIT_FAILURE;
        }
        std::string line;
        int count = 0;
        while(count < lines && std::getline(in, line)){
            std::cout<<line<<std::endl;
            count++;
        }
    }
    return EXIT_SUCCESS;
}
