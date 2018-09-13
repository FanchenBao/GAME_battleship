/*
 * settings.c
 *
 *  Created on: Jul 28, 2018
 *      Author: fanchen
 */

#include "settings.h"

int maxShipSize(GameSettings* pUserInput){
	// find the max ship size in a given array
	int maxSize = 0;

	for(int i = 0; i < (int)pUserInput->typesShips; i++){
		if (maxSize < pUserInput->shipSettings[i].size)
			maxSize = pUserInput->shipSettings[i].size;
	}

	return maxSize;
}

int calMinArea(GameSettings* pUserInput){
    // calculate approximate min area needed for ship map to comfortably hold all ships.
    int maxSize = maxShipSize(pUserInput);

    // overestimation of absolute minArea in order to make random ship assignment feasible.
    // no overlapping among the outer-edge of ships, tightly fit side-by-side. First and last ship bordering the ship map edge.
    return ((maxSize + 2) * (3 * pUserInput->totalShips -  2));
}

void iniUserSettings(GameSettings* pUserInput){
    // Initialize the user- input settings to its default status

    // default game values. User can be prompted to change these values
	char* defaultShips = "Aircraft Carrier, Battleship, Submarine, Cruiser, Destroyer,";
	int defaultShipSizes[] = {5, 4, 3, 3, 2};
	int defaultTypesShips = 5;
	int defaultQuant = 1;
	int defaultRow = 10;
	int defaultCol = 10;
	int defaultAIlvl = 1; // default AI level is easy
	int defaultEnable = 1;

    // housekeeping
    int offset = 0;
    char* pDefaultShips = 0;
    int i = 0;

    pUserInput->shipSettings = (ShipSet*)malloc((size_t)defaultTypesShips * sizeof(ShipSet));
    for (i = 0; i < defaultTypesShips; i++)
        pUserInput->shipSettings[i].name = (char*)calloc(MAX_CAPACITY, MAX_CAPACITY * sizeof(char));

    i = 0;
    pUserInput->totalShips = 0;
    pDefaultShips = defaultShips;
    while(sscanf(pDefaultShips, " %[^,],%n", pUserInput->shipSettings[i].name, &offset) > 0){
        pUserInput->shipSettings[i].size = defaultShipSizes[i];
        pUserInput->shipSettings[i].quant = defaultQuant;
        pUserInput->shipSettings[i].enable = defaultEnable;
        pUserInput->totalShips += pUserInput->shipSettings[i].quant;
        pDefaultShips += offset;
        i++;
    }

    pUserInput->row = (size_t)defaultRow;
    pUserInput->col = (size_t)defaultCol;
    pUserInput->typesShips = (size_t)defaultTypesShips;
    pUserInput->AIlvl = defaultAIlvl;
    pUserInput->minShipMapArea = calMinArea(pUserInput);

}

int getAIlvl(GameSettings* pUserInput){
    // get user-defined AI level
    int AIlvl = 0;
    char *AIlvls[] = {"placeholder", "Easy", "Medium", "Hard"};
    while(1){
        printf("\n=====================================\n");
        printf("Choose Difficulty (currently '%s'):\n", AIlvls[pUserInput->AIlvl]);
        printf("    1. Easy (default)\n    2. Medium\n    3. Hard\n\n    0. Return to main menu\n");
        AIlvl = getUserChoice(1);

        if(AIlvl < 0 || AIlvl > 3)
            printf("\nError: option %d is invalid.\n", AIlvl);
        else{
            if (AIlvl != 0){
                printf("\n+--------------------------------------+\n");
                if (AIlvl == 2)
                    printf("| New difficulty '%s' has been set.%*c", AIlvls[AIlvl], 1, '|');
                else
                    printf("| New difficulty '%s' has been set.%*c", AIlvls[AIlvl], 3, '|');
                printf("\n+--------------------------------------+\n");
            }
            return AIlvl;
        }
    }
}

