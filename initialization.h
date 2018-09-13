/*
 * initialization.h
 *
 *  Created on: Jul 28, 2018
 *      Author: fanchen
 */

#ifndef INITIALIZATION_H_
#define INITIALIZATION_H_

#define _CRT_SECURE_NO_WARNINGS
#include "settings.h"

typedef struct{ // used during game play
    char* name;
    int size;
    int shipLeft; // how many ships still left in the game
    int damage; // each hit increments damage. When damage == ship's size, ship sinks
    int undeployed; // number of ships of this specific category that have not been deployed yet
} Ship;

typedef struct{ // specific to each player, used during game play
    Ship* ships;
    char** shipMap; // the grid with ship arrangement info in it
    int totalShipLeft;
} GameInfo;

char** iniGrid(size_t row, size_t col);
void iniGameInfo(GameSettings* pMySettings, GameInfo* pMyGame);
void deallocateGrid(GameSettings* pMySettings, char** grid);
void deallocateGameInfo(GameSettings* pMySettings, GameInfo* pGame);

#endif /* INITIALIZATION_H_ */
