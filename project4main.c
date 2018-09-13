/*
* Battleship Game Simulator
* project4.c
* author: Fanchen Bao
* Starting Date: July 16, 2018
* End Date: July 28, 2018
*/


#define _CRT_SECURE_NO_WARNINGS

#include "gameApp.h"

void test(){
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
        // printf("The option chose is: %d", mainMenuChoice);

        if (!mainMenu(mainMenuChoice, &mySettings, &userInput, &relayGrid, &myGame, &AIGame, &bow, &stern, &bomb))
            break;
//    	int shipSize = 0, shipQuant = 0;
//    	char shipName[MAX_CAPACITY];
//    	getShipInfo(1, &userInput, shipName, &shipSize, &shipQuant);

//        setGameSettings(&mySettings, &userInput);
//        relayGrid = iniGrid(mySettings.row, mySettings.col);
//        iniGameInfo(&mySettings, &myGame);
//        iniGameInfo(&mySettings, &AIGame);

        // genPlayerShipMap(&mySettings, &myGame, &bow, &stern);
//        genAIShipMap(&mySettings, &myGame);
//        genAIShipMap(&mySettings, &AIGame);

        // moveByMove(getFirstMoveChoice(), &mySettings, &myGame, &AIGame, &bomb, &relayGrid);

        // if(moveByMove(1, &mySettings, &myGame, &AIGame, &bomb, &relayGrid) == 1)
        //     userCount++;
        // else
        //     AIcount++;

        // count++;



    }
    free(userInput.shipSettings);

}

int main() {
//    test();
    process();
    return 0;
}
