#include<errno.h>
#include<iostream>
#include<fstream>
#include<string>
#include<regex>
#include<vector>
#include<cstring>

/* 简化版 sed: 支持最常用替换子集
   sed [addr]s/old/new/[g] [file...]
     addr 可选:行号 N 或 /正则/ 或空(所有行)
     s/PAT/REP/g  用 REP 替换匹配, g 为全局(否则每行第一个)
   无 -n,打印所有经过处理的行;无 /s 时原样打印。
   文件缺省读标准输入。 */

struct Rule{ bool hasPat; std::regex pat; bool hasLine; long line;
             std::regex re; std::string repl; bool global; bool doSub; };

int main(int argc, char *argv[]){
    if(argc < 2){
        std::cerr<<"Usage: "<<argv[0]<<" [addr]s/old/new/[g] [file...]"<<std::endl;
        return EXIT_FAILURE;
    }

    std::string prog = argv[1];
    int idx = 2;

    /* 解析地址(可选): 行号 或 /re/ */
    Rule r; r.hasPat=false; r.hasLine=false; r.doSub=false; r.global=false;
    std::string rest = prog;
    if(!rest.empty() && rest[0] == '/'){
        size_t e = rest.find('/', 1);
        if(e == std::string::npos){
            std::cerr<<"sed: bad address"<<std::endl;
            return EXIT_FAILURE;
        }
        r.hasPat = true;
        r.pat = std::regex(rest.substr(1, e-1));
        rest = rest.substr(e+1);
    } else if(!rest.empty() && rest[0]>='0' && rest[0]<='9'){
        r.hasLine = true;
        r.line = std::atol(rest.c_str());
        size_t e = rest.find('s');
        if(e==std::string::npos){ std::cerr<<"sed: bad address"<<std::endl; return EXIT_FAILURE; }
        rest = rest.substr(e);
    }

    /* 动作 s/PAT/REP/[g] */
    if(rest.empty() || rest[0] != 's'){
        std::cerr<<"sed: unsupported (only s/// supported)"<<std::endl;
        return EXIT_FAILURE;
    }
    r.doSub = true;
    std::string s = rest.substr(1);
    if(s.size() < 5){ std::cerr<<"sed: bad s command"<<std::endl; return EXIT_FAILURE; }
    char delim = s[0];
    size_t e1 = s.find(delim, 1);
    if(e1 == std::string::npos){ std::cerr<<"sed: bad s command"<<std::endl; return EXIT_FAILURE; }
    size_t e2 = s.find(delim, e1+1);
    if(e2 == std::string::npos){ std::cerr<<"sed: bad s command"<<std::endl; return EXIT_FAILURE; }
    r.re = std::regex(s.substr(1, e1-1));
    r.repl = s.substr(e1+1, e2-e1-1);
    if(e2+1 < s.size() && s[e2+1] == 'g')
        r.global = true;
    rest = "";

    auto apply = [&](const std::string &line){
        if(r.hasLine){}
        return std::regex_replace(line, r.re, r.repl, r.global
                                  ? std::regex_constants::match_default
                                  : std::regex_constants::format_first_only);
    };

    auto process = [&](std::istream &in){
        std::string line; long ln = 0;
        while(std::getline(in, line)){
            ln++;
            bool hit = true;
            if(r.hasLine) hit = (ln == r.line);
            if(r.hasPat) hit = std::regex_search(line, r.pat);
            if(hit)
                std::cout<<apply(line)<<std::endl;
            else
                std::cout<<line<<std::endl;
        }
    };

    if(idx >= argc)
        process(std::cin);
    else
        for(; idx<argc; idx++){
            std::ifstream in(argv[idx]);
            if(!in){
                std::cerr<<"sed: cannot open '"<<argv[idx]<<"': "<<strerror(errno)<<std::endl;
                return EXIT_FAILURE;
            }
            process(in);
        }
    return EXIT_SUCCESS;
}
