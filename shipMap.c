/*
 * shipMap.c
 *
 *  Created on: Jul 28, 2018
 *      Author: fanchen
 */

#include "shipMap.h"

int checkOrientation(Coordinate* pBow, Coordinate* pStern){
    // check whether the two coordinates form a vertial or horizontal line. If not, return 0
    if(pBow->row == pStern->row)
        return 1; // horizontal
    else if (pBow->col == pStern->col)
        return 2; // vertical
    else
        return 0;
}

int checkBoarderGrid(int gridRow, int gridCol, GameSettings* pMySettings){
    // check whether a grid coordinate (converted from user-input coordinate) is outside the grid
    if (gridRow < 0 || gridCol < 0 || gridRow >= (int)pMySettings->row || gridCol >= (int)pMySettings->col)
        return 0; // outside
    else
        return 1; // inside
}

int checkSize(Coordinate* pBow, Coordinate* pStern, int shipSize, int* pUserSize, int orientation){
    // check whether the two coordinates create a size larger than expected
    *pUserSize = (orientation == 1) ? (abs(pBow->col - pStern->col) + 1) : (abs(pBow->row - pStern->row) + 1);
    if (*pUserSize == shipSize)
        return 1;
    else
        return 0;
}

int checkSingleSpacing(Coordinate* pCoord, GameSettings* pMySettings, GameInfo* pMyGame){
    // check whether the single bow or stern coordinate is already adjacent to another ship
    int topLeftRow = 0, topLeftCol = 0; // the top left 'empty' space immediate diagnol to the user-input coordinate, in the grid coordinate format
    int i = 0, j = 0;

    topLeftRow = ((int)pCoord->row) % 32 - 2;
    topLeftCol = pCoord->col - 2;

    for(i = topLeftRow; i < (topLeftRow + 3); i++){
        for(j = topLeftCol; j < (topLeftCol + 3); j++){
            if (checkBoarderGrid(i, j, pMySettings)){ // current space inside the grid
                if (pMyGame->shipMap[i][j]) // current space occupied by other ship
                    return 0;
            }
        }
    }

    return 1; // spacing checking for the single coordiate passes

}

int checkSpacing(Coordinate* pBow, Coordinate* pStern, GameSettings* pMySettings, GameInfo* pMyGame, int index, int orientation){
    // check whether the ships are right next to each other
    // This is done AFTER the bow and stern cooridnates pass the single spacing check
    int topLeftRow = 0, topLeftCol = 0; // the top left 'empty' space immediate diagnol to the left most ship space, in the grid coordinate
    int i = 0, j = 0;

    if (orientation == 1){ // horizontal
        topLeftRow = ((int)pBow->row) % 32 - 2;
        if (pBow->col < pStern->col) // bow left, stern right
            topLeftCol = pBow->col - 2;
        else // bow right, stern left
            topLeftCol = pStern->col - 2;
    }

    else{ // vertical
        topLeftCol = pBow->col - 2;
        if (pBow->row < pStern->row) // bow top, stern bottom
            topLeftRow = ((int)pBow->row) % 32 - 2;
        else // bow bottom, stern top
            topLeftRow = ((int)pStern->row) % 32 - 2;
    }

    // check whether the spaces surrounding the ship, as well as the potential spaces for the ship, overlaps some other ship
    // (note that shipMap hasn't been updated yet, therefore the coordinate for the current ship should not have been filled yet)
    if(orientation == 1){ // horizontal
        for(i = topLeftRow; i < (topLeftRow + 3); i++){
            for(j = topLeftCol; j < (topLeftCol + 2 + pMyGame->ships[index].size); j++){
                if (checkBoarderGrid(i, j, pMySettings)){ // current space inside the grid
                    if (pMyGame->shipMap[i][j]) // current space occupied by other ship
                        return 0;
                }
            }
        }
    }
    else{ // vertical
        for(i = topLeftRow; i < (topLeftRow + 2 + pMyGame->ships[index].size); i++){
            for(j = topLeftCol; j < (topLeftCol + 3); j++){
                if (checkBoarderGrid(i, j, pMySettings)){ // current space inside the grid
                    if (pMyGame->shipMap[i][j]) // current space occupied by other ship
                        return 0;
                }
            }
        }
    }
    return 1; // space checking passed
}

