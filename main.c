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
    printf("\n\n================\n");
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
        //=========================================================[moves +]====================================================
        uint64_t target = 9223372036854775808;
        for (int i = 0; i < 64; i++){
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

                //move the piece
                //-------------------------------------------------[move up]------------------------------------------------
                char dontExit = 1;
                mask = target;
                mask <<= 8;
                
                while (mask != 0 & dontExit){
                    if (mask & (Nwpawn | Nwrook | Nwknight | Nwbishop | Nwqueen | Nwking)){
                        dontExit = 0;
                    }
                    else {
                        //remove the main piece
                        *piece ^= target;
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
                                if (mask & 9223372036854775808){
                                    Notherinfos &= 127;//removes casling black left
                                }
                            }
                        }
                        //evaluate
                        printBoard(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Notherinfos);
                        x = minmax(Nwpawn, Nwrook, Nwknight, Nwbishop, Nwqueen, Nwking, Nbpawn, Nbrook, Nbknight, Nbbishop, Nbqueen, Nbking, Npawnmv, Notherinfos ^ 3, depth - 1);
                        if (x > score){
                            score = x;
                        }
                        //remove piece
                        *piece ^= mask;
                        mask <<= 8;
                    }
                    
                    Nwrook = wrook;
                    Nwqueen = wqueen;

                    Nbpawn = bpawn;
                    Nbrook = brook;
                    Nbknight = bknight;
                    Nbbishop = bbishop;
                    Nbqueen = bqueen;
                    Nbking = bking;

                    Notherinfos = otherinfos;
                }
            }
            target >>= 1;
        }
        
        //=========================================================[pawnmv]=====================================================
        target = 9223372036854775808;
        for (int i = 0; i < 56; i++){
            if ((i > 8) && (target & wpawn)){//if the target is a pawn (cant have pawns in the first row they promoted)

                //-------------------------------------------------[move 1]-------------------------------------------------

                //removes the ability to double move
                mask = 1<<(i%8);
                Npawnmv &= ~mask;
                
                mask = target;
                mask += target<<8;
               	Nwpawn ^= mask; //moves the pawn one rank
               	if (!(Nwpawn & Nwrook & Nwknight & Nwbishop & Nwqueen & Nwking & Nbpawn & Nbrook & Nbknight & Nbbishop & Nbqueen & Nbking)){//if the pawn can move in that direction
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
                if (pawnmv & mask){
                	Npawnmv = pawnmv ^ mask;//remove the double move
                	Npawnmv = Npawnmv ^ mask<<16;//makes so it can be taken en-passant
                	
                	mask = target;
                	mask += target<<8;
                	Nwpawn ^= mask; //moves the pawn one rank
                	if (!(Nwpawn & Nwrook & Nwknight & Nwbishop & Nwqueen & Nwking & Nbpawn & Nbrook & Nbknight & Nbbishop & Nbqueen & Nbking)){
		            	mask <<= 8;
		            	Nwpawn ^= mask; //moves the pawn one other rank
		            	if (!(Nwpawn & Nwrook & Nwknight & Nwbishop & Nwqueen & Nwking & Nbpawn & Nbrook & Nbknight & Nbbishop & Nbqueen & Nbking)){
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
                    if (pawnmv & 2147483648>>((i%8)+1)){//if you can eat en passant 
                        printf("en passant !!!!!!!!!!!!!!!!!!!!!!!!!!! \n");
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
                        if (mask & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)){//check if there is a piece to eat
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
                    if (pawnmv & 8388608>>((i%8)-1)){//if you can eat en passant 
                        printf("en passant !!!!!!!!!!!!!!!!!!!!!!!!!!! \n");
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
                        if (mask & (Nbpawn | Nbrook | Nbknight | Nbbishop | Nbqueen | Nbking)){//check if there is a piece to eat
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

    //tests //////////////////////////////////////////////////////////
    uint64_t mask = 1<<8;
    wpawn ^= mask;
    wpawn ^= mask<<1;

    mask <<= 16;
    wpawn ^= mask;
    wpawn ^= mask<<1;

    mask <<= 9;
    bpawn ^= mask;
    bpawn ^= mask>>1;

    mask <<= 16;
    bpawn ^= mask;
    bpawn ^= mask>>1;

    //

    mask = 1<<14;
    wpawn ^= mask;
    wpawn ^= mask<<1;

    mask <<= 16;
    wpawn ^= mask;
    wpawn ^= mask<<1;

    mask <<= 9;
    bpawn ^= mask;
    bpawn ^= mask>>1;

    mask <<= 16;
    bpawn ^= mask;
    bpawn ^= mask>>1;
    //can delete ////////////////////////////////////////////////////////////

    minmax(wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, pawnmv, otherinfos, 1);
    printBoard(wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, otherinfos);
    


    //printf("%d", getEval(wpawn, wrook, wknight, wbishop, wqueen, wking, bpawn, brook, bknight, bbishop, bqueen, bking, otherinfos));
    //printToB(wpawn);
    return 0;
}
