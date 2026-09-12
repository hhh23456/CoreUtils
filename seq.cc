#include<iostream>
#include<cstdlib>

/* 简化版 seq:
   seq <last>                   1..last
   seq <first> <last>           first..last
   seq <first> <incr> <last>    first, first+incr, ... */

int main(int argc, char *argv[]){
    long first=1, incr=1, last;
    if(argc==2){
        last = std::atol(argv[1]);
    } else if(argc==3){
        first = std::atol(argv[1]);
        last  = std::atol(argv[2]);
    } else if(argc==4){
        first = std::atol(argv[1]);
        incr  = std::atol(argv[2]);
        last  = std::atol(argv[3]);
    } else {
        std::cerr<<"Usage: "<<argv[0]<<" [first [incr]] last"<<std::endl;
        return EXIT_FAILURE;
    }
    if(incr == 0){
        std::cerr<<"seq: invalid increment 0"<<std::endl;
        return EXIT_FAILURE;
    }
    if((incr > 0 && last < first) || (incr < 0 && last > first))
        return EXIT_SUCCESS;

    for(long v=first; (incr>0 ? v<=last : v>=last); v+=incr)
        std::cout<<v<<std::endl;
    return EXIT_SUCCESS;
}
