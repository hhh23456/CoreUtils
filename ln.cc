#include<errno.h>
#include<iostream>
#include<string>
#include<cstring>
#include<unistd.h>

/* 简化版 ln:
   ln  target link         硬链接
   ln -s target link       符号链接 */
int main(int argc, char *argv[]){
    if(argc < 3){
        std::cerr<<"Usage: "<<argv[0]<<" [-s] <target> <link>"<<std::endl;
        return EXIT_FAILURE;
    }
    bool sym = false;
    int idx = 1;
    if(std::string(argv[1]) == "-s"){
        sym = true;
        idx = 2;
    }
    if(argc - idx < 2){
        std::cerr<<"Usage: "<<argv[0]<<" [-s] <target> <link>"<<std::endl;
        return EXIT_FAILURE;
    }
    const char *target = argv[idx];
    const char *linkname = argv[idx+1];

    int ret;
    if(sym)
        ret = symlink(target, linkname);
    else
        ret = link(target, linkname);

    if(ret != 0){
        std::cerr<<"ln: cannot create link '"<<linkname<<"': "<<strerror(errno)<<std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