int getShipMenuChoice(GameSettings* pUserInput){
    // get user choice for customizing ship
    int shipMenuChoice = 0;
    int i = 0, shipCount = 0;

   while(1){
        printf("\n========================================\n");
        printf("Current fleet lineup:\n\n");
        printf("%15s%10s%14s\n", "SHIP CATEGORY", "SIZE", "QUANTITY");
        printf("----------------------------------------\n");
        while( shipCount < (int)pUserInput->typesShips){
            if (pUserInput->shipSettings[i].enable){
                printf("%-22s%-13d%-16d\n", pUserInput->shipSettings[i].name, pUserInput->shipSettings[i].size, pUserInput->shipSettings[i].quant);
                shipCount++;
            }
            i++;
        }

        printf("\nHow do you wish to customize the ships?\n");
        printf("    1. Add a category\n    2. Delete a category\n    3. Modify a category\n\n    0. Return to main menu\n");
        shipMenuChoice = getUserChoice(1);

        if(shipMenuChoice < 0 || shipMenuChoice > 3)
            printf("\nError: option %d is invalid.\n", shipMenuChoice);
        else
            return shipMenuChoice;
   }
}

int getName(int shipMenuChoice, GameSettings* pUserInput, char* shipName, int* shipIndex){
	// get ship name from player.
	int i = 0;

	while(1){
		if (shipMenuChoice == 1)
			printf("\nInput the name of ship category you want to add (less than 16 digits)\n(type 0 to cancel)\n");
		else if (shipMenuChoice == 2)
			printf("\nInput the name of ship category you want to delete (case sensitive)\n(type 0 to cancel)\n");
		else
			printf("\nInput the name of ship category you want to modify (case sensitive)\n(type 0 to cancel)\n");

		getUserString(shipName);

		if (shipName[0] == '0' && strlen(shipName) == 1)
			return 0; // user wants to go back to previous menu
		else{
			if (shipMenuChoice == 1){
				if (strlen(shipName) > 16)
					printf("The name '%s' is too long. Please input again.\n", shipName);
				else
					return 1; // input successful
			}

			else{
				for (i = 0; i < (int)pUserInput->typesShips; i++){
					if(!strcmp(shipName, pUserInput->shipSettings[i].name)){
						*shipIndex = i;
						break;
					}
				}
				if(*shipIndex < 0)
					printf("\nError: Name '%s' is invalid. Please input again.\n", shipName);
				else
					return 1; // input successful
			}
		}
	}
}

int getSize(int shipMenuChoice, char* shipName, int* pShipSize, int maxShipSize){
	// get ship size from player
	while(1){
		if(shipMenuChoice == 1)
			printf("\nInput the size of %s (must not exceed %d)?\n(type 0 to cancel)\n", shipName, maxShipSize);
		else
			printf("\nInput a NEW size of (must not exceed %d)\n(type 0 to cancel)\n", maxShipSize);

		*pShipSize = getUserChoice(1);

		if (*pShipSize > maxShipSize)
			printf("\nError: Size %d is too big. Please input again.\n", *pShipSize);
		else if (*pShipSize < 0)
			printf("\nError: Size cannot be negative. Please input again.\n");
		else if (*pShipSize == 0)
			return 0; // user wants to go back to previous menu
		else
			return 1; // input successful
	}
}

int getQuant(int shipMenuChoice, char* shipName, int* pShipQuant, int maxShipQuant){
	// get ship quantity from player
	while(1){
		if(shipMenuChoice == 1)
			printf("\nInput the quantity of %s (must not exceed %d)?\n(type 0 to cancel)\n", shipName, maxShipQuant);
		else
			printf("\nInput a NEW quantity (must not exceed %d)?\n(type 0 to cancel)\n", maxShipQuant);

		*pShipQuant = getUserChoice(1);

		if (*pShipQuant > maxShipQuant)
			printf("\nError: Quantity %d is too many. Please input again.\n", *pShipQuant);
		else if (*pShipQuant < 0)
			printf("\nError: Quantity cannot be negative. Please input again.\n");
		else if (*pShipQuant == 0)
			return 0; // user wants to go back to previous menu
		else
			return 1; // input successful
	}
}

