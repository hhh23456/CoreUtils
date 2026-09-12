#include<iostream>
#include<string>

/* 简化版 dirname: dirname <path>...
   取路径的目录部分 */
static std::string dir_of(const std::string &p){
    size_t end = p.size();
    while(end > 1 && p[end-1] == '/')       /* 去掉末尾斜杠 */
        end--;
    std::string s = p.substr(0, end);
    size_t slash = s.rfind('/');
    if(slash == std::string::npos)
        return ".";
    if(slash == 0)
        return "/";
    return s.substr(0, slash);
}

int main(int argc, char *argv[]){
    if(argc < 2){
        std::cerr<<"Usage: "<<argv[0]<<" <path>..."<<std::endl;
        return EXIT_FAILURE;
    }
    for(int i=1; i<argc; i++)
        std::cout<<dir_of(argv[i])<<std::endl;
    return EXIT_SUCCESS;
}
