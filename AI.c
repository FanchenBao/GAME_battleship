/*
 * AI.c
 *
 *  Created on: Jul 28, 2018
 *      Author: fanchen
 */

#include "AI.h"

void AIcheckSurrounding(GameSettings* pMySettings, GameInfo* pMyGame, Coordinate* pPreHit, int* surroundings){
    // check the surrounding 4 spaces of the previous hit, record the findings in surroundings array
    int i = 0;
    int nextRow = 0, nextCol = 0;
    int gridRow = (int)pPreHit->row % 32 - 1;
    int gridCol = pPreHit->col - 1;

    for (i = 0; i < 4; i++){ // loop from up -> right -> down -> left
        switch(i){
            case 0: // check up a space
                nextRow = gridRow - 1;
                nextCol = gridCol;
                break;

            case 1: // check right a space
                nextRow = gridRow;
                nextCol = gridCol + 1;
                break;

            case 2: // check down a space
                nextRow = gridRow + 1;
                nextCol = gridCol;
                break;

            case 3: // check left a space
                nextRow = gridRow;
                nextCol = gridCol - 1;
                break;

            default:
                printf("AINextAttempt() encounter a fatal error. Program terminated.\n");
                exit(0);
        }

        if(checkBoarderGrid(nextRow, nextCol, pMySettings)){ // inside map
            if(pMyGame->shipMap[nextRow][nextCol] == 'X') // next is a hit
                surroundings[i] = 1;
            else if(pMyGame->shipMap[nextRow][nextCol] == '/') // next is a miss
                surroundings[i] = -1;
            else // next is available space (empty or a ship)
                surroundings[i] = 0;
        }
        else // outside map
            surroundings[i] = -1;
    }
}

Coordinate AInextCoord(int dir, Coordinate* pPre){
    // return a coordinate based on which direction the coordinate should be adjacent to a given coordinate
    Coordinate temp;

    switch (dir){
        case 0:
            temp.row = pPre->row - 1;
            temp.col = pPre->col;
            return temp;
        case 1:
            temp.row = pPre->row;
            temp.col = pPre->col + 1;
            return temp;
        case 2:
            temp.row = pPre->row + 1;
            temp.col = pPre->col;
            return temp;
        case 3:
            temp.row = pPre->row;
            temp.col = pPre->col - 1;
            return temp;
        default:
            printf("AIcoordinate() encounters a fatal error. Program terminated.\n");
            exit(0);
    }
}

Coordinate mediumAI(GameSettings* pMySettings, GameInfo* pMyGame, Coordinate* pPreHit, int returnMove){
    //  medium difficulty AI algorithm
    Coordinate temp; // store a coordinate temporarily
    int gridRow = 0, gridCol = 0;
    int i = 0, j = 0;
    int surroundings[4] = {0}; // the situation of the four spaces surrounding the previous hit space.
                        // 0 = available, 1 = hit, -1 = miss or N/A. surroundings[0] up, surroundings[1] right, surroundings[2] down, surroundings[3] left
    int emptySpaces[4] = {0}; // store the direction value (from surroundings array) sequentially
    int dir = 0; // store a direction (0-4)
    int dirIndex = 0; // store the index of emptySpaces array.

    if (pPreHit->col == -1 || returnMove >= 0){ // AI has no hit yet or a ship has just sunk
        pPreHit->col = -1; // nullify preHit to avoid confusion of a miss after a hit or a miss after a miss
        return randomCoordinate(pMySettings, pMyGame, 0);
    }

    else{ // the previous hit hasn't sunk the ship yet
        AIcheckSurrounding(pMySettings, pMyGame, pPreHit, surroundings);
        for (i = 0; i < 4; i++){ // go around surroundings() to check where the next step can be
            if (surroundings[i] == 1){ // a hit is in the surrounding
                // check the opposite space
                dir = (i + 2) % 4;
                if (surroundings[dir] == 0) // opposite is empty or a ship
                    return AInextCoord(dir, pPreHit);
                else{ // opposite is a miss (or theoretically another hit, but that is impossible)
                    // going along the original direction and place the bomb at the first empty space
                    temp = AInextCoord(i, pPreHit); // set temp to the hit coordinate in the surrounding
                    while (1){
                        gridRow = (int)temp.row % 32 - 1;
                        gridCol = temp.col - 1;
                        if (pMyGame->shipMap[gridRow][gridCol] != 'X' && pMyGame->shipMap[gridRow][gridCol] != '/') // current temp coordinate is empty or a ship
                            return temp;
                        else
                            temp = AInextCoord(i, &temp); // keep searching in the same direction until am enpty space is found
                    }
                }
            }

            if (surroundings[i] == 0){ // surrounding position is empty or a ship
                emptySpaces[j] = i; // record the direction
                j++;
            }

        }

        // no hit in the surrounding (meaning the previous hit is the first hit)
        // next move should be a random pick among the empty spaces of the surroundings
        dirIndex = rand() % j; // j is currently the size of the emptySpaces array
        return AInextCoord(emptySpaces[dirIndex], pPreHit);
    }

}

