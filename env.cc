#include<errno.h>
#include<iostream>
#include<string>
#include<vector>
#include<cstring>
#include<unistd.h>

/* 简化版 env:
   env                  打印所有环境变量
   env NAME=VALUE... cmd args...  设置环境后运行命令 */
extern char **environ;

int main(int argc, char *argv[]){
    if(argc < 2){
        for(char **e = environ; *e; e++)
            std::cout<<*e<<std::endl;
        return EXIT_SUCCESS;
    }

    /* 找出设置项 NAME=VALUE 和命令 */
    std::vector<std::string> sets;
    int i = 1;
    for(; i<argc; i++){
        std::string a = argv[i];
        size_t eq = a.find('=');
        if(eq != std::string::npos && eq > 0)
            sets.push_back(a);
        else
            break;
    }

    for(const auto &kv : sets){
        size_t eq = kv.find('=');
        std::string name = kv.substr(0, eq);
        std::string val  = kv.substr(eq + 1);
        setenv(name.c_str(), val.c_str(), 1);
    }

    if(i >= argc){
        for(char **e = environ; *e; e++)
            std::cout<<*e<<std::endl;
        return EXIT_SUCCESS;
    }

    /* 运行命令 */
    std::vector<char*> args;
    for(; i<argc; i++)
        args.push_back(argv[i]);
    args.push_back(NULL);
    execvp(args[0], args.data());
    std::cerr<<"env: "<<args[0]<<": "<<strerror(errno)<<std::endl;
    return EXIT_FAILURE;
}
