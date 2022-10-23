#include <stdlib.h>
#include <stdio.h>

/*
or |
xor ^
and &
inverse ~
*/

char nthBNum(unsigned long long x, int n){//nthBNum(001, 0) = 1, nthBNum(001, 1) = 0, nthBNum(001, 2) = 0
    unsigned long long target = 1<<n;
    if ((x & target) != 0){
        return 1;
    }
    return 0;
}

void printToB(long long x){
    
}

int main(){
    unsigned long long wrook;
    unsigned long long brook;
    unsigned long long wbishop;
    
    char awr = nthBNum(wrook, 3);
    printf("%d", awr);
    return 0;
}
