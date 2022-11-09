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
    uint64_t target = 9223372036854775808LLU;
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
    printf("\n\n================\n");
}


void printToB(uint64_t x){
    uint64_t target = 9223372036854775808LLU;
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
    uint64_t target = 9223372036854775808LLU;
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
        printBoard(wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, otherinfos);
        return getEval(wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, otherinfos);
    }
    
    //////////////////////////printBoard(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Notherinfos);
	
	//set all New pieces
	unsigned long Npawnmv = pawnmv;
    unsigned char Notherinfos = otherinfos;
    uint64_t Nwpawn = wpawn;
    uint64_t Nwrook = wrook;
    uint64_t Nwknight = wknight;
    uint64_t Nwbishop = wbishop;
    uint64_t Nwqueen = wqueen;
    uint64_t Nwking = wking;

    uint64_t Nbpawn = bpawn;
    uint64_t Nbrook = brook;
    uint64_t Nbknight = bknight;
    uint64_t Nbbishop = bbishop;
    uint64_t Nbqueen = bqueen;
    uint64_t Nbking = bking;

    if (otherinfos & 1){
        //#########################################################[white to move]##################################################

        uint64_t mask = 34292891647; //reset en passant for white
        pawnmv &= mask;
        
        int score = -1000;
        int x = -1000;

        uint64_t *piece;
        uint64_t target = 9223372036854775808LLU;
        for (int i = 0; i < 64; i++){
            //=========================================================[moves +]====================================================
            if (target & (wqueen | wrook)){
                //assign pointer to the piece we move
                if (target & wrook){
                    if (i == 63){
                        //removes casling white right
                        Notherinfos &= 239;
                    }
                    else if (i == 55){
                        //removes casling white left
                        Notherinfos &= 223;
                    }
                    piece = &Nwrook;
                }
                else {
                    piece = &Nwqueen;
                }
                //remove the main piece
                *piece ^= target;

                //move the piece
                //-------------------------------------------------[move up]------------------------------------------------
                char dontExit = 1;
                mask = target << 8;
                
                while (dontExit & mask != 0){
                    if (mask & (Nwpawn | Nwrook | Nwknight | Nwbishop | Nwqueen | Nwking)){
                        dontExit = 0;
                    }
                    else {
                        //place piece
                        *piece ^= mask;
                        //if piece eated somme
                        if (*piece & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)){
                            dontExit = 0;
                            mask = ~mask;
                            Nbpawn &= mask;
                            Nbrook &= mask;
                            Nbknight &= mask;
                            Nbbishop &= mask;
                            Nbqueen &= mask;
                            Nbking &= mask;
                            mask = ~mask;
                            {    
                                //remove the casling if he eats the corner
                                if (mask & 72057594037927936){
                                    Notherinfos &= 191;//removes casling black right
                                }
                                if (mask & 9223372036854775808LLU){
                                    Notherinfos &= 127;//removes casling black left
                                }
                            }
                        }
                        //evaluate
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }
                        //remove piece
                        *piece ^= mask;
                        mask <<= 8;
                    }

                    Nbpawn = bpawn;
                    Nbrook = brook;
                    Nbknight = bknight;
                    Nbbishop = bbishop;
                    Nbqueen = bqueen;
                    Nbking = bking;

                    Notherinfos = otherinfos;
                }

                //-------------------------------------------------[move down]------------------------------------------------
                dontExit = 1;
                mask = target >> 8;
                
                while (dontExit & mask != 0){
                    if (mask & (Nwpawn | Nwrook | Nwknight | Nwbishop | Nwqueen | Nwking)){
                        dontExit = 0;
                    }
                    else {
                        //place piece
                        *piece ^= mask;
                        //if piece eated somme
                        if (*piece & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)){
                            dontExit = 0;
                            mask = ~mask;
                            Nbpawn &= mask;
                            Nbrook &= mask;
                            Nbknight &= mask;
                            Nbbishop &= mask;
                            Nbqueen &= mask;
                            Nbking &= mask;
                            mask = ~mask;
                            {    
                                //remove the casling if he eats the corner
                                if (mask & 72057594037927936){
                                    Notherinfos &= 191;//removes casling black right
                                }
                                if (mask & 9223372036854775808LLU){
                                    Notherinfos &= 127;//removes casling black left
                                }
                            }
                        }
                        //evaluate
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }
                        //remove piece
                        *piece ^= mask;
                        mask >>= 8;
                    }

                    Nbpawn = bpawn;
                    Nbrook = brook;
                    Nbknight = bknight;
                    Nbbishop = bbishop;
                    Nbqueen = bqueen;
                    Nbking = bking;

                    Notherinfos = otherinfos;
                }

                //-------------------------------------------------[move right]------------------------------------------------
                dontExit = 1;
                mask = target >> 1;
                
                while (dontExit & !(mask & 9259542123273814144LLU) & mask != 0){//9259542123273814144 is a wall of 1 on the left
                    if (mask & (Nwpawn | Nwrook | Nwknight | Nwbishop | Nwqueen | Nwking)){
                        dontExit = 0;
                    }
                    else {
                        //place piece
                        *piece ^= mask;
                        //if piece eated somme
                        if (*piece & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)){
                            dontExit = 0;
                            mask = ~mask;
                            Nbpawn &= mask;
                            Nbrook &= mask;
                            Nbknight &= mask;
                            Nbbishop &= mask;
                            Nbqueen &= mask;
                            Nbking &= mask;
                            mask = ~mask;
                            {    
                                //remove the casling if he eats the corner
                                if (mask & 72057594037927936){
                                    Notherinfos &= 191;//removes casling black right
                                }
                                if (mask & 9223372036854775808LLU){
                                    Notherinfos &= 127;//removes casling black left
                                }
                            }
                        }
                        //evaluate
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }
                        //remove piece
                        *piece ^= mask;
                        mask >>= 1;
                    }

                    Nbpawn = bpawn;
                    Nbrook = brook;
                    Nbknight = bknight;
                    Nbbishop = bbishop;
                    Nbqueen = bqueen;
                    Nbking = bking;

                    Notherinfos = otherinfos;
                }

                //-------------------------------------------------[move left]------------------------------------------------
                dontExit = 1;
                mask = target << 1;
                
                while (dontExit & !(mask & 72340172838076673) & mask != 0){//72340172838076673 is a wall of 1 on the right
                    if (mask & (Nwpawn | Nwrook | Nwknight | Nwbishop | Nwqueen | Nwking)){
                        dontExit = 0;
                    }
                    else {
                        //place piece
                        *piece ^= mask;
                        //if piece eated somme
                        if (*piece & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)){
                            dontExit = 0;
                            mask = ~mask;
                            Nbpawn &= mask;
                            Nbrook &= mask;
                            Nbknight &= mask;
                            Nbbishop &= mask;
                            Nbqueen &= mask;
                            Nbking &= mask;
                            mask = ~mask;
                            {    
                                //remove the casling if he eats the corner
                                if (mask & 72057594037927936){
                                    Notherinfos &= 191;//removes casling black right
                                }
                                if (mask & 9223372036854775808LLU){
                                    Notherinfos &= 127;//removes casling black left
                                }
                            }
                        }
                        //evaluate
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }
                        //remove piece
                        *piece ^= mask;
                        mask <<= 1;
                    }

                    Nbpawn = bpawn;
                    Nbrook = brook;
                    Nbknight = bknight;
                    Nbbishop = bbishop;
                    Nbqueen = bqueen;
                    Nbking = bking;

                    Notherinfos = otherinfos;
                }
                *piece ^= target;//place the piece back
            }

            
            //=========================================================[moves x]====================================================
            if (target & (wqueen | wbishop)){
                //assign pointer to the piece we move
                if (target & wbishop){
                    piece = &Nwbishop;
                }
                else {
                    piece = &Nwqueen;
                }
                //remove the main piece
                *piece ^= target;

                //move the piece
                //-------------------------------------------------[move left up]------------------------------------------------
                char dontExit = 1;
                mask = target << 9;
                
                while (dontExit && mask != 0 && !(mask & 72340172838076673)){//72340172838076673 is a wall of 1 on the right
                    if (mask & (Nwpawn | Nwrook | Nwknight | Nwbishop | Nwqueen | Nwking)){
                        dontExit = 0;
                    }
                    else {
                        //place piece
                        *piece ^= mask;
                        //if piece eated somme
                        if (*piece & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)){
                            dontExit = 0;
                            mask = ~mask;
                            Nbpawn &= mask;
                            Nbrook &= mask;
                            Nbknight &= mask;
                            Nbbishop &= mask;
                            Nbqueen &= mask;
                            Nbking &= mask;
                            mask = ~mask;
                            {    
                                //remove the casling if he eats the corner
                                if (mask & 72057594037927936){
                                    Notherinfos &= 191;//removes casling black right
                                }
                                if (mask & 9223372036854775808LLU){
                                    Notherinfos &= 127;//removes casling black left
                                }
                            }
                        }
                        //evaluate
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }
                        //remove piece
                        *piece ^= mask;
                        mask <<= 9;
                    }

                    Nbpawn = bpawn;
                    Nbrook = brook;
                    Nbknight = bknight;
                    Nbbishop = bbishop;
                    Nbqueen = bqueen;
                    Nbking = bking;

                    Notherinfos = otherinfos;
                }

                //-------------------------------------------------[move left down]------------------------------------------------
                dontExit = 1;
                mask = target >> 7;
                
                while (dontExit & mask != 0 & !(mask & 72340172838076673)){//72340172838076673 is a wall of 1 on the right
                    if (mask & (Nwpawn | Nwrook | Nwknight | Nwbishop | Nwqueen | Nwking)){
                        dontExit = 0;
                    }
                    else {
                        //place piece
                        *piece ^= mask;
                        //if piece eated somme
                        if (*piece & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)){
                            dontExit = 0;
                            mask = ~mask;
                            Nbpawn &= mask;
                            Nbrook &= mask;
                            Nbknight &= mask;
                            Nbbishop &= mask;
                            Nbqueen &= mask;
                            Nbking &= mask;
                            mask = ~mask;
                            {    
                                //remove the casling if he eats the corner
                                if (mask & 72057594037927936){
                                    Notherinfos &= 191;//removes casling black right
                                }
                                if (mask & 9223372036854775808LLU){
                                    Notherinfos &= 127;//removes casling black left
                                }
                            }
                        }
                        //evaluate
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }
                        //remove piece
                        *piece ^= mask;
                        mask >>= 7;
                    }

                    Nbpawn = bpawn;
                    Nbrook = brook;
                    Nbknight = bknight;
                    Nbbishop = bbishop;
                    Nbqueen = bqueen;
                    Nbking = bking;

                    Notherinfos = otherinfos;
                }

                //-------------------------------------------------[move right up]------------------------------------------------
                dontExit = 1;
                mask = target << 7;
                
                while (dontExit & mask != 0 & !(mask & 9259542123273814144LLU)){//9259542123273814144 is a wall of 1 on the left
                    if (mask & (Nwpawn | Nwrook | Nwknight | Nwbishop | Nwqueen | Nwking)){
                        dontExit = 0;
                    }
                    else {
                        //place piece
                        *piece ^= mask;
                        //if piece eated somme
                        if (*piece & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)){
                            dontExit = 0;
                            mask = ~mask;
                            Nbpawn &= mask;
                            Nbrook &= mask;
                            Nbknight &= mask;
                            Nbbishop &= mask;
                            Nbqueen &= mask;
                            Nbking &= mask;
                            mask = ~mask;
                            {    
                                //remove the casling if he eats the corner
                                if (mask & 72057594037927936){
                                    Notherinfos &= 191;//removes casling black right
                                }
                                if (mask & 9223372036854775808LLU){
                                    Notherinfos &= 127;//removes casling black left
                                }
                            }
                        }
                        //evaluate
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }
                        //remove piece
                        *piece ^= mask;
                        mask <<= 7;
                    }

                    Nbpawn = bpawn;
                    Nbrook = brook;
                    Nbknight = bknight;
                    Nbbishop = bbishop;
                    Nbqueen = bqueen;
                    Nbking = bking;

                    Notherinfos = otherinfos;
                }

                //-------------------------------------------------[move right down]------------------------------------------------
                dontExit = 1;
                mask = target >> 9;
                
                while (dontExit && mask != 0 && (mask & 9259542123273814144LLU) == 0){//9259542123273814144 is a wall of 1 on the left
                    if ((mask & (Nwpawn | Nwrook | Nwknight | Nwbishop | Nwqueen | Nwking)) != 0){//colide with piece
                        dontExit = 0;
                    }
                    else {
                        //place piece
                        *piece ^= mask;
                        //if piece eated somme
                        if (*piece & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)){
                            dontExit = 0;
                            mask = ~mask;
                            Nbpawn &= mask;
                            Nbrook &= mask;
                            Nbknight &= mask;
                            Nbbishop &= mask;
                            Nbqueen &= mask;
                            Nbking &= mask;
                            mask = ~mask;
                            {    
                                //remove the casling if he eats the corner
                                if (mask & 72057594037927936){
                                    Notherinfos &= 191;//removes casling black right
                                }
                                if (mask & 9223372036854775808LLU){
                                    Notherinfos &= 127;//removes casling black left
                                }
                            }
                        }
                        //evaluate
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }
                        //remove piece
                        *piece ^= mask;
                        mask >>= 9;
                    }

                    Nbpawn = bpawn;
                    Nbrook = brook;
                    Nbknight = bknight;
                    Nbbishop = bbishop;
                    Nbqueen = bqueen;
                    Nbking = bking;

                    Notherinfos = otherinfos;
                }
                //place the piece back
                *piece ^= target;
            }


            //=========================================================[knightmv]===================================================
            if (target & wknight){
                Nwknight ^= target;//remove the knight
                //---------------------------------------------------------[up and down]----------------------------------------
                mask = target<<17;
                for (int __ = 0; __ < 2; __++){
                    
                    if (mask != 0 && (mask & 72340172838076673) == 0 && (mask & (Nwpawn | Nwrook | Nwknight | Nwbishop | Nwqueen | Nwking)) == 0){//72340172838076673 is a wall of 1 on the right
                        //place knight
                        Nwknight ^= mask;

                        if (Nwknight & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)){//if you eat somme
                            mask = ~mask;
                            Nbpawn &= mask;
                            Nbrook &= mask;
                            Nbknight &= mask;
                            Nbbishop &= mask;
                            Nbqueen &= mask;
                            Nbking &= mask;
                            mask = ~mask;

                            {    
                                //remove the casling if he eats the corner
                                if (mask & 9223372036854775808LLU){
                                    Notherinfos &= 127;//removes casling black left
                                }
                            }
                        }
                        
                        //evaluate
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }
                        
                        //removes the knight
                        Nwknight ^= mask;

                        Nbpawn = bpawn;
                        Nbrook = brook;
                        Nbknight = bknight;
                        Nbbishop = bbishop;
                        Nbqueen = bqueen;
                        Nbking = bking;

                        Notherinfos = otherinfos;
                    }

                    mask>>=2;
                    
                    if (mask != 0 && (mask & 9259542123273814144LLU) == 0 && (mask & (Nwpawn | Nwrook | Nwknight | Nwbishop | Nwqueen | Nwking)) == 0){//9259542123273814144 is a wall of 1 on the left
                        //place knight
                        Nwknight ^= mask;

                        if (Nwknight & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)){//if you eat somme
                            mask = ~mask;
                            Nbpawn &= mask;
                            Nbrook &= mask;
                            Nbknight &= mask;
                            Nbbishop &= mask;
                            Nbqueen &= mask;
                            Nbking &= mask;
                            mask = ~mask;

                            {    
                                //remove the casling if he eats the corner
                                if (mask & 72057594037927936){
                                    Notherinfos &= 191;//removes casling black right
                                }
                            }
                        }
                        
                        //evaluate
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }
                        
                        //removes the knight
                        Nwknight ^= mask;
                        Nbpawn = bpawn;
                        Nbrook = brook;
                        Nbknight = bknight;
                        Nbbishop = bbishop;
                        Nbqueen = bqueen;
                        Nbking = bking;

                        Notherinfos = otherinfos;
                    }
                    

                    mask = target>>15;
                }
                
                //---------------------------------------------------------[left and right]-------------------------------------

                mask = target<<10;
                for (int __ = 0; __ < 2; __++){
                    if (mask != 0 && (mask & 217020518514230019) == 0 && (mask & (Nwpawn | Nwrook | Nwknight | Nwbishop | Nwqueen | Nwking)) == 0){//217020518514230019 is a wall of 2 1 on the right
                        //place knight
                        Nwknight ^= mask;

                        if (Nwknight & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)){//if you eat somme
                            mask = ~mask;
                            Nbpawn &= mask;
                            Nbrook &= mask;
                            Nbknight &= mask;
                            Nbbishop &= mask;
                            Nbqueen &= mask;
                            Nbking &= mask;
                            mask = ~mask;

                            {    
                                //remove the casling if he eats the corner
                                if (mask & 72057594037927936){
                                    Notherinfos &= 191;//removes casling black right
                                }
                            }
                        }
                        
                        //evaluate
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }
                        
                        //removes the knight
                        Nwknight ^= mask;

                        Nbpawn = bpawn;
                        Nbrook = brook;
                        Nbknight = bknight;
                        Nbbishop = bbishop;
                        Nbqueen = bqueen;
                        Nbking = bking;

                        Notherinfos = otherinfos;
                    }

                    mask>>=16;
                }

                mask = target<<6;
                for (int __ = 0; __ < 2; __++){
                    if (mask != 0 && (mask & 13889313184910721216LLU) == 0 && (mask & (Nwpawn | Nwrook | Nwknight | Nwbishop | Nwqueen | Nwking)) == 0){//13889313184910721216 is a wall of 2 1 on the left
                        //place knight
                        Nwknight ^= mask;

                        if (Nwknight & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)){//if you eat somme
                            mask = ~mask;
                            Nbpawn &= mask;
                            Nbrook &= mask;
                            Nbknight &= mask;
                            Nbbishop &= mask;
                            Nbqueen &= mask;
                            Nbking &= mask;
                            mask = ~mask;

                            {    
                                //remove the casling if he eats the corner
                                if (mask & 9223372036854775808LLU){
                                    Notherinfos &= 127;//removes casling black left
                                }
                            }
                        }
                        
                        //evaluate
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }
                        
                        //removes the knight
                        Nwknight ^= mask;

                        Nbpawn = bpawn;
                        Nbrook = brook;
                        Nbknight = bknight;
                        Nbbishop = bbishop;
                        Nbqueen = bqueen;
                        Nbking = bking;

                        Notherinfos = otherinfos;
                    }

                    mask>>=16;
                }

                Nwknight ^= target;//place the knight back
            }


            //=========================================================[kingmv]=====================================================//king left and right dont colide
            if (target & wking){
                Nwking ^= target;//remove the wking
                mask = target << 9;
                for (int _ = 0; _ < 2; _++){//line up and down
                    for (int __ = 0; __ < 3; __++){//3 cases over and under
                        if ((mask & (Nwpawn | Nwrook | Nwknight | Nwbishop | Nwqueen)) == 0 && mask != 0){//no white pieces there
                            //place the king
                            Nwking ^= mask;
                            
                            if ((mask & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking) != 0)){//if you eat piece
                                mask = ~mask;
                                Nbpawn &= mask;
                                Nbrook &= mask;
                                Nbknight &= mask;
                                Nbbishop &= mask;
                                Nbqueen &= mask;
                                Nbking &= mask;
                                mask = ~mask;
                                {    
                                    //remove the casling if he eats the corner
                                    if (mask & 72057594037927936){
                                        Notherinfos &= 191;//removes casling black right
                                    }
                                    if (mask & 9223372036854775808LLU){
                                        Notherinfos &= 127;//removes casling black left
                                    }
                                }
                                //evaluate
                                x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                                if (x > score){
                                    score = x;
                                }

                                //reset if eaten
                                Nbpawn = bpawn;
                                Nbrook = brook;
                                Nbknight = bknight;
                                Nbbishop = bbishop;
                                Nbqueen = bqueen;
                            }
                            else{
                                //evaluate
                                x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                                if (x > score){
                                    score = x;
                                }
                            }

                            //removes the king
                            Nwking ^= mask;
                        }
                    }
                    mask >>= 14;
                }

                //wking move right and left (no need to check if you break casling bc you wouldnt be able to be here)
                mask = target << 1;
                if ((mask & (Nwpawn | Nwrook | Nwknight | Nwbishop | Nwqueen)) == 0 && mask != 0 && (i%8 != 0)){//no white pieces there
                    //place the king
                    Nwking ^= mask;

                    if ((mask & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)) != 0){//if you eat piece
                        mask = ~mask;
                        Nbpawn &= mask;
                        Nbrook &= mask;
                        Nbknight &= mask;
                        Nbbishop &= mask;
                        Nbqueen &= mask;
                        Nbking &= mask;
                        mask = ~mask;

                        //evaluate
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }

                        //reset if eaten
                        Nbpawn = bpawn;
                        Nbrook = brook;
                        Nbknight = bknight;
                        Nbbishop = bbishop;
                        Nbqueen = bqueen;
                    }
                    else {
                        //evaluate
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }
                    }
                    //remove the king
                    Nwking ^= mask;
                }

                mask = target >> 1;
                if ((mask & (Nwpawn | Nwrook | Nwknight | Nwbishop | Nwqueen)) == 0 && mask != 0 && (i%8 != 7)){//no white pieces there
                    //place the king
                    Nwking ^= mask;

                    if ((mask & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking) != 0)){//if you eat piece
                        mask = ~mask;
                        Nbpawn &= mask;
                        Nbrook &= mask;
                        Nbknight &= mask;
                        Nbbishop &= mask;
                        Nbqueen &= mask;
                        Nbking &= mask;
                        mask = ~mask;

                        //evaluate
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }

                        //reset if eaten
                        Nbpawn = bpawn;
                        Nbrook = brook;
                        Nbknight = bknight;
                        Nbbishop = bbishop;
                        Nbqueen = bqueen;
                    }
                    else {
                        //evaluate
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }
                    }
                    //remove the king
                    Nwking ^= mask;
                }
                Nwking ^= target;//place the king back
            }

            target >>= 1;
        }
        
        //=========================================================[pawnmv]=====================================================
        target = 9223372036854775808LLU;
        for (int i = 0; i < 56; i++){
            if ((i > 8) && (target & wpawn)){//if the target is a pawn (cant have pawns in the first row they promoted)

                //-------------------------------------------------[move 1]-------------------------------------------------

                //removes the ability to double move
                mask = 1<<(i%8);
                Npawnmv &= ~mask;
                
                mask = target;
                mask += target<<8;
               	Nwpawn ^= mask; //moves the pawn one rank
               	if ((Nwpawn & (Nwrook | Nwknight | Nwbishop | Nwqueen | Nwking | Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)) == 0){//if the pawn can move in that direction
               		if (i < 16){
                		Nwpawn ^= target<<8;
                		Nwqueen ^= target<<8;
                	}
                	x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                	if (x > score){
		            	score = x;
		        	}
		        	Nwqueen = wqueen;
               	}
               	Nwpawn = wpawn;
		 
		        
                //-------------------------------------------------[move 2]-------------------------------------------------
                mask = 1<<(i%8);
                if ((pawnmv & mask) != 0){
                	Npawnmv = pawnmv ^ mask;//remove the double move
                	Npawnmv = Npawnmv ^ mask<<16;//makes so it can be taken en-passant
                	
                	mask = target;
                	mask += target<<8;
                	Nwpawn ^= mask; //moves the pawn one rank
                	if ((Nwpawn & (Nwrook | Nwknight | Nwbishop | Nwqueen | Nwking | Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)) == 0){
		            	mask <<= 8;
		            	Nwpawn ^= mask; //moves the pawn one other rank
		            	if ((Nwpawn & (Nwrook | Nwknight | Nwbishop | Nwqueen | Nwking | Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)) == 0){
		            		x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
					    	if (x > score){
					            score = x;
					        }
		            	}
                	}
                }
                Npawnmv = pawnmv;
                Nwpawn = wpawn;

                //-------------------------------------------------[eat right]-------------------------------------------------
                if (i%8 < 7){
                    if ((pawnmv & 2147483648>>(i%8)) != 0){//if you can eat en passant 
                        printf("en passant right !!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                        printBoard(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Notherinfos);
                        mask = target>>1;
                        
                        //remove the piece that got eated
                        Nbpawn &= ~mask;
                        
                        //move the pawn
                        mask <<= 8;
                        mask += target;
                        Nwpawn ^= mask;
                        
                                                    
                        printBoard(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Notherinfos);
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }
                        Nwpawn = wpawn;
                        Nbpawn = bpawn;
                        Nbrook = brook;
                        Nbknight = bknight;
                        Nbbishop = bbishop;
                        Nbqueen = bqueen;
                        Nbking = bking;
                    }

                    else{// eat right normaly 
                        mask = target<<7;
                        if ((mask & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)) != 0){//check if there is a piece to eat
                            //remove the piece that got eated
                            mask = ~mask;
                            Nbpawn &= mask;
                            Nbrook &= mask;
                            Nbknight &= mask;
                            Nbbishop &= mask;
                            Nbqueen &= mask;
                            Nbking &= mask;
                            mask = ~mask;
                            
                            if (i < 8){//if you promote
                                if (i == 7){
                                    Notherinfos &= 191;//removes casling black right
                                }
                                Nwqueen ^= mask;
                                Nwpawn ^= target;
                            }
                            else{//move the pawn
                                mask += target;
                                Nwpawn ^= mask;
                            }

                            //removes the ability to double move
                            mask = 1<<(i%8);
                            Npawnmv &= ~mask; 
                                                        
                            x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                            if (x > score){
                                score = x;
                            }
                            Nwpawn = wpawn;
                            Nbpawn = bpawn;
                            Nbrook = brook;
                            Nbknight = bknight;
                            Nbbishop = bbishop;
                            Nbqueen = bqueen;
                            Nbking = bking;
                            Npawnmv = pawnmv;
                            Notherinfos = otherinfos;
                        }
                    }
                }
                
                
                //-------------------------------------------------[eat left]-------------------------------------------------
                if (i%8){
                    if ((pawnmv & 8388608>>(i%8)) != 0){//if you can eat en passant 
                        printf("en passant left !!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                        printBoard(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Notherinfos);
                        mask = target<<1;
                        
                        //remove the piece that got eated
                        Nbpawn &= ~mask;
                        
                        //move the pawn
                        mask <<= 8;
                        mask += target;
                        Nwpawn ^= mask;
                        
                                                    
                        printBoard(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Notherinfos);
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }
                        Nwpawn = wpawn;
                        Nbpawn = bpawn;
                        Nbrook = brook;
                        Nbknight = bknight;
                        Nbbishop = bbishop;
                        Nbqueen = bqueen;
                        Nbking = bking;
                    }

                    else{// eat left normaly 
                        mask = target<<9;
                        if ((mask & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)) != 0){//check if there is a piece to eat
                            //remove the piece that got eated
                            mask = ~mask;
                            Nbpawn &= mask;
                            Nbrook &= mask;
                            Nbknight &= mask;
                            Nbbishop &= mask;
                            Nbqueen &= mask;
                            Nbking &= mask;
                            mask = ~mask;
                            
                            if (i < 8){//if you promote
                                if (i == 0){
                                    Notherinfos &= 127;//removes casling black left
                                }
                                Nwqueen ^= mask;
                                Nwpawn ^= target;
                            }
                            else{//move the pawn
                                mask += target;
                                Nwpawn ^= mask;
                            }

                            //removes the ability to double move
                            mask = 1<<(i%8);
                            Npawnmv &= ~mask;
                            
                            x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                            if (x > score){
                                score = x;
                            }
                            Nwpawn = wpawn;
                            Nbpawn = bpawn;
                            Nbrook = brook;
                            Nbknight = bknight;
                            Nbbishop = bbishop;
                            Nbqueen = bqueen;
                            Nbking = bking;
                            Npawnmv = pawnmv;
                            Notherinfos = otherinfos;
                        }
                    }
                }
            }
            target >>= 1;
        }
        //othermoves 
    }

    else{
        //black to move
    }

    //generate mv
    //minmax(wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, pawnmv, otherinfos ^ 3, depth - 1);//^ 3 -> change the player color
}


int main(){
    uint64_t wpawn = 65280;
    uint64_t wrook = 129;
    uint64_t wknight = 66;
    uint64_t wbishop = 36;
    uint64_t wqueen = 16;
    uint64_t wking = 8;

    uint64_t bpawn = 71776119061217280LLU;
    uint64_t brook = 9295429630892703744LLU;
    uint64_t bknight = 4755801206503243776LLU;
    uint64_t bbishop = 2594073385365405696LLU;
    uint64_t bqueen = 1152921504606846976LLU;
    uint64_t bking = 576460752303423488LLU;

    unsigned long pawnmv = 65535;//first 16 bits are for if it can be taken with en-passant (8b 8w)\\ last 16 bits for if the first move is done (8b 8w)
    unsigned char otherinfos = 241;//bleft bright wleft wright 0 0 \\ 1 bturn \\ 1 wturn

    minmax(wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, pawnmv, otherinfos, 1);
    printBoard(wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, otherinfos);
    


    //printf("%d", getEval(wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, otherinfos));
    //printToB(wpawn);
    return 0;
}
