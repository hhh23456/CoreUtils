#include<errno.h>
#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<cstring>

/* 简化版 cat: cat <file> ...
   无参数时读标准输入 */
static int cat_file(const char *path, int fd){
    char buf[8192];
    ssize_t n;
    while((n = read(fd, buf, sizeof(buf))) > 0){
        ssize_t off = 0;
        while(off < n){
            ssize_t w = write(STDOUT_FILENO, buf + off, n - off);
            if(w < 0){
                std::cerr<<"cat: error writing: "<<strerror(errno)<<std::endl;
                return EXIT_FAILURE;
            }
            off += w;
        }
    }
    if(n < 0){
        std::cerr<<"cat: error reading '"<<path<<"': "<<strerror(errno)<<std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]){
    if(argc < 2)
        return cat_file("stdin", STDIN_FILENO);

    for(int i=1; i<argc; i++){
        int fd = open(argv[i], O_RDONLY);
        if(fd < 0){
            std::cerr<<"cat: cannot open '"<<argv[i]<<"': "<<strerror(errno)<<std::endl;
            continue;
        }
        int ret = cat_file(argv[i], fd);
        close(fd);
        if(ret != EXIT_SUCCESS)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
