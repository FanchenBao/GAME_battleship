/*
 * gameApp.c
 *
 *  Created on: Jul 28, 2018
 *      Author: fanchen
 */

#include "gameApp.h"

void banner(int isStart){
    if (isStart){
        printf("\n#################################################\n");
        printf("#                                               #\n");
        printf("#        Welcome to Battleship Simulator!       #\n");
        printf("#                                               #\n");
        printf("#################################################\n");
    }
    else{
        printf("\n#####################################################\n");
        printf("#                                                   #\n");
        printf("#      Thank you for using Battleship Simulator     #\n");
        printf("#           Live Long and Prosper \\\\//              #\n");
        printf("#                                                   #\n");
        printf("#####################################################\n");
    }
}

int getMainMenuChoice(){
    // get the user choice from main menu
    int choice = 0;

    while(1){
        printf("\n***************\n");
        printf("   MAIN MENU   ");
        printf("\n***************\n");
        printf("    1. Play Game\n    2. Select Difficulty\n    3. Customize Ship\n    4. Resize Ship Map\n\n    0. Exit\n");
        choice = getUserChoice(1);

        if(choice < 0 || choice > 4)
            printf("\nError: option %d is invalid.\n", choice);
        else
            return choice;
    }
}

int mainMenu(int mainMenuChoice, GameSettings* pMySettings, GameSettings* pUserInput, char*** pRelayGrid, GameInfo* pMyGame, GameInfo* pAIGame, Coordinate* pBow, Coordinate* pStern, Coordinate* pBomb){
    // based on mainMenuChoice, decides what to do next.
    // 1 = play game with default setting, 2 = select different AIlvl level, 3 = customize ship name/size/quantity, 4 = change size of the grid, 0 = exit the game

    int returnGGD = 0; // store return value from getGridDimen()
    // record user input
    char shipName[MAX_CAPACITY] = {'\0'};
    int shipSize = 0;
    int shipQuant = 0;
    int newRow = 0, newCol = 0;
    int inputAIlvl = 0;
    int shipMenuChoice = 0;
    int shipIndex = 0;

    switch(mainMenuChoice){
        case 1:
        	if (pUserInput->minShipMapArea > ((int)pUserInput->col * (int)pUserInput->row)){
        		printf("\nWARNING: Ship map too small. Please resize the ship map before game starts.\n");
        		return 1;
        	}

        	if(gamePlay(pMySettings, pUserInput, pRelayGrid, pMyGame, pAIGame, pBow, pStern, pBomb)){
        		deallocateGrid(pMySettings, *pRelayGrid);
        		deallocateSettings(pMySettings);
        		deallocateGameInfo(pMySettings, pMyGame);
        		deallocateGameInfo(pMySettings, pAIGame);
        		break; // back to main menu
        	}
        	else
        		return 0; // quit game

        case 2:
            inputAIlvl = getAIlvl(pUserInput);
            break;

        case 3:
            while(1){
                shipMenuChoice = getShipMenuChoice(pUserInput);
                shipIndex = getShipInfo(shipMenuChoice, pUserInput, shipName, &shipSize, &shipQuant);
                if (shipIndex != -2) // when user cancels, back to the shipMenu, otherwise back to the loop.
                    break;
            }
            break;

        case 4:
            returnGGD = getGridDimen(&newRow, &newCol, pUserInput);
            break;

        case 0:
            return 0;

        default:
            printf("manMenu() encountered a fatal error. Program terminated.\n");
            exit(0);
    }
    updateUserSettings(mainMenuChoice, shipMenuChoice, pUserInput, inputAIlvl, shipName, shipSize, shipQuant, shipIndex, returnGGD, newRow, newCol);
    return 1;
}

int getFirstMoveChoice(){
    // decide either player or AI make the first move
    int choice = 0;

    printf("\nThe game is ready! Do you want to start first? Enter one of the following options.\n");
    printf("    1: You go first\n    2: AI goes first\n    3: Coin toss\n\n    0: Return to main menu\n");
    while(1){
        choice = getUserChoice(1);
        if (choice < 0 || choice > 3)
            printf("\nError: option %d is invalid. Please enter the option again.\n", choice);
        else
            return choice;
    }
}

