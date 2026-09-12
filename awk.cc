#include<errno.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<regex>
#include<cstring>

/* 简化版 awk: awk [-F 分隔符] '程序' [file...]
   支持最常用子集:
     - 动作  {print $1, $2 ...}: $N 取字段、$NF、$0 整行
     - 内建  NR(行号)、NF(字段数)、FILENAME
     - 模式  /正则/ { ... }: 仅对匹配的行执行
   字段默认按空白分隔,可用 -F 指定(单字符)。 */

struct Line{
    std::string text;
    std::vector<std::string> fs;      /* 拆分后的字段 */
    int n;                            /* 字段数 */
    long nr;                          /* 行号 */
};

static void split_fields(Line &ln, const std::string &sep){
    if(sep == " "){
        std::istringstream iss(ln.text);
        std::string t;
        while(iss >> t)
            ln.fs.push_back(t);
    } else {
        std::string cur;
        for(char c : ln.text){
            if(sep.find(c) != std::string::npos){
                if(!cur.empty() || true) ln.fs.push_back(cur);
                cur.clear();
            } else {
                cur += c;
            }
        }
        ln.fs.push_back(cur);
    }
    ln.n = (int)ln.fs.size();
}

static std::string field(const Line &ln, const std::string &ref){
    int idx;
    if(ref == "0") return ln.text;
    if(ref == "NF") idx = ln.n;
    else idx = std::atoi(ref.c_str());
    if(idx < 1 || idx > ln.n) return "";
    return ln.fs[idx-1];
}

/* 解析 /regex/ 过滤(可选) */
static bool parse_pattern(const std::string &prog, std::regex *re, std::string &rest){
    if(prog.empty() || prog[0] != '/')
        return true;      /* 无模式 */
    auto end = prog.find('/', 1);
    if(end == std::string::npos) return false;
    *re = std::regex(prog.substr(1, end-1));
    rest = prog.substr(end+1);
    return true;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        std::cerr<<"Usage: "<<argv[0]<<" [-F sep] 'program' [file...]"<<std::endl;
        return EXIT_FAILURE;
    }
    std::string sep = " ";
    int idx = 1;
    if(argc > 1 && std::string(argv[1]).rfind("-F", 0) == 0){
        std::string a = argv[1];
        if(a.size() > 2){           /* 连写: -F, */
            sep = a.substr(2);
            idx = 2;
        } else if(argc > 2){        /* 分开: -F , */
            sep = argv[2];
            idx = 3;
        } else {
            std::cerr<<"awk: -F requires an argument"<<std::endl;
            return EXIT_FAILURE;
        }
    }
    if(idx >= argc){
        std::cerr<<"awk: missing program"<<std::endl;
        return EXIT_FAILURE;
    }
    std::string prog = argv[idx++];

    std::regex re;
    std::string body;
    if(!parse_pattern(prog, &re, body)){
        std::cerr<<"awk: invalid pattern"<<std::endl;
        return EXIT_FAILURE;
    }
    if(body.empty())
        body = prog;                 /* 无 /正则/ 模式时程序即 body */
    bool has_pattern = !body.empty() && prog[0]=='/';

    /* 解析动作 body:{print ...} 或 {print} */
    size_t lb = body.find('{');
    size_t rb = body.rfind('}');
    if(lb == std::string::npos || rb==std::string::npos || rb<lb){
        std::cerr<<"awk: unsupported program '"<<prog<<"'"<<std::endl;
        return EXIT_FAILURE;
    }
    std::string action = body.substr(lb+1, rb-lb-1);
    bool do_print = (action.find("print") != std::string::npos);
    std::string args = action.substr(action.find("print") + 5);

    auto process_line = [&](Line &ln, const std::string &fname){
        if(has_pattern && !std::regex_search(ln.text, re))
            return;
        if(!do_print) return;
        /* 解析 print 参数:逗号/空格分隔 */
        std::vector<std::string> parts;
        std::string cur;
        for(char c : args){
            if(c==',' || c==' ' || c=='\t'){
                if(!cur.empty()) parts.push_back(cur);
                cur.clear();
            } else cur += c;
        }
        if(!cur.empty()) parts.push_back(cur);
        for(size_t i=0; i<parts.size(); i++){
            if(i>0) std::cout<<" ";
            std::string p = parts[i];
            if(!p.empty() && p[0]=='$')
                std::cout<<field(ln, p.substr(1));
            else if(p=="NR")
                std::cout<<ln.nr;
            else if(p=="NF")
                std::cout<<ln.n;
            else if(p=="FILENAME")
                std::cout<<fname;
            else
                std::cout<<p;
        }
        std::cout<<std::endl;
    };

    long nr = 0;
    if(idx >= argc){
        std::string line;
        while(std::getline(std::cin, line)){
            nr++;
            Line ln{line, {}, 0, nr};
            split_fields(ln, sep);
            process_line(ln, "stdin");
        }
    } else {
        for(; idx<argc; idx++){
            std::ifstream in(argv[idx]);
            if(!in){
                std::cerr<<"awk: cannot open '"<<argv[idx]<<"': "<<strerror(errno)<<std::endl;
                return EXIT_FAILURE;
            }
            std::string line;
            while(std::getline(in, line)){
                nr++;
                Line ln{line, {}, 0, nr};
                split_fields(ln, sep);
                process_line(ln, argv[idx]);
            }
        }
    }
    return EXIT_SUCCESS;
}
