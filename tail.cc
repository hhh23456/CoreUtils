#include<errno.h>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cstring>
#include<cstdlib>

/* 简化版 tail: 打印文件末尾 10 行(可用 -n 指定行数)
   tail [-n N] <file> ... */
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
            std::cerr<<"tail: cannot open '"<<argv[idx]<<"': "<<strerror(errno)<<std::endl;
            return EXIT_FAILURE;
        }
        std::vector<std::string> rbuf;
        std::string line;
        while(std::getline(in, line))
            rbuf.push_back(line);
        int start = (int)rbuf.size() - lines;
        if(start < 0) start = 0;
        for(int i=start; i<(int)rbuf.size(); i++)
            std::cout<<rbuf[i]<<std::endl;
    }
    return EXIT_SUCCESS;
}
