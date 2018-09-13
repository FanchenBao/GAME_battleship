/*
 * AI.h
 *
 *  Created on: Jul 28, 2018
 *      Author: fanchen
 */

#ifndef AI_H_
#define AI_H_

#define _CRT_SECURE_NO_WARNINGS
#include "shipMap.h"

void AIcheckSurrounding(GameSettings* pMySettings, GameInfo* pMyGame, Coordinate* pPreHit, int* surroundings);
Coordinate AInextCoord(int dir, Coordinate* pPre);
Coordinate AIcoordinate(GameSettings* pMySettings, GameInfo* pMyGame, int AIlvl, Coordinate* pPreHit, int returnMove);
Coordinate mediumAI(GameSettings* pMySettings, GameInfo* pMyGame, Coordinate* pPreHit, int returnMove);
Coordinate hardAI(GameSettings* pMySettings, GameInfo* pMyGame, Coordinate* pPreHit, int returnMove);

#endif /* AI_H_ */
