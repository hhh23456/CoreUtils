#include<errno.h>
#include<iostream>
#include<sys/stat.h>
#include<cstring>

/* 简化版 mkdir: mkdir <dir> ... */
int main(int argc, char *argv[]){
    if(argc < 2){
        std::cerr<<"Usage: "<<argv[0]<<" <dir> ..."<<std::endl;
        return EXIT_FAILURE;
    }
    for(int i=1; i<argc; i++){
        if(mkdir(argv[i], 0755) != 0){
            std::cerr<<"mkdir: cannot create directory '"<<argv[i]<<"': "<<strerror(errno)<<std::endl;
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
