/*
 * gameApp.h
 *
 *  Created on: Jul 28, 2018
 *      Author: fanchen
 */

#ifndef GAMEAPP_H_
#define GAMEAPP_H_

#define _CRT_SECURE_NO_WARNINGS
#include "execution.h"

void banner(int isStart);

int getMainMenuChoice();
int mainMenu(int mainMenuChoice, GameSettings* pMySettings, GameSettings* pUserInput, char*** pRelayGrid, GameInfo* pMyGame, GameInfo* pAIGame, Coordinate* pBow, Coordinate* pStern, Coordinate* pBomb);

int getFirstMoveChoice();
int getCoinTossChoice();
int checkCoinToss(int coinToss);
int getGenMapChoice();

int gamePlay(GameSettings* pMySettings, GameSettings* pUserInput, char*** pRelayGrid, GameInfo* pMyGame, GameInfo* pAIGame, Coordinate* pBow, Coordinate* pStern, Coordinate* pBomb);

char getPlayAgainChoice();
int playAgain(char userChoice);

// the actual game with full integration of all functionalities
void process();


#endif /* GAMEAPP_H_ */