int coordValidate(Coordinate* pBow, Coordinate* pStern, GameSettings* pMySettings, GameInfo* pMyGame, int index, int isPlayer){
    // check whether the user-input coordiantes are valid.
    int userSize = 0; // record the size of ship based on user's coordinates input
    int orientation = 0; // 1 = horizontal, 2 = vertical, 0 = invalid

    orientation = checkOrientation(pBow, pStern);
    if (!orientation){
        if(isPlayer)
            printf("\nError: ship can ONLY be set horizontally or vertically.\n");
        return 0;
    }
    if (!checkSize(pBow, pStern, pMyGame->ships[index].size, &userSize, orientation)){
        if(isPlayer)
            printf("\nError: ship has size %d but your coordinates give size %d.\n", pMyGame->ships[index].size, userSize);
        return 0;
    }
    if (!checkSpacing(pBow, pStern, pMySettings, pMyGame, index, orientation)){
        if(isPlayer)
            printf("\nError: ships cannot be right next to each other or overlapping\n");
        return 0;
    }
    return 1;
}


Coordinate randomCoordinate(GameSettings* pMySettings, GameInfo* pGame, int isGenMap){
    // generate one coordiate for AI. isGenMap: 1 = function used during ship map generation; 2 = function used during game play
    Coordinate AIcoord;
    int row = (int)pMySettings->row;
    int col = (int)pMySettings->col;
    int gridRow = 0, gridCol = 0;

    while(1){
        AIcoord.row = (char)((rand() % row) + 97); // generate a random character from 'a' to max row
        AIcoord.col = (rand() % col) + 1; // generate a random number from 1 to max col
        if (isGenMap) // map generation
            return AIcoord;

        else{ // game play
            gridRow = (int)AIcoord.row % 32 - 1;
            gridCol = AIcoord.col - 1;
            if(pGame->shipMap[gridRow][gridCol] != 'X' && pGame->shipMap[gridRow][gridCol] != '/') // do not hit the same spot twice
                return AIcoord;
        }
    }
}

void genAIShipMap(GameSettings* pMySettings, GameInfo* pGame, int forPlayer){
    // generate a ship map for AI to use, or a random ship map for player
    Coordinate AIBow, AIStern;
    int count = 0;
    int maxStep = 1000; // allowing computer to take maximum 1000 steps to generate a map each time
    int isSuccess = 1;

    if(forPlayer)
    	printf("\nRandom ship map is being generated");
    else
    	printf("\nAI generating its ship map");

    // while loop is for the situation where one shot for map generation is not successful, AI will try again
    while(1){
    	for (int i = 0; i < (int)pMySettings->typesShips; i++){
    		for (int j = 0; j < pMySettings->shipSettings[i].quant; j++){
    			while(count <= maxStep){
    				if (!(count % 10)) // every 10 iteration of AI's efforts print a dot.
    					printf(".");
    				AIBow = randomCoordinate(pMySettings, pGame, 1); // get bow coordinate (pAIGame is not used)
    				if (!checkSingleSpacing(&AIBow, pMySettings, pGame))
    					continue;
    				AIStern = randomCoordinate(pMySettings, pGame, 1); // get stern coordinate
    				if (!checkSingleSpacing(&AIStern, pMySettings, pGame))
    					continue;

    				// after the single spacing check passes for both coordiates, check the validity of them together
    				if (coordValidate(&AIBow, &AIStern, pMySettings, pGame, i, 0))
    					break;
    				count++;
    			}
    			if (count <= maxStep){
    				updateShipMap(&AIBow, &AIStern, pGame, i); // update shipMap with the newly added ship
    				isSuccess = 1;
    			}
    			else{
    				isSuccess = 0; // too many steps
    				break;
    			}
    		}
    		if (!isSuccess)
    			break;
    	}
    	if(isSuccess)
    		break;
    	else{
    		// reset the map and try again
    		for (int i = 0; i < (int)pMySettings->row; i++)
    			memset(pGame->shipMap[i], '\0', pMySettings->col * sizeof(char));
    		count = 0;
    	}

    }

    if(forPlayer)
    	printf("\n\nRandom ship map has been generated for player!\n");
    else
    	printf("\n\nAI has deployed all its ships!\n");
}


