#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/*
or |
xor ^
and &
inverse ~
*/


void printBoard(uint64_t wpawn, uint64_t wrook, uint64_t wknight, uint64_t wbishop, uint64_t wqueen, uint64_t wking, uint64_t bpawn, uint64_t brook, uint64_t bknight, uint64_t bbishop, uint64_t bqueen, uint64_t bking, uint64_t otherinfos){
    uint64_t target = 9223372036854775808;
    for (int i = 0; i < 64; i++){
        if (!(i%8)){
            printf("\n");
        }

        if (target & wpawn){
            printf("♟ ");
        }
        else if (target & wrook){
            printf("♜ ");
        }
        else if (target & wknight){
            printf("♞ ");
        }
        else if (target & wbishop){
            printf("♝ ");
        }
        else if (target & wqueen){
            printf("♛ ");
        }
        else if (target & wking){
            printf("♚ ");
        }
        else if (target & bpawn){
            printf("♙ ");
        }
        else if (target & brook){
            printf("♖ ");
        }
        else if (target & bknight){
            printf("♘ ");
        }
        else if (target & bbishop){
            printf("♗ ");
        }
        else if (target & bqueen){
            printf("♕ ");
        }
        else if (target & bking){
            printf("♔ ");
        }

        else{
            printf("  ");
        }
        target /= 2;
    }
}


char getNthBNum(uint64_t x, int n){//getNthBNum(001, 0) = 1, getNthBNum(001, 1) = 0, getNthBNum(001, 2) = 0
    uint64_t target = 1<<n;
    if ((x & target) != 0){
        return 1;
    }
    return 0;
}


void printToB(uint64_t x){
    uint64_t target = 9223372036854775808;
    for (int i = 0; i < 64; i++){
        if (!(i%8)){
            printf("\n");
        }
        if (target & x){
            printf("1 ");
        }
        else{
            printf("0 ");
        }
        target /= 2;
    }
}


int main(){
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

    uint64_t wpawn = 65280;
    uint64_t wrook = 129;
    uint64_t wknight = 66;
    uint64_t wbishop = 36;
    uint64_t wqueen = 16;
    uint64_t wking = 8;

    uint64_t bpawn = 71776119061217280;
    uint64_t brook = 9295429630892703744;
    uint64_t bknight = 4755801206503243776;
    uint64_t bbishop = 2594073385365405696;
    uint64_t bqueen = 1152921504606846976;
    uint64_t bking = 576460752303423488;

    unsigned long pawnmv = 65535;//first 16 bits are for if it can take with en-passant \\ last 16 bits for if the first move is done

    unsigned char otherinfos = 241;//bleft bright wleft wright 0 0 \\ 1 bturn \\ 1 wturn

    printBoard(wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, otherinfos);
    //printToB(wpawn);
    return 0;
}