Coordinate hardAI(GameSettings* pMySettings, GameInfo* pMyGame, Coordinate* pPreHit, int returnMove){
    // hard difficulty algorithm
    int startRow = 0, startCol = 0; // the grid coordinates for the up-left corner of the entire ship area
    int preHitRow = 0, preHitCol = 0; // the grid coordinates of preHit
    int colMax = 0, rowMax = 0; // conditional statement for loops
    int i = 0, j = 0;
    int surroundings[4] = {0}; // the situation of the four spaces surrounding the previous hit space.
                        // 0 = available, 1 = hit, -1 = miss or N/A. surroundings[0] up, surroundings[1] right, surroundings[2] down, surroundings[3] left
    int dir = 0; // direction towards where the rest of the sunk ship is.

    if(returnMove >= 0){ // previous hit causes ship to sink
        preHitRow = (int)pPreHit->row % 32 - 1;
        preHitCol = pPreHit->col - 1;
        AIcheckSurrounding(pMySettings, pMyGame, pPreHit, surroundings);
        for (i = 0; i < 4; i++){ // go around surroundings() to check for the other hit, thus determining the value for dir
            if (surroundings[i] == 1){ // found the dir
                dir = i;
                break;
            }
        }
        switch(dir){
            // find the most top-left space grid coordinate
            case 0:
                startCol = preHitCol - 1;
                startRow = preHitRow - pMyGame->ships[returnMove].size;
                colMax = 3;
                rowMax = pMyGame->ships[returnMove].size + 2;
                break;
            case 1:
                startCol = preHitCol - 1;
                startRow = preHitRow - 1;
                colMax = pMyGame->ships[returnMove].size + 2;
                rowMax = 3;
                break;
            case 2:
                startCol = preHitCol - 1;
                startRow = preHitRow - 1;
                colMax = 3;
                rowMax = pMyGame->ships[returnMove].size + 2;
                break;
            case 3:
                startCol = preHitCol - pMyGame->ships[returnMove].size;
                startRow = preHitRow - 1;
                colMax = pMyGame->ships[returnMove].size + 2;
                rowMax = 3;
                break;
            default:
                printf("hardAI() encounters a fatal error. Program terminated.\n");
                exit(0);
        }

        // set the surroundings of the sunk ship all to miss, thus further helping AI to guess the next bomb drop location
        for (i = 0; i < rowMax; i++){
            for(j = 0; j < colMax; j++){
                if (checkBoarderGrid((startRow + i), (startCol + j), pMySettings)){ // if inside the grid
                    if (pMyGame->shipMap[startRow + i][startCol + j] != 'X')
                        pMyGame->shipMap[startRow + i][startCol + j] = '/';
                }
            }
        }


    }

    return mediumAI(pMySettings, pMyGame, pPreHit, returnMove);
}

Coordinate AIcoordinate(GameSettings* pMySettings, GameInfo* pMyGame, int AIlvl, Coordinate* pPreHit, int returnMove){
    // generate a coordinate based on AI level for AI move in the game

    switch(AIlvl){
        case 1: // easy, each coordinate choice is random but avoiding the spaces already visited
            return randomCoordinate(pMySettings, pMyGame, 0);

        case 2: // medium, built on top of easy, but proactively search for more hit when after an initial hit
            return mediumAI(pMySettings, pMyGame, pPreHit, returnMove);

        case 3: // hard, built upon medium, but also set the spaces around the ship to '/' after the ship has sunk
            return hardAI(pMySettings, pMyGame, pPreHit, returnMove);

        default:
            printf("AIcoordinate() encountered a fatal error. Program terminated.\n");
            exit(0);
    }

}
