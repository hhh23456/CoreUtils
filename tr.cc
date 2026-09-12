#include<errno.h>
#include<iostream>
#include<string>
#include<cstring>

/* 简化版 tr:
   tr SET1 SET2      把 SET1 中字符映射为 SET2 对应字符
   tr -d SET         删除 SET 中出现过的字符
   tr -s SET         把连续重复折叠为一个
   支持形如 "a-z" 的范围。 */

/* 展开范围,如 "a-z" -> "abcdefg..."  */
static void expand(const std::string &s, std::string &out){
    for(size_t i=0; i<s.size(); i++){
        if(i+2 < s.size() && s[i+1] == '-' && s[i+2] != '\0'){
            for(char c = s[i]; c <= s[i+2]; c++)
                out += c;
            i += 2;
        } else {
            out += s[i];
        }
    }
}

int main(int argc, char *argv[]){
    bool del=false, squeeze=false;
    int idx=1;
    while(idx<argc && argv[idx][0]=='-' && std::string(argv[idx]).size()>1){
        std::string a=argv[idx];
        for(char c : a.substr(1)){
            if(c=='d') del=true;
            else if(c=='s') squeeze=true;
        }
        idx++;
    }
    if(del){
        if(idx>=argc){ std::cerr<<"tr: missing set\n"<<std::endl; return EXIT_FAILURE; }
        std::string set; expand(argv[idx], set);
        std::string line;
        while(std::getline(std::cin, line)){
            for(char c : line)
                if(set.find(c)==std::string::npos)
                    std::cout<<c;
            std::cout<<std::endl;
        }
        return EXIT_SUCCESS;
    }
    if(argc-idx < 2){ std::cerr<<"tr: missing operands\n"<<std::endl; return EXIT_FAILURE; }
    std::string set1, set2;
    expand(argv[idx], set1);
    expand(argv[idx+1], set2);
    std::string line;
    while(std::getline(std::cin, line)){
        std::string out;
        char last = 0;
        for(char c : line){
            size_t pos = set1.find(c);
            char r = (pos==std::string::npos) ? c : (pos < set2.size() ? set2[pos] : set2.back());
            if(squeeze && r==last) continue;
            out += r;
            last = r;
        }
        std::cout<<out<<std::endl;
    }
    return EXIT_SUCCESS;
}
