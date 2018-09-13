/*
 * execution.c
 *
 *  Created on: Jul 28, 2018
 *      Author: fanchen
 */

#include "execution.h"

void printTwoMaps(char** mapOne, char** mapTwo, GameSettings* pMySettings, GameInfo* pMyGame, GameInfo* pAIGame){
    // print two shipMaps side by side. For use during the game play
    int i = 0, j = 0;
    char* titleLeft = "YOUR MAP";
    char* titleRight = "OPPONENT'S MAP";
    char* fleetInfoTitle = "Current Fleet Situation";

    printf("\n");
    for(i = 0; i < ((int)pMySettings->col * 4 + 3 + 2) * 2; i++)
        printf("=");

    printf("\nDIFFICULTY LEVEL: ");
    if(pMySettings->AIlvl == 1)
    	printf("Easy\n");
    else if (pMySettings->AIlvl == 2)
    	printf("Medium\n");
    else
    	printf("Hard\n");

    // center the title name relative to each map
    printf("\n%*s%*s", (((int)pMySettings->col * 4 + 3) / 2 + (int)strlen(titleLeft) / 2), titleLeft, (((int)pMySettings->col * 4 + 3) + 3 - (int)strlen(titleLeft) / 2 + (int)strlen(titleRight) / 2), titleRight);

    // print current fleet info
    printf("\n    %s%*s\n", fleetInfoTitle, (int)pMySettings->col * 4 + 6, fleetInfoTitle);
    for(i = 0; i < (int)pMySettings->typesShips; i++)
        printf("     -%-16s    %d left%*c-%-16s    %d left\n", pMyGame->ships[i].name, pMyGame->ships[i].shipLeft, ((int)pMySettings->col * 4 + 5 - 16 - 10),' ', pAIGame->ships[i].name, pAIGame->ships[i].shipLeft);
    printf("\n");

    // col coordinates
    printf("   ");
    for(j = 0; j < (int)pMySettings->col; j++){
        if(j < 9)
            printf(" %d  ", (j + 1));
        else
            printf("%d  ", (j + 1));
    }
    printf("      ");
    for(j = 0; j < (int)pMySettings->col; j++){
        if(j < 9)
            printf(" %d  ", (j + 1));
        else
            printf("%d  ", (j + 1));
    }
    printf("\n");

    printf("  +");
    for(j = 0; j < (int)pMySettings->col; j++)
        printf("---+");
    printf("     +");
    for(j = 0; j < (int)pMySettings->col; j++)
        printf("---+");
    printf("\n");

    for(i = 0; i < (int)pMySettings->row; i++){
        printf("%c ", (i + 65)); // row coordinates
        for(j = 0; j < (int)pMySettings->col; j++){
            if(mapOne[i][j])
                printf("| %c ", mapOne[i][j]);
            else // when a space is not occupied, print a whitespace
                printf("|   ");
        }
        printf("|   ");
        printf("%c ", (i + 65)); // row coordinates
        for(j = 0; j < (int)pMySettings->col; j++){
            if(mapTwo[i][j])
                printf("| %c ", mapTwo[i][j]);
            else // when a space is not occupied, print a whitespace
                printf("|   ");
        }
        printf("|\n");

        printf("  +");
        for(j = 0; j < (int)pMySettings->col; j++)
            printf("---+");
        printf("     +");
        for(j = 0; j < (int)pMySettings->col; j++)
            printf("---+");
        printf("\n");

    }
}

int printResult(int returnMove, GameSettings* pMySettings, GameInfo* pMyGame, GameInfo* pAIGame, char** relayGrid, int isPlayer){
    // print result of each move. Also notify whether one side has victory
    if(isPlayer){
        if(returnMove == -2)
            printf("\nYou MISSED\n");
        else{
            printf("\nYou made a HIT!!\n");
            if(returnMove == -1)
                printf("But the ship is still afloat\n");
            else{
                printf("AI's %s has sunk.\n", pAIGame->ships[returnMove].name);
                if(pAIGame->totalShipLeft == 0){ // player won
                    printTwoMaps(pMyGame->shipMap, relayGrid, pMySettings, pMyGame, pAIGame);
                    printf("\nWONDERFUL JOB! You have sunk all AI's ships! You WON!\n");
                    return 1; // winner player
                }
            }
        }
    }
    else{
        if(returnMove == -2)
            printf("\nAI MISSED\n");
        else{
            printf("\nAI made a HIT!!\n");
            if(returnMove == -1)
                printf("But the ship is still afloat\n");
            else{
                printf("Your %s has sunk.\n", pMyGame->ships[returnMove].name);
                if(pMyGame->totalShipLeft == 0){ // AI won
                    printTwoMaps(pMyGame->shipMap, relayGrid, pMySettings, pMyGame, pAIGame);
                    printf("\nSORRY, all your ships have sunk. You lost!\n");
                    return 2; // winner AI
                }
            }
        }
    }
    return 0; // no winner yet

}