int getShipInfo(int shipMenuChoice, GameSettings* pUserInput, char* shipName, int* pShipSize, int* pShipQuant){
    // for each shipMenuChoice, get the information about the ship that user wants to modify
    // return value is the index of the ship in userInput.shipSettings array
    int maxShipSize = ((int)pUserInput->row > (int)pUserInput->col)?((int)pUserInput->row):((int)pUserInput->col);
    int maxShipQuant = 3; // max quantity of each ship category
    int shipIndex = -1;

    switch(shipMenuChoice){
        case 1:
            if(!getName(shipMenuChoice, pUserInput, shipName, &shipIndex))
            	return -2; // user wants to go back to previous menu
            if(!getSize(shipMenuChoice, shipName, pShipSize, maxShipSize))
            	return -2;
            if(!getQuant(shipMenuChoice, shipName, pShipQuant, maxShipQuant))
            	return -2;

            printf("\n+-----------------------------------+\n");
            printf("| New ship category has been added! |");
            printf("\n+-----------------------------------+\n");

            return -1; // no index needs return

        case 2:
        	if(!getName(shipMenuChoice, pUserInput, shipName, &shipIndex))
        		return -2; // user wants to go back to previous menu

            printf("\n+-----------------------------------+\n");
            printf("|  Ship category has been deleted!  |");
            printf("\n+-----------------------------------+\n");
            return shipIndex;

        case 3:
        	if(!getName(shipMenuChoice, pUserInput, shipName, &shipIndex))
        		return -2; // user wants to go back to previous menu

            printf("\nShip category '%s':\n    Size: %d spaces\n    Quantity: %d\n", shipName, pUserInput->shipSettings[shipIndex].size, pUserInput->shipSettings[shipIndex].quant);

            if(!getSize(shipMenuChoice, shipName, pShipSize, maxShipSize))
            	return -2;
            if(!getQuant(shipMenuChoice, shipName, pShipQuant, maxShipQuant))
            	return -2;

            printf("\n+-----------------------------------+\n");
            printf("|  Ship category has been modified! |");
            printf("\n+-----------------------------------+\n");
            return shipIndex;

        case 0:
            return -1; // do nothing and back to main menu

        default:
            printf("getShipInfo() encountered a fatal error. Program terminated.\n");
            exit(0);
    }

}

int getRowCol(int* pRow, int* pCol, int maxSize, int isRow){
	// get row or col for resizing the grid
	int temp = 0;

	while(1){
		if(isRow)
			printf("\nInput Row (must be between %d and 26):\n(type 0 to cancel)\n", maxSize);
		else
			printf("\nInput Col (must between %d and 26):\n(type 0 to cancel)\n", maxSize);

		temp = getUserChoice(1);
		// input at least needs to fit the largest ship
		if(temp < maxSize && temp > 0)
			printf("\nError: row number %d too small.\n", temp);
		else if(temp > 26)
			printf("\nError: row number %d too big.\n", temp);
		else if(temp < 0)
			printf("\nError: row number cannot be negative.\n");
		else if(temp == 0)
			return 0; // user cancels
		else{
			if(isRow)
				*pRow = temp;
			else
				*pCol = temp;
			return 1; // input successful
		}
	}
}

int getGridDimen(int* pRow, int* pCol, GameSettings* pUserInput){
    // get user-input grid row and col values
    int maxSize = maxShipSize(pUserInput);

    for(int i = 0; i < (int)pUserInput->typesShips; i++){
		if (maxSize < pUserInput->shipSettings[i].size)
			maxSize = pUserInput->shipSettings[i].size;
	}

    printf("\n========================================\n");
    printf("Current ship map size:\n");
    printf("    Row: %d\n    Column: %d\n", (int)pUserInput->row, (int)pUserInput->col);

    while(1){
        printf("\nOptimal ship map area should be larger than %d\n", pUserInput->minShipMapArea);
        if(!getRowCol(pRow, pCol, maxSize, 1)) // get row
        	return 0; // user cancels and goes back to main menu

        if(!getRowCol(pRow, pCol, maxSize, 0)) // get col
        	return 0; // user cancels and goes back to main menu

        if ((*pRow) * (*pCol) < pUserInput->minShipMapArea)
            printf("\nError: total area %d too small.\n", (*pRow) * (*pCol));
        else
            break;
    }

    printf("\n+-----------------------------------+\n");
    printf("|   Ship map resizing successful!   |");
    printf("\n+-----------------------------------+\n");

    return 1; // user input successful
}

void addShip(GameSettings* pUserInput, char* shipName, int shipSize, int shipQuant){
	// update user setting after a ship is added
	int newMinArea = 0; // store the new min ship map area after ship customization

	(pUserInput->typesShips)++;
	pUserInput->shipSettings = (ShipSet*)realloc(pUserInput->shipSettings, (size_t)pUserInput->typesShips * sizeof(ShipSet));
	pUserInput->shipSettings[pUserInput->typesShips - 1].name = (char*)calloc(MAX_CAPACITY, MAX_CAPACITY * sizeof(char));
	strcpy(pUserInput->shipSettings[pUserInput->typesShips - 1].name, shipName);
	pUserInput->shipSettings[pUserInput->typesShips - 1].size = shipSize;
	pUserInput->shipSettings[pUserInput->typesShips - 1].quant = shipQuant;
	pUserInput->shipSettings[pUserInput->typesShips - 1].enable = 1;
	pUserInput->totalShips += shipQuant;

	newMinArea = calMinArea(pUserInput);
	pUserInput->minShipMapArea = newMinArea;
	if (newMinArea > ((int)pUserInput->col * (int)pUserInput->row))
		printf("\nWARNING: Ship map too small, must be resized before game starts!\n");
	return;
}

