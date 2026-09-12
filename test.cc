#include<errno.h>
#include<iostream>
#include<string>
#include<cstring>
#include<cstdlib>
#include<sys/stat.h>
#include<unistd.h>

/* 简化版 test: 条件判断,供 shell 脚本使用
   test  expr         或   [ expr ]
   支持常用子集:
     文件: -e -f -d -x -r -w -L
     字符串: -n -z, str1 = str2, str1 != str2, str 非空
     数字: 整数 -eq -ne -lt -gt -le -ge 整数 */
static bool is_int(const char *s){
    if(*s == '\0') return false;
    for(const char *p = s; *p; p++){
        if(*p=='-' && p==s) continue;
        if(*p < '0' || *p > '9') return false;
    }
    return true;
}

int main(int argc, char *argv[]){
    /* 去掉 "[ ... ]" 形式的结尾 "]" */
    int n = argc - 1;
    if(n >= 1 && std::string(argv[n]) == "]")
        n--;

    if(n == 1){                      /* 单参数:非空字符串为真 */
        bool ok = argv[1][0] != '\0';
        return ok ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    if(n == 2){
        std::string op = argv[1];
        const char *a = argv[2];
        if(op == "-n") return a[0] != '\0' ? EXIT_SUCCESS : EXIT_FAILURE;
        if(op == "-z") return a[0] == '\0' ? EXIT_SUCCESS : EXIT_FAILURE;
        struct stat st;
        if(op == "-e") return lstat(a,&st)==0 ? EXIT_SUCCESS : EXIT_FAILURE;
        if(op == "-f") return stat(a,&st)==0 && S_ISREG(st.st_mode) ? EXIT_SUCCESS : EXIT_FAILURE;
        if(op == "-d") return stat(a,&st)==0 && S_ISDIR(st.st_mode) ? EXIT_SUCCESS : EXIT_FAILURE;
        if(op == "-L") return lstat(a,&st)==0 && S_ISLNK(st.st_mode) ? EXIT_SUCCESS : EXIT_FAILURE;
        if(op == "-r") return access(a,R_OK)==0 ? EXIT_SUCCESS : EXIT_FAILURE;
        if(op == "-w") return access(a,W_OK)==0 ? EXIT_SUCCESS : EXIT_FAILURE;
        if(op == "-x") return access(a,X_OK)==0 ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    if(n == 3){
        std::string op = argv[2];
        const char *a = argv[1];
        const char *b = argv[3];
        if(op == "=" || op == "==") return std::string(a)==std::string(b) ? EXIT_SUCCESS : EXIT_FAILURE;
        if(op == "!=")       return std::string(a)!=std::string(b) ? EXIT_SUCCESS : EXIT_FAILURE;
        if(op == "-eq"){ if(!is_int(a)||!is_int(b)) return EXIT_FAILURE; return std::atol(a)==std::atol(b) ? EXIT_SUCCESS : EXIT_FAILURE; }
        if(op == "-ne"){ if(!is_int(a)||!is_int(b)) return EXIT_FAILURE; return std::atol(a)!=std::atol(b) ? EXIT_SUCCESS : EXIT_FAILURE; }
        if(op == "-lt"){ if(!is_int(a)||!is_int(b)) return EXIT_FAILURE; return std::atol(a)< std::atol(b) ? EXIT_SUCCESS : EXIT_FAILURE; }
        if(op == "-gt"){ if(!is_int(a)||!is_int(b)) return EXIT_FAILURE; return std::atol(a)> std::atol(b) ? EXIT_SUCCESS : EXIT_FAILURE; }
        if(op == "-le"){ if(!is_int(a)||!is_int(b)) return EXIT_FAILURE; return std::atol(a)<=std::atol(b) ? EXIT_SUCCESS : EXIT_FAILURE; }
        if(op == "-ge"){ if(!is_int(a)||!is_int(b)) return EXIT_FAILURE; return std::atol(a)>=std::atol(b) ? EXIT_SUCCESS : EXIT_FAILURE; }
    }
    std::cerr<<"test: unsupported expression"<<std::endl;
    return EXIT_FAILURE;
}
