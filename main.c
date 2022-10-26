#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/*
or |
xor ^
and &
inverse ~
*/


void printBoard(uint64_t wpawn, uint64_t wrook, uint64_t wknight, uint64_t wbishop, uint64_t wqueen, uint64_t wking, uint64_t bpawn, uint64_t brook, uint64_t bknight, uint64_t bbishop, uint64_t bqueen, uint64_t bking, unsigned char otherinfos){
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
        target >>= 1;
    }
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
        target >>= 1;
    }
}


char getNthBNum(uint64_t x, int n){//getNthBNum(001, 0) = 1, getNthBNum(001, 1) = 0, getNthBNum(001, 2) = 0
    uint64_t target = 1<<n;
    if (x & target){
        return 1;
    }
    return 0;
}


int getEval(uint64_t wpawn, uint64_t wrook, uint64_t wknight, uint64_t wbishop, uint64_t wqueen, uint64_t wking, uint64_t bpawn, uint64_t brook, uint64_t bknight, uint64_t bbishop, uint64_t bqueen, uint64_t bking, unsigned char otherinfos){
    uint64_t target = 9223372036854775808;
    int count = 0;

    for (int i = 0; i < 64; i++){
        if (target & wpawn){
            count += 1;
        }
        else if (target & wrook){
            count += 5;
        }
        else if (target & wknight){
            count += 3;
        }
        else if (target & wbishop){
            count += 3;
        }
        else if (target & wqueen){
            count += 9;
        }
        else if (target & wking){
            count += 1000;
        }
        else if (target & bpawn){
            count -= 1;
        }
        else if (target & brook){
            count -= 5;
        }
        else if (target & bknight){
            count -= 3;
        }
        else if (target & bbishop){
            count -= 3;
        }
        else if (target & bqueen){
            count -= 9;
        }
        else if (target & bking){
            count -= 1000;
        }

        target >>= 1;
    }
    return count;
}


float minmax(uint64_t wpawn, uint64_t wrook, uint64_t wknight, uint64_t wbishop, uint64_t wqueen, uint64_t wking, uint64_t bpawn, uint64_t brook, uint64_t bknight, uint64_t bbishop, uint64_t bqueen, uint64_t bking, unsigned long pawnmv, unsigned char otherinfos, int depth){
    if (depth == 0){
        return getEval(wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, otherinfos);
    }


    if (otherinfos & 1){
        //white to move
        unsigned long Npawnmoves = pawnmoves; /////////////////////////////////////////////////////////////////////////////////////////////change name like above
        unsigned char Notherinfos = otherinfos;
        uint64_t Nwpawn = wpawn;
        uint64_t Nwqueen = wqueen;
        unsigned uint64_t mask = 34292891647;//reset en passant for white
        pawnmoves &= mask;
        
        int score = -1000;
        
        //pawnmoves
        uint64_t target = 9223372036854775808;
        for (int i = 0; i < 48; i++){
            target >>= 1;
            if ((target & wpawn) && (i > 8)){
                //move 1
                mask = 1<<(i%8);
                Npawnmoves &= ~mask; //removes the ability to double move
                
                mask = target + target<<8;
               	Nwpawn ^= target; //moves the pawn one rank
               	if (!(Nwpawn & wrook & wknight & wbishop & wqueen & wking)){
               		if (i < 16){
                		Nwpawn ^= target<<8;
                		Nwqueen ^= target<<8;
                	}
                	
                	int x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, otherinfos ^ 3, depth - 1);
                	if (x > score){
		            score = x;
		        }
               	}
		 
		        
                //move 2
                mask = 1<<(i%8);
                if (pawnmoves & mask){
		        Nwpawn = wpawn;
		        Npawnmoves = pawnmoves;

		        x = minmax(Nwpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, pawnmv, otherinfos ^ 3, depth - 1);
		        if (x > score){
		            score = x;
		        }
                }
                
                
                //eat right
                Nwpawn = wpawn;

                x = minmax(Nwpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, pawnmv, otherinfos ^ 3, depth - 1);
                if (x > score){
                    score = x;
                }
                
                
                //eat left
                Nwpawn = wpawn;

                x = minmax(Nwpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, pawnmv, otherinfos ^ 3, depth - 1);
                if (x > score){
                    score = x;
                }
                
                
                //eat right en passant 
                Nwpawn = wpawn;

                x = minmax(Nwpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, pawnmv, otherinfos ^ 3, depth - 1);
                if (x > score){
                    score = x;
                }
                
                
                //eat left en passant
                Nwpawn = wpawn;

                x = minmax(Nwpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, pawnmv, otherinfos ^ 3, depth - 1);
                if (x > score){
                    score = x;
                }
            }
        }
    }

    else{
        //black to move
    }

    //generate mv
    minmax(wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, pawnmv, otherinfos ^ 3, depth - 1);//^ 3 -> change the player color
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

    unsigned long pawnmv = 65535;//first 16 bits are for if it can be taken with en-passant (8b 8w)\\ last 16 bits for if the first move is done (8b 8w)
    unsigned char otherinfos = 241;//bleft bright wleft wright 0 0 \\ 1 bturn \\ 1 wturn


    //minmax(wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, pawnmv, otherinfos, 1);
    printBoard(wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, otherinfos);
    


    printf("%d", getEval(wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, otherinfos));
    //printToB(wpawn);
    return 0;
}
