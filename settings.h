/*
 * settings.h
 *
 *  Created on: Jul 28, 2018
 *      Author: fanchen
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#define _CRT_SECURE_NO_WARNINGS
#include "utility.h"

typedef struct _shipset { // specific to each player, used during game play
    char* name;
    int size;
    int quant; // initial quantity of this specific ship
    int enable; // 1 = ship exist, 2 = ship deleted
} ShipSet;

typedef struct _gamesettings {
    ShipSet* shipSettings;
    size_t row; // row of the grid
    size_t col; // col of the grid
    size_t typesShips; // total types of ships
    int totalShips; // total number of ships
    int AIlvl; // the smartness of AI
    int minShipMapArea; // mininum of ship map area (overestimation) based on the current ship sizes and quantity
} GameSettings;

int calMinArea(GameSettings* pUserInput);
int maxShipSize(GameSettings* pUserInput);
void iniUserSettings(GameSettings* pUserInput);
int getAIlvl(GameSettings* pUserInput);
int getShipMenuChoice(GameSettings* pUserInput);
int getName(int shipMenuChoice, GameSettings* pUserInput, char* shipName, int* shipIndex);
int getSize(int shipMenuChoice, char* shipName, int* pShipSize, int maxShipSize);
int getQuant(int shipMenuChoice, char* shipName, int* pShipQuant, int maxShipQuant);
int getShipInfo(int shipMenuChoice, GameSettings* pUserInput, char* shipName, int* pShipSize, int* pShipQuant);
int getGridDimen(int* pRow, int* pCol, GameSettings* pUserInput);
void updateUserSettings(int mainMenuChoice, int shipMenuChoice, GameSettings* pUserInput, int inputAIlvl, char* shipName, int shipSize, int shipQuant, int shipIndex, int returnGGD, int newRow, int newCol);
void setGameSettings(GameSettings* pMySettings, GameSettings* pUserInput);
void deallocateSettings(GameSettings* pSettings);



#endif /* SETTINGS_H_ */
