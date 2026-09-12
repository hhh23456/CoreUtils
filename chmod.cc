#include<errno.h>
#include<iostream>
#include<sys/stat.h>
#include<cstdlib>
#include<cstring>

/* 简化版 chmod: chmod <mode> <file> ...
   mode 用八进制数字,如 755 / 644 */
int main(int argc, char *argv[]){
    if(argc < 3){
        std::cerr<<"Usage: "<<argv[0]<<" <mode> <file> ..."<<std::endl;
        return EXIT_FAILURE;
    }
    mode_t mode = (mode_t)std::strtol(argv[1], NULL, 8);
    int ret = EXIT_SUCCESS;
    for(int i=2; i<argc; i++){
        if(chmod(argv[i], mode) != 0){
            std::cerr<<"chmod: cannot change mode of '"<<argv[i]<<"': "<<strerror(errno)<<std::endl;
            ret = EXIT_FAILURE;
        }
    }
    return ret;
}
