#include<errno.h>
#include<iostream>
#include<string>
#include<cstdlib>
#include<cstring>

/* 简化版 expr:
   expr 整数 + - * / % 整数      算术
   expr a OP b                  字符串/数字比较 (= != < > <= >=)
   expr length <str>            字符串长度
   expr substr <s> <pos> <len>  取子串(位置从1起)
   仅支持二元运算,不做优先级链。
   退出码:结果为 0 或假→1,非0/真→0。 */

static bool is_int(const std::string &s){
    if(s.empty()) return false;
    for(size_t i=0;i<s.size();i++)
        if(!(s[i]>='0'&&s[i]<='9') && !(s[i]=='-'&&i==0)) return false;
    return true;
}

int main(int argc, char *argv[]){
    if(argc == 3 && std::string(argv[1])=="length"){
        std::cout<<std::string(argv[2]).size()<<std::endl;
        return EXIT_SUCCESS;
    }
    if(argc == 5 && std::string(argv[1])=="substr"){
        std::string s = argv[2];
        long pos = std::atol(argv[3]);
        long len = std::atol(argv[4]);
        std::cout<<s.substr(pos<1?0:pos-1, len<0?0:len)<<std::endl;
        return EXIT_SUCCESS;
    }
    if(argc != 4){
        std::cerr<<"expr: syntax error"<<std::endl;
        return EXIT_FAILURE;
    }

    std::string op = argv[2];
    std::string a = argv[1], b = argv[3];

    auto out_truth = [](bool t){
        std::cout<<(t?1:0)<<std::endl;
        return t ? EXIT_SUCCESS : EXIT_FAILURE;
    };

    if(is_int(a) && is_int(b)){
        long x = std::atol(a.c_str()), y = std::atol(b.c_str());
        if(op=="="||op=="==") return out_truth(x==y);
        if(op=="!=") return out_truth(x!=y);
        if(op=="<") return out_truth(x<y);
        if(op==">") return out_truth(x>y);
        if(op=="<=") return out_truth(x<=y);
        if(op==">=") return out_truth(x>=y);
        if(!y && (op=="/"||op=="%")){ std::cerr<<"expr: division by zero"<<std::endl; return EXIT_FAILURE; }
        long r;
        if(op=="+") r=x+y;
        else if(op=="-") r=x-y;
        else if(op=="*") r=x*y;
        else if(op=="/") r=x/y;
        else if(op=="%") r=x%y;
        else { std::cerr<<"expr: syntax error"<<std::endl; return EXIT_FAILURE; }
        std::cout<<r<<std::endl;
        return r != 0 ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    if(op=="="||op=="==") return out_truth(a==b);
    if(op=="!=") return out_truth(a!=b);
    if(op=="<") return out_truth(a<b);
    if(op==">") return out_truth(a>b);

    std::cerr<<"expr: syntax error"<<std::endl;
    return EXIT_FAILURE;
}
