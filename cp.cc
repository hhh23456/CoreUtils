#include<errno.h>
#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<cstring>

/* 简化版 cp: cp <src> <dst>
   只支持普通文件到普通文件,不做目录递归、不做权限/时间戳保留 */
static int copy_file(const char *src, const char *dst){
    int in = open(src, O_RDONLY);
    if(in < 0){
        std::cerr<<"cp: cannot open '"<<src<<"': "<<strerror(errno)<<std::endl;
        return EXIT_FAILURE;
    }
    int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(out < 0){
        std::cerr<<"cp: cannot create '"<<dst<<"': "<<strerror(errno)<<std::endl;
        close(in);
        return EXIT_FAILURE;
    }

    char buf[8192];
    ssize_t n;
    int ret = EXIT_SUCCESS;
    while((n = read(in, buf, sizeof(buf))) > 0){
        ssize_t off = 0;
        while(off < n){
            ssize_t w = write(out, buf + off, n - off);
            if(w < 0){
                std::cerr<<"cp: error writing '"<<dst<<"': "<<strerror(errno)<<std::endl;
                ret = EXIT_FAILURE;
                break;
            }
            off += w;
        }
        if(ret != EXIT_SUCCESS)
            break;
    }
    if(n < 0){
        std::cerr<<"cp: error reading '"<<src<<"': "<<strerror(errno)<<std::endl;
        ret = EXIT_FAILURE;
    }

    if(close(out) < 0){
        std::cerr<<"cp: error closing '"<<dst<<"': "<<strerror(errno)<<std::endl;
        ret = EXIT_FAILURE;
    }
    close(in);
    return ret;
}

int main(int argc, char *argv[]){
    if(argc < 3){
        std::cerr<<"Usage: "<<argv[0]<<" <src> <dst>"<<std::endl;
        return EXIT_FAILURE;
    }
    for(int i=1; i<argc-1; i++){
        if(copy_file(argv[i], argv[argc-1]) != EXIT_SUCCESS)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
