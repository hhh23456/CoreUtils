#include<errno.h>
#include<iostream>
#include<string>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<cstring>
#include<cstdlib>

/* 简化版 install:
   install [-d] [-m mode] <src>... <dst>   拷贝文件(可设权限)
   install -d [-m mode] <dir>...           创建目录(含父目录) */
static int copy_file(const char *src, const char *dst, mode_t mode){
    int in = open(src, O_RDONLY);
    if(in < 0){
        std::cerr<<"install: cannot open '"<<src<<"': "<<strerror(errno)<<std::endl;
        return -1;
    }
    int out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if(out < 0){
        std::cerr<<"install: cannot create '"<<dst<<"': "<<strerror(errno)<<std::endl;
        close(in);
        return -1;
    }
    char buf[8192]; ssize_t n;
    while((n = read(in, buf, sizeof(buf))) > 0){
        ssize_t off = 0;
        while(off < n){
            ssize_t w = write(out, buf + off, n - off);
            if(w < 0){ std::cerr<<"install: write error"<<std::endl; close(in); close(out); return -1; }
            off += w;
        }
    }
    close(in); close(out);
    chmod(dst, mode);
    return 0;
}

int main(int argc, char *argv[]){
    bool isdir = false;
    mode_t mode = 0755;
    int idx = 1;
    while(idx < argc && argv[idx][0]=='-' && std::string(argv[idx]).size()>1){
        std::string a = argv[idx];
        if(a == "-d") isdir = true;
        else if(a == "-m" && idx+1 < argc) mode = (mode_t)std::strtol(argv[++idx], NULL, 8);
        else { std::cerr<<"install: unknown option '"<<a<<"'"<<std::endl; return EXIT_FAILURE; }
        idx++;
    }
    if(idx >= argc){
        std::cerr<<"Usage: "<<argv[0]<<" [-d] [-m mode] <src>... <dst>"<<std::endl;
        return EXIT_FAILURE;
    }

    if(isdir){
        for(; idx<argc; idx++)
            if(mkdir(argv[idx], mode) != 0 && errno != EEXIST){
                std::cerr<<"install: cannot create directory '"<<argv[idx]<<"': "<<strerror(errno)<<std::endl;
                return EXIT_FAILURE;
            }
        return EXIT_SUCCESS;
    }

    const char *dst = argv[argc-1];
    int nsrc = argc - idx - 1;
    if(nsrc < 1){
        std::cerr<<"install: missing operand"<<std::endl;
        return EXIT_FAILURE;
    }
    struct stat st;
    bool dst_is_dir = (stat(dst, &st)==0 && S_ISDIR(st.st_mode));
    for(int i=idx; i<argc-1; i++){
        std::string target;
        if(dst_is_dir){
            const char *sl = strrchr(argv[i], '/');
            target = std::string(dst) + "/" + (sl ? sl+1 : argv[i]);
        } else {
            target = dst;
        }
        if(copy_file(argv[i], target.c_str(), mode) != 0)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
