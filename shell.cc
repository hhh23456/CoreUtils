#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<csignal>
#include<cstdio>
#include<cerrno>
#include<cstring>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/types.h>

/* 简化版 shell: 逐行读取 → 按空白切词 → 内建或外部命令执行
   无管道 / 重定向 / 脚本等编程功能 */

static std::vector<std::string> tokenize(const std::string &line){
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string t;
    while(iss >> t)
        tokens.push_back(t);
    return tokens;
}

static int run_external(const std::vector<std::string> &args){
    pid_t pid = fork();
    if(pid < 0){
        std::cerr<<"shell: fork failed: "<<strerror(errno)<<std::endl;
        return EXIT_FAILURE;
    }
    if(pid == 0){
        std::vector<char*> argv;
        for(const auto &a : args)
            argv.push_back(const_cast<char*>(a.c_str()));
        argv.push_back(NULL);
        /* execvp 按 PATH 查找,找不到则尝试绝对/相对路径 */
        execvp(argv[0], argv.data());
        std::cerr<<"shell: "<<args[0]<<": "<<strerror(errno)<<std::endl;
        _exit(EXIT_FAILURE);
    }
    int status;
    while(waitpid(pid, &status, 0) < 0){
        if(errno != EINTR){
            std::cerr<<"shell: waitpid failed: "<<strerror(errno)<<std::endl;
            return EXIT_FAILURE;
        }
    }
    if(WIFEXITED(status))
        return WEXITSTATUS(status);
    return EXIT_FAILURE;
}

int main(){
    std::string line;
    bool interactive = ::isatty(STDIN_FILENO);
    while(true){
        if(interactive){
            char cwd[4096];
            if(getcwd(cwd, sizeof(cwd)))
                std::cout<<cwd<<" $ ";
            else
                std::cout<<"? $ ";
            std::cout<<std::flush;
        }
        if(!std::getline(std::cin, line))
            break;                       /* EOF */
        if(line.empty())
            continue;

        std::vector<std::string> args = tokenize(line);
        if(args.empty())
            continue;

        /* 内建命令 */
        if(args[0] == "exit" || args[0] == "quit")
            return EXIT_SUCCESS;
        if(args[0] == "cd"){
            if(args.size() < 2 || args[1] == "~")
                chdir(getenv("HOME") ? getenv("HOME") : "/");
            else if(chdir(args[1].c_str()) != 0)
                std::cerr<<"shell: cd: "<<args[1]<<": "<<strerror(errno)<<std::endl;
            continue;
        }

        run_external(args);
    }
    if(interactive)
        std::cout<<std::endl;
    return EXIT_SUCCESS;
}
