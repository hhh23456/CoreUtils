#include<errno.h>
#include<iostream>
#include<unistd.h>
#include<sys/stat.h>
#include<string>
#include<cstring>

/* 简化版 mv: mv <src> <dst> (仅单文件到单文件)
   mv <src>... <dir> (多个源移到目录)
   不处理跨文件系统的情况 */
int main(int argc, char *argv[]){
    if(argc < 3){
        std::cerr<<"Usage: "<<argv[0]<<" <src> <dst> 或 "<<argv[0]<<" <src>... <dir>"<<std::endl;
        return EXIT_FAILURE;
    }
    const char *last = argv[argc-1];
    int ret = EXIT_SUCCESS;

    /* 目标是否是目录 */
    struct stat st;
    bool isdir = (stat(last, &st) == 0 && S_ISDIR(st.st_mode));

    if(!isdir && argc != 3){
        std::cerr<<"mv: target '"<<last<<"' is not a directory"<<std::endl;
        return EXIT_FAILURE;
    }

    for(int i=1; i<argc-1; i++){
        std::string dst;
        if(isdir){
            const char *slash = strrchr(argv[i], '/');
            const char *base = slash ? slash + 1 : argv[i];
            dst = std::string(last) + "/" + base;
        }
        else
            dst = last;
        if(rename(argv[i], dst.c_str()) != 0){
            std::cerr<<"mv: cannot move '"<<argv[i]<<"' to '"<<dst<<"': "<<strerror(errno)<<std::endl;
            ret = EXIT_FAILURE;
        }
    }
    return ret;
}