void deleteShip(GameSettings* pUserInput, int shipIndex){
	// update user setting after a ship is deleted
	(pUserInput->typesShips)--;
	pUserInput->shipSettings[shipIndex].enable = 0; // set enable to 0 as if the ship is deleted
	pUserInput->totalShips -= pUserInput->shipSettings[shipIndex].quant;
	return;
}

void modifyShip(GameSettings* pUserInput, int shipSize, int shipQuant, int shipIndex){
	// update user setting after a ship is modified
	int newMinArea = 0; // store the new min ship map area after ship customization

	pUserInput->shipSettings[shipIndex].size = shipSize;

	pUserInput->totalShips -= pUserInput->shipSettings[shipIndex].quant;
	pUserInput->shipSettings[shipIndex].quant = shipQuant;
	pUserInput->totalShips += pUserInput->shipSettings[shipIndex].quant;

	newMinArea = calMinArea(pUserInput);
	pUserInput->minShipMapArea = newMinArea;
	if (newMinArea > ((int)pUserInput->col * (int)pUserInput->row))
		printf("\nWARNING: Ship map too small, must be resized before game starts!\n");
	return;
}

void updateUserSettings(int mainMenuChoice, int shipMenuChoice, GameSettings* pUserInput, int inputAIlvl, char* shipName, int shipSize, int shipQuant, int shipIndex, int returnGGD, int newRow, int newCol){
    // update the setting values in userInput
	switch(mainMenuChoice){
        case 1:
            break;
        case 2:
            if (inputAIlvl) // inputAIlvl = 0 means user wants to return to main menu
                pUserInput->AIlvl = inputAIlvl;
            break;
        case 3:
            switch(shipMenuChoice){
                case 1:
                    addShip(pUserInput, shipName, shipSize, shipQuant);
                    break;
                case 2:
                    deleteShip(pUserInput, shipIndex);
                    break;
                case 3:
                    modifyShip(pUserInput, shipSize, shipQuant, shipIndex);
                    break;
                case 0:
                    break;
                default:
                    printf("inner switch of updateUserSettings() encountered a fatal error. Program terminated.\n");
                    exit(0);
            }
            break;
        case 4:
            if (returnGGD){ // returnGGD = 0 means user wants to return to main menu
                pUserInput->row = newRow;
                pUserInput->col = newCol;
            }
            break;
        default:
            printf("updateUserSettings() encountered a fatal error. Program terminated.\n");
            exit(0);
    }
    return;
}

void setGameSettings(GameSettings* pMySettings, GameSettings* pUserInput){
    // initialize game settings based on userInput
    size_t nameSize = 0;
    int i = 0, shipCount = 0;

    pMySettings->row = pUserInput->row;
    pMySettings->col = pUserInput->col;
    pMySettings->typesShips = pUserInput->typesShips;
    pMySettings->AIlvl = pUserInput->AIlvl;
    pMySettings->totalShips = pUserInput->totalShips;

    pMySettings->shipSettings = (ShipSet*)malloc(pMySettings->typesShips * sizeof(ShipSet));
    while(shipCount < (int)pUserInput->typesShips){
        if(pUserInput->shipSettings[i].enable){ // check whether the ship has been deleted
        	nameSize = (size_t)(strlen(pUserInput->shipSettings[i].name) + 1);
            pMySettings->shipSettings[shipCount].name = (char*)calloc(nameSize, nameSize * sizeof(char));
            strcpy(pMySettings->shipSettings[shipCount].name, pUserInput->shipSettings[i].name);
            pMySettings->shipSettings[shipCount].size = pUserInput->shipSettings[i].size;
            pMySettings->shipSettings[shipCount].quant = pUserInput->shipSettings[i].quant;
            shipCount++;
        }
        i++;
    }

}

void deallocateSettings(GameSettings* pSettings){
	// deallocate the shipSettings and the name within each ShipSet struct
	for (int i = 0; i < (int)pSettings->typesShips; i++)
		free(pSettings->shipSettings[i].name);
	free(pSettings->shipSettings);
}
