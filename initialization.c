/*
 * initialization.c
 *
 *  Created on: Jul 28, 2018
 *      Author: fanchen
 */

#include "initialization.h"

char** iniGrid(size_t row, size_t col){
    // initialize the grid with given row and column number
    char** grid = calloc(row, row * sizeof(char*));
    for(int i = 0; i < (int)row; i++){
        grid[i] = (char*)calloc(col, col * sizeof(char));
    }

    return grid;
}

void deallocateGrid(GameSettings* pMySettings, char** grid){
	// deallocate all the memories of grid
	for(int i = 0; i < (int)pMySettings->row; i++)
		free(grid[i]);
	free(grid);
}

void iniGameInfo(GameSettings* pMySettings, GameInfo* pMyGame){
    // initialize GameInfo based on the parameters in GameSettings
    size_t nameSize = 0;

    pMyGame->totalShipLeft = pMySettings->totalShips;

    pMyGame->shipMap = iniGrid(pMySettings->row, pMySettings->col);

    pMyGame->ships = (Ship*)malloc(pMySettings->typesShips * sizeof(Ship));

    for(int i = 0; i < (int)pMySettings->typesShips; i++){
        nameSize = (size_t)(strlen(pMySettings->shipSettings[i].name) + 1);
        pMyGame->ships[i].name = (char*)calloc(nameSize, nameSize * sizeof(char));
        strcpy(pMyGame->ships[i].name, pMySettings->shipSettings[i].name);
        pMyGame->ships[i].size = pMySettings->shipSettings[i].size;
        pMyGame->ships[i].shipLeft = pMySettings->shipSettings[i].quant;
        pMyGame->ships[i].damage = 0;
        pMyGame->ships[i].undeployed = pMySettings->shipSettings[i].quant;
    }
}

void deallocateGameInfo(GameSettings* pMySettings, GameInfo* pGame){
	// deallocate ships and its names
	for(int i = 0; i < (int)pMySettings->typesShips; i++) // deallocate names first
		free(pGame->ships[i].name);
	free(pGame->ships); // then deallocate the struct array to avoid memory leak
	deallocateGrid(pMySettings, pGame->shipMap);
}
