#include<errno.h>
#include<iostream>
#include<fstream>
#include<string>
#include<regex>
#include<cstring>

/* 简化版 grep: grep <pattern> <file>...
   pattern 用扩展正则(ERE)匹配,打印命中的行
   无文件时读标准输入;多文件时前缀打印文件名 */
static int search(const char *name, std::istream &in, const std::regex &re,
                  bool prefix){
    std::string line;
    int hits = 0;
    while(std::getline(in, line)){
        if(std::regex_search(line, re)){
            if(prefix)
                std::cout<<name<<":";
            std::cout<<line<<std::endl;
            hits++;
        }
    }
    return hits;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        std::cerr<<"Usage: "<<argv[0]<<" <pattern> [file...]"<<std::endl;
        return EXIT_FAILURE;
    }
    const char *pattern = argv[1];
    std::regex re;
    try{
        re = std::regex(pattern);
    } catch(const std::regex_error &e){
        std::cerr<<"grep: invalid pattern '"<<pattern<<"': "<<e.what()<<std::endl;
        return EXIT_FAILURE;
    }

    if(argc == 2){
        return search("stdin", std::cin, re, false) > 0 ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    bool prefix = (argc > 3);
    bool found = false;
    for(int i=2; i<argc; i++){
        std::ifstream in(argv[i]);
        if(!in){
            std::cerr<<"grep: cannot open '"<<argv[i]<<"': "<<strerror(errno)<<std::endl;
            continue;
        }
        if(search(argv[i], in, re, prefix) > 0)
            found = true;
    }
    return found ? EXIT_SUCCESS : EXIT_FAILURE;
}
