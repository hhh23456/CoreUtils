#include<errno.h>
#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<utime.h>
#include<cstring>

/* 简化版 touch: touch <file> ...
   不存在的则创建,存在的则更新访问/修改时间 */
static int touch_file(const char *path){
    int fd = open(path, O_WRONLY | O_CREAT | O_NONBLOCK, 0666);
    if(fd < 0){
        std::cerr<<"touch: cannot touch '"<<path<<"': "<<strerror(errno)<<std::endl;
        return EXIT_FAILURE;
    }
    close(fd);
    if(utime(path, NULL) != 0){
        std::cerr<<"touch: cannot update times of '"<<path<<"': "<<strerror(errno)<<std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        std::cerr<<"Usage: "<<argv[0]<<" <file> ..."<<std::endl;
        return EXIT_FAILURE;
    }
    for(int i=1; i<argc; i++){
        if(touch_file(argv[i]) != EXIT_SUCCESS)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