int move(GameSettings* pMySettings, GameInfo* pGame, Coordinate* pBomb, Coordinate* pPreHit, char*** pRelayGrid, int AIlvl, int returnMove, int isPlayer){
    // drop bomb on opponent's map, then decide whether it's a hit or miss
    // return -2 for miss, return -1 for ship hit but still afloat, return index of the ship for ship sunk.
    int gridRow = 0, gridCol = 0;
    int returnUGI = 0; // store return value of updateGameInfo()

    if (isPlayer){
        *pBomb = getCoordiante(pMySettings->row, pMySettings->col, "coordinate to drop BOMB");
        if(!pBomb->col) // user cancels
        	return -3;
    }
    else
        *pBomb = AIcoordinate(pMySettings, pGame, AIlvl, pPreHit, returnMove);

    gridRow = (int)pBomb->row % 32 - 1;
    gridCol = pBomb->col - 1;

    if (pGame->shipMap[gridRow][gridCol] && pGame->shipMap[gridRow][gridCol] != '/' && pGame->shipMap[gridRow][gridCol] != 'X'){ // it's a hit

        returnUGI = updateGameInfo(pMySettings, pGame, gridRow, gridCol);

        // label the hit ship part as 'X'
        pGame->shipMap[gridRow][gridCol] = 'X';
        if(isPlayer) // player's move, thus updating the relay grid
            (*pRelayGrid)[gridRow][gridCol] = 'X';
        else{
            pPreHit->row = pBomb->row;
            pPreHit->col = pBomb->col;
        }

        return returnUGI; // return index of sunk ship
    }
    else{
        // label the miss as '/'
        pGame->shipMap[gridRow][gridCol] = '/';
        if(isPlayer)
            (*pRelayGrid)[gridRow][gridCol] = '/';
        return -2; // missed
    }
}

int updateGameInfo(GameSettings* pMySettings, GameInfo* pGame, int gridRow, int gridCol){
    // after a hit, GameInfo needs to be updated. return index of sunk ship or -1 indicating ship afloat
    char label = pGame->shipMap[gridRow][gridCol];
    for (int i = 0; i < (int)pMySettings->typesShips; i++){
        if (pGame->ships[i].name[0] == label){
            pGame->ships[i].damage++;
            if(pGame->ships[i].damage == pGame->ships[i].size){
                pGame->ships[i].shipLeft--;
                pGame->ships[i].damage = 0;
                pGame->totalShipLeft--;
                return i; // return the index of the sunk ship
            }
            else
                return -1; // targeted ship still afloat
        }
    }
//    printf("%c\n", label);
    printf("updateGameInfo() encountered a fatal error. Program has shut down\n");
    exit(0);
}

int moveByMove(int choice, GameSettings* pMySettings, GameInfo* pMyGame, GameInfo* pAIGame, Coordinate* pBomb, char*** pRelayGrid){
   // simulate the actual game loop. Each player follows the other in dropping bombs until one side's ships all sink
    int AIreturnMove = -2; // record the return value of move(). initialize it to a miss
    Coordinate AIpreHit = {.row = 'z', .col = -1}; // store the current hit coordinate, used for AI to generate the next bomb coordinate

    // Used ONLY when testing AI vs. AI
    int userReturnMove = -2;
    Coordinate userPreHit = {.row = 'z', .col = -1};

    if(choice == 1){
        printTwoMaps(pMyGame->shipMap, *pRelayGrid, pMySettings, pMyGame, pAIGame); // print player's map and an empty map of AI

        // player makes a move and prints out its result
        userReturnMove = move(pMySettings, pAIGame, pBomb, &userPreHit, pRelayGrid, pMySettings->AIlvl, userReturnMove, 1);
        if(userReturnMove == -3) // player cancels
        	return 0;
        printResult(userReturnMove, pMySettings, pMyGame, pAIGame, *pRelayGrid, 1);


    }
    while(1){
        // AI makes a move and prints out its result
        AIreturnMove = move(pMySettings, pMyGame, pBomb, &AIpreHit, pRelayGrid, pMySettings->AIlvl, AIreturnMove, 0);
        if(printResult(AIreturnMove, pMySettings, pMyGame, pAIGame, *pRelayGrid, 0)) // AI wins
            return 2;

        printTwoMaps(pMyGame->shipMap, *pRelayGrid, pMySettings, pMyGame, pAIGame);

        // player move
        userReturnMove = move(pMySettings, pAIGame, pBomb, &userPreHit, pRelayGrid, pMySettings->AIlvl, userReturnMove, 1);
        if(userReturnMove == -3) // player cancels
        	return 0;
        if(printResult(userReturnMove, pMySettings, pMyGame, pAIGame, *pRelayGrid, 1)) // player wins
            return 1;
    }

}