int getCoinTossChoice(){
	// get user's choice for heads or tail in a coin toss
	char coinToss = 0;

	printf("\nYou will go first if you win the coin toss.\n");
	while(1){
		printf("Input 'h' for heads and 't' for tails:\n(type 'C' to cancel)\n");
		coinToss = (char)getUserChoice(0);
		if(tolower(coinToss) != 'h' && tolower(coinToss) != 't' && tolower(coinToss) != 'c')
			printf("\nError! Please input a valid option.\n");
		else if (tolower(coinToss) == 'c') // player cancels
			return -1;
		else
			return (tolower(coinToss) == 'h') ? (1) : (0);
	}
}

int checkCoinToss(int coinToss){
	// check whether user's coin toss input has won
	int actualToss = rand() % 2; // heads = 1, tails = 0

	if(coinToss == -1) // player cancels
		return 0;
	else{
		if(coinToss == actualToss){ // player wins the coin toss
			printf("\nYou WON the coin Toss! You will go first.\n");
			return 1; // user go first
		}
		else{
			printf("\nYou LOST the coin Toss. AI will go first.\n");
			return 2;
		}
	}
}

int getGenMapChoice(){
	// get user-input choice whether to generate player's map manually or get a random map from AI.
	int choice = 0;

	printf("\n-------------------------------------\n");
	printf("How do you want your ships deployed?\n    1. Manual deployment\n    2. Random deployment\n\n    0. Return to main menu\n");
	while(1){
		choice = getUserChoice(1);
		if (choice < 0 || choice > 2)
			printf("\nError: %d is not a valid option.\n", choice);
		else
			return choice;
	}
}

int gamePlay(GameSettings* pMySettings, GameSettings* pUserInput, char*** pRelayGrid, GameInfo* pMyGame, GameInfo* pAIGame, Coordinate* pBow, Coordinate* pStern, Coordinate* pBomb){
	// control the game procession
    int coinToss = 0;
    int genMapChoice = 0;
    int firstMoveChoice = 0;

	// basic game settings
	setGameSettings(pMySettings, pUserInput);
	*pRelayGrid = iniGrid(pMySettings->row, pMySettings->col);
	iniGameInfo(pMySettings, pMyGame);
	iniGameInfo(pMySettings, pAIGame);

	// generating player's map
	genMapChoice = getGenMapChoice();
	if(genMapChoice == 1){
		if(!genPlayerShipMap(pMySettings, pMyGame, pBow, pStern)) // user cancels
			return 1;
	}
	else if (genMapChoice == 0) // user wants to go back to main menu
		return 1;
	else
		genAIShipMap(pMySettings, pMyGame, 1);

	// generate AI's map
	genAIShipMap(pMySettings, pAIGame, 0);

	firstMoveChoice = getFirstMoveChoice();
	if(firstMoveChoice == 3){ // coin toss
		coinToss = getCoinTossChoice();
		firstMoveChoice = checkCoinToss(coinToss);
	}
	if(firstMoveChoice == 0) // player cancels
		return 1;
	if(!moveByMove(firstMoveChoice, pMySettings, pMyGame, pAIGame, pBomb, pRelayGrid)) // player cancels
		return 1;
	if(!playAgain(getPlayAgainChoice()))
		return 0; // exit game
	else
		return 1;
}

char getPlayAgainChoice(){
    // get user input for whether to play again or not
    char userChoice = 0;

    while(1){
        printf("\nDo you want to play again? Y/N\n");
        userChoice = tolower((char)(getUserChoice(0)));
        if(userChoice == 'y' || userChoice == 'n')
            return userChoice;
        else
            printf("\nError! Please input 'Y' or 'N'\n");
    }
}

int playAgain(char userChoice){
    // decide whether user wants to quit or play again
    if(userChoice == 'y')
        return 1; // player wants to play again
    else{
        printf("\nGAME OVER\n");
        return 0;
    }
}


void process(){
    GameSettings mySettings;
    GameSettings userInput;
    GameInfo myGame;
    GameInfo AIGame;
    char** relayGrid = 0; // the grid used by player to drop bomb on opponent. It doesn't show any ship arrangement info but indicates hit and miss.

    Coordinate bow;
    Coordinate stern;
    Coordinate bomb;
    int mainMenuChoice = 0;

    srand(time(0));

    banner(1);
    iniUserSettings(&userInput);

    while(1){
        mainMenuChoice = getMainMenuChoice();
        if (!mainMenu(mainMenuChoice, &mySettings, &userInput, &relayGrid, &myGame, &AIGame, &bow, &stern, &bomb))
            break;
    }

    deallocateSettings(&userInput);
    banner(0);
}
