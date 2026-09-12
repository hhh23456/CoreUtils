#include<iostream>
#include<string>

/* 简化版 basename: basename <path> [suffix]
   取路径最后一段,可去后缀 */
int main(int argc, char *argv[]){
    if(argc < 2){
        std::cerr<<"Usage: "<<argv[0]<<" <path> [suffix]"<<std::endl;
        return EXIT_FAILURE;
    }
    std::string p = argv[1];
    std::string base;
    size_t slash = p.rfind('/');
    base = (slash == std::string::npos) ? p : p.substr(slash + 1);
    if(argc >= 3){
        std::string suffix = argv[2];
        if(base.size() > suffix.size() &&
           base.substr(base.size() - suffix.size()) == suffix)
            base.erase(base.size() - suffix.size());
    }
    std::cout<<base<<std::endl;
    return EXIT_SUCCESS;
}
