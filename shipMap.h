/*
 * shipMap.h
 *
 *  Created on: Jul 28, 2018
 *      Author: fanchen
 */

#ifndef SHIPMAP_H_
#define SHIPMAP_H_

#define _CRT_SECURE_NO_WARNINGS
#include "initialization.h"


typedef struct { // record a user-input coordinate
    char row; // must be from A to max row representation
    int col; // must be from 1 to max row
} Coordinate;

// check coordinate validity
int checkOrientation(Coordinate* pBow, Coordinate* pStern);
int checkBoarderGrid(int gridRow, int gridCol, GameSettings* pMySettings);
int checkSize(Coordinate* pBow, Coordinate* pStern, int shipSize, int* pUserSize, int orientation);
int checkSingleSpacing(Coordinate* pCoord, GameSettings* pMySettings, GameInfo* pMyGame);
int checkSpacing(Coordinate* pBow, Coordinate* pStern, GameSettings* pMySettings, GameInfo* pMyGame, int index, int orientation);
int coordValidate(Coordinate* pBow, Coordinate* pStern, GameSettings* pMySettings, GameInfo* pMyGame, int index, int isPlayer);

// gen AI(random) ship map
Coordinate randomCoordinate(GameSettings* pMySettings, GameInfo* pMyGame, int isGenMap);
void genAIShipMap(GameSettings* pMySettings, GameInfo* pAIGame, int forPlayer);

// gen player(manual) ship map
Coordinate getCoordiante(size_t row, size_t col, char* message);
int getShipLocation(GameSettings* pMySettings, GameInfo* pMyGame, Coordinate* pBow, Coordinate* pStern, int shipIndex, int shipOrder);
void printShipMap(GameSettings* pMySettings, GameInfo* pMyGame);
void updateShipMap(Coordinate* pBow, Coordinate* pStern, GameInfo* pMyGame, int index);
int genPlayerShipMap(GameSettings* pMySettings, GameInfo* pMyGame, Coordinate* pBow, Coordinate* pStern);


#endif /* SHIPMAP_H_ */
