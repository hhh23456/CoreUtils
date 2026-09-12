#include<errno.h>
#include<iostream>
#include<unistd.h>
#include<sys/stat.h>
#include<dirent.h>
#include<string>
#include<vector>
#include<cstring>

/* 简化版 rm: rm [-r] [-f] <path>...
   -r  递归删除目录树
   -f  忽略不存在的路径,不询问、不报错 */
static int rm_path(const char *path, bool recursive, bool force){
    if(unlink(path) == 0)
        return 0;

    /* 目录:需要 -r */
    struct stat st;
    if(lstat(path, &st) == 0 && S_ISDIR(st.st_mode)){
        if(!recursive){
            std::cerr<<"rm: cannot remove '"<<path<<"': Is a directory"<<std::endl;
            return -1;
        }
        DIR *dir = opendir(path);
        if(dir == NULL){
            std::cerr<<"rm: cannot open '"<<path<<"': "<<strerror(errno)<<std::endl;
            return -1;
        }
        /* 收集条目再删,避免 opendir 期间目录变化 */
        std::vector<std::string> items;
        struct dirent *e;
        while((e = readdir(dir)) != NULL){
            if(std::string(e->d_name) == "." || std::string(e->d_name) == "..")
                continue;
            items.push_back(std::string(path) + "/" + e->d_name);
        }
        closedir(dir);
        for(const auto &item : items)
            rm_path(item.c_str(), recursive, force);
        if(rmdir(path) != 0){
            std::cerr<<"rm: cannot remove '"<<path<<"': "<<strerror(errno)<<std::endl;
            return -1;
        }
        return 0;
    }

    /* 非目录且失败 */
    if(!force)
        std::cerr<<"rm: cannot remove '"<<path<<"': "<<strerror(errno)<<std::endl;
    return -1;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        std::cerr<<"Usage: "<<argv[0]<<" [-r] [-f] <path>..."<<std::endl;
        return EXIT_FAILURE;
    }
    bool recursive = false, force = false;
    int i = 1;
    for(; i<argc; i++){
        std::string a = argv[i];
        if(a.size() > 1 && a[0] == '-'){
            for(size_t j=1; j<a.size(); j++){
                if(a[j] == 'r') recursive = true;
                else if(a[j] == 'f') force = true;
            }
        } else {
            break;
        }
    }
    int ret = EXIT_SUCCESS;
    for(; i<argc; i++){
        struct stat st;
        if(force && lstat(argv[i], &st) != 0)
            continue;               /* -f 且不存在:忽略 */
        if(rm_path(argv[i], recursive, force) != 0)
            ret = EXIT_FAILURE;
    }
    return ret;
}
