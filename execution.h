/*
 * execution.h
 *
 *  Created on: Jul 28, 2018
 *      Author: fanchen
 */

#ifndef EXECUTION_H_
#define EXECUTION_H_

#define _CRT_SECURE_NO_WARNINGS
#include "AI.h"

void printTwoMaps(char** mapOne, char** mapTwo, GameSettings* pMySettings, GameInfo* pMyGame, GameInfo* pAIGame);
int printResult(int returnMove, GameSettings* pMySettings, GameInfo* pMyGame, GameInfo* pAIGame, char** relayGrid, int isPlayer);
int move(GameSettings* pMySettings, GameInfo* pGame, Coordinate* pBomb, Coordinate* pPreHit, char*** pRelayGrid, int AIlvl, int returnMove, int isPlayer);
int updateGameInfo(GameSettings* pMySettings, GameInfo* pGame, int gridRow, int gridCol);
int moveByMove(int choice, GameSettings* pMySettings, GameInfo* pMyGame, GameInfo* pAIGame, Coordinate* pBomb, char*** pRelayGrid);

#endif /* EXECUTION_H_ */
