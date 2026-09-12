#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>

/* 简化版 sort: 对文件所有行排序后输出
   sort <file> ... */
int main(int argc, char *argv[]){
    if(argc < 2){
        std::cerr<<"Usage: "<<argv[0]<<" <file> ..."<<std::endl;
        return EXIT_FAILURE;
    }
    std::vector<std::string> lines;
    for(int i=1; i<argc; i++){
        std::ifstream in(argv[i]);
        if(!in){
            std::cerr<<"sort: cannot open '"<<argv[i]<<"'"<<std::endl;
            return EXIT_FAILURE;
        }
        std::string line;
        while(std::getline(in, line))
            lines.push_back(line);
    }
    std::sort(lines.begin(), lines.end());
    for(const auto &l : lines)
        std::cout<<l<<std::endl;
    return EXIT_SUCCESS;
}
