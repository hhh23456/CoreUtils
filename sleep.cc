#include<cstdlib>
#include<unistd.h>

/* 简化版 sleep: sleep <seconds> ,仅支持整数秒 */
int main(int argc, char *argv[]){
    if(argc < 2)
        return EXIT_FAILURE;
    unsigned sec = (unsigned)std::atoi(argv[1]);
    for(unsigned i=0; i<sec; i++){
        if(sleep(1) == 0) continue;
        break;
    }
    return EXIT_SUCCESS;
}
