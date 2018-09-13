/*
 * utility.h
 *
 *  Created on: Jul 28, 2018
 *      Author: fanchen
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> // exit(), calloc()
#include <string.h> // strlen(), strcmp()
#include <time.h> // time()
#include <math.h> // log10()
#include <ctype.h> // tolower()
#define MAX_CAPACITY 100

int getUserChoice(int isChoiceNum);
void getUserString(char* dest);

#endif /* UTILITY_H_ */
