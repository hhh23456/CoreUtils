#include<iostream>
#include<fstream>
#include<string>

/* 简化版 wc: 统计每个文件的 行数/词数/字节数,末尾合计
   wc <file> ... */
int main(int argc, char *argv[]){
    if(argc < 2){
        /* 无参数时读标准输入 */
        long l=0, w=0, b=0;
        std::string line;
        bool inword = false;
        while(std::getline(std::cin, line)){
            l++;
            b += line.size() + 1;
            for(char c : line){
                if(c==' '||c=='\t'||c=='\n'){
                    inword = false;
                } else if(!inword){
                    inword = true;
                    w++;
                }
            }
            inword = false;
        }
        std::cout<<"   "<<l<<"   "<<w<<"   "<<b<<std::endl;
        return EXIT_SUCCESS;
    }
    long tl=0, tw=0, tb=0;
    for(int i=1; i<argc; i++){
        std::ifstream in(argv[i], std::ios::binary);
        if(!in){
            std::cerr<<"wc: cannot open '"<<argv[i]<<"'"<<std::endl;
            return EXIT_FAILURE;
        }
        long l=0, w=0, b=0;
        std::string line;
        bool inword = false;
        while(std::getline(in, line)){
            l++;
            b += line.size() + 1;
            for(char c : line){
                if(c==' '||c=='\t'||c=='\n'){
                    inword = false;
                } else if(!inword){
                    inword = true;
                    w++;
                }
            }
            inword = false;
        }
        std::cout<<"   "<<l<<"   "<<w<<"   "<<b<<" "<<argv[i]<<std::endl;
        tl += l; tw += w; tb += b;
    }
    if(argc > 2)
        std::cout<<"   "<<tl<<"   "<<tw<<"   "<<tb<<" total"<<std::endl;
    return EXIT_SUCCESS;
}