Coordinate getCoordiante(size_t row, size_t col, char* message){
    // get a coordinate in the format of Row-Column from user.

    Coordinate userCoord;

    char strInput[MAX_CAPACITY] = {'\0'}; // get the whole line
    char* pStrInput = 0;

    // housekeeping and input validation
    int cycle = 0, offset = 0;

    while(1){
    	memset(strInput, 0, MAX_CAPACITY * sizeof(strInput[0]));
        printf("\nPlease input a %s (e.g. A-1):\n(type 'C' to cancel and return to main menu)\n", message);
        if(fgets(strInput, MAX_CAPACITY - 1, stdin) == NULL)
            printf("\nError! Please input a coordinate.\n");
        else{
        	if(tolower(strInput[0]) == 'c' && strlen(strInput) == 2){
        		userCoord.col = 0; // indicating user wants to cancel
        		return userCoord;
        	}
            pStrInput = strInput;
            cycle = 0;
            while(sscanf(pStrInput, " %c-%d%n", &userCoord.row, &userCoord.col, &offset) == 2){ // scan in row as a char, col as int
                pStrInput += offset;
                cycle ++;
            }
            if (cycle == 1){ // user follows the input format
                userCoord.row = tolower(userCoord.row); // set the user-input row to lowercase letter
                // check validity of the user-input row and col coordinate. The row coordinate can be upper or lower case letter. 97 = 'a'
                if ((int)userCoord.row >= 97 && (int)userCoord.row <= (97 + (int)row - 1) && userCoord.col >= 1 && userCoord.col <= (int)col)
                    return userCoord;
                else
                    printf("\nError: Coordinate %c-%d is invalid.\n", userCoord.row, userCoord.col);
            }
            else
                printf("\nError! Please input one valid coordinate.\n");

        }
    }

}

int getShipLocation(GameSettings* pMySettings, GameInfo* pMyGame, Coordinate* pBow, Coordinate* pStern, int shipIndex, int shipOrder){
    // prompt user to enter coordinates for ship bow and stern, and decide whether the ship position is valid
    // i is the index of the ship in ships array, j is the current count of the ship being generated
    printf("\nDEPLOYING %s (%d of %d):\n", pMyGame->ships[shipIndex].name, shipOrder, pMySettings->shipSettings[shipIndex].quant);
    printf("    Size: %d spaces\n", pMyGame->ships[shipIndex].size);
    while(1){
        *pBow = getCoordiante(pMySettings->row, pMySettings->col, "coordinate for bow"); // get bow coordinate
        if(!pBow->col)
        	return 0;
        if (!checkSingleSpacing(pBow, pMySettings, pMyGame)){
            printf("\nError: ships cannot be right next to each other or overlapping\n");
            continue;
        }
        *pStern = getCoordiante(pMySettings->row, pMySettings->col, "coordinate for stern"); // get stern coordinate
        if(!pStern->col)
        	return 0;
        if (!checkSingleSpacing(pStern, pMySettings, pMyGame)){
            printf("\nError: ships cannot be right next to each other or overlapping\n");
            continue;
        }

        // after the single spacing check passes for both coordiates, check the validity of them together
        if (coordValidate(pBow, pStern, pMySettings, pMyGame, shipIndex, 1)){
            printf("=======================================\n");
            printf("%s successfully deployed!\n", pMyGame->ships[shipIndex].name);
            printf("=======================================\n\n");
            (pMyGame->ships[shipIndex].undeployed)--;
            return 1;
        }
    }
}

