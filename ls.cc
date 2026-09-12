#include<iostream>
#include<errno.h>
#include<unistd.h>
#include<cstring>
#include<dirent.h>
void ls(const char *path){
    DIR *dir;
    if((dir = opendir(path)) == NULL){
        std::cerr<<"ls cannot open directory "<<path<<": "<<strerror(errno)<<std::endl;
        return;
    }
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
        std::cout<<entry->d_name<<std::endl;
    }
    if(errno != 0){
        std::cerr<<"ls cannot read directory "<<path<<": "<<strerror(errno)<<std::endl;
    }
    closedir(dir);
}
int main(int argc, char *argv[]){
    if(argc < 2){
        ls(".");
        return EXIT_SUCCESS;
    }
    for(int i=1; i<argc; i++){
        ls(argv[i]);
    }
}