void printShipMap(GameSettings* pMySettings, GameInfo* pMyGame){
    // print a shipMap with coordinates for row and col during ship deployment
    int i = 0, j = 0;
    printf("\nShips to be deployed:\n");
    for(i = 0; i < (int)pMySettings->typesShips; i++)
    	printf("    %d %s\n", pMyGame->ships[i].undeployed, pMyGame->ships[i].name);

    // col coordinates
    printf("\n   ");
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
    printf("\n");

    for(i = 0; i < (int)pMySettings->row; i++){
        printf("%c ", (i + 65)); // row coordinates
        for(j = 0; j < (int)pMySettings->col; j++){
            if(pMyGame->shipMap[i][j])
                printf("| %c ", pMyGame->shipMap[i][j]);
            else // when a space is not occupied, print a whitespace
                printf("|   ");
        }
        printf("|\n");

        printf("  +");
        for(j = 0; j < (int)pMySettings->col; j++)
            printf("---+");
        printf("\n");

    }
}

void updateShipMap(Coordinate* pBow, Coordinate* pStern, GameInfo* pMyGame, int index){
    // write the user generated ship arrangement into the shipMap
    int startRow = 0, startCol = 0; // the starting grid coordinate for ship
    char label = pMyGame->ships[index].name[0];
    int i = 0;

    // get the startRow and startCol grid coordinates
    if (pBow->row == pStern->row){ // horizontal
        startRow = ((int)pBow->row) % 32 - 1;
        if (pBow->col < pStern->col) // bow left, stern right
            startCol = pBow->col - 1;
        else // bow right, stern left
            startCol = pStern->col - 1;
    }

    else{ // vertical
        startCol = pBow->col - 1;
        if (pBow->row < pStern->row) // bow top, stern bottom
            startRow = ((int)pBow->row) % 32 - 1;
        else // bow bottom, stern top
            startRow = ((int)pStern->row) % 32 - 1;
    }

    // populate the spaces of the ship with its label
    if(pBow->row == pStern->row){ // horizontal
        for(i = startCol; i < (startCol + pMyGame->ships[index].size); i++)
            pMyGame->shipMap[startRow][i] = label;
    }
    else{ // vertical
        for(i = startRow; i < (startRow + pMyGame->ships[index].size); i++)
            pMyGame->shipMap[i][startCol] = label;
    }
}

int genPlayerShipMap(GameSettings* pMySettings, GameInfo* pMyGame, Coordinate* pBow, Coordinate* pStern){
    // generate player's shipMap in the grid inside myGame
	printf("\nTo deploy ships manually, you must follow the following RULES:\n");
    printf("    1. Ships can only be aligned horizontally or vertically.\n    2. Ships cannot be placed right next to or overlap each other\n    3. Ships cannot cross the grid boarder\n");
    printf("    4. Provide the ship's bow (start) and stern (end) coordinates (Row-Column) to deploy your ship onto the map.\n\n");

    printShipMap(pMySettings, pMyGame);
    for (int i = 0; i < (int)pMySettings->typesShips; i++){
        for (int j = 0; j < pMySettings->shipSettings[i].quant; j++){
            if(!getShipLocation(pMySettings, pMyGame, pBow, pStern, i, (j + 1))) // get user-input coordinates for bow and stern
            	return 0; // user cancels
            updateShipMap(pBow, pStern, pMyGame, i); // update shipMap with the newly added ship
            printShipMap(pMySettings, pMyGame);
        }
    }
    printf("\nAll your ships have been deployed!\n");
    return 1;
}
