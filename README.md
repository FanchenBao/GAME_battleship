# GAME_battleship
A console version of Hasbro's battleship game

_The following is converted from google doc using GD2md-html version 1.0β11_  
_Project derived from a school project of COP2220 in FAU in Summer 2018_  
_Game is run from main.c_


## © 2018 Fanchen Bao 



*   Header Comment of Source Code file

    ```
    /
    * Battleship Game Simulator
    * project4.c
    * author: Fanchen Bao
    * date: 
    */

    ```



# Goals (Requirements): 

The Battleship[^1] game will be simulated for a single player versus the computer AI. 



*   V1.0 (complete)
    *   Game is based on a 10 x 10 grid. 
    *   Size of ship and number of ships for each size are shown to the player as default.
    *   Player is given instruction to generate his ship map.
    *   AI generates its own ship map. The ship alignment is random.
    *   Player is given an option to go first or second.
    *   Each round of game play, player's own ship map (with ships) and AI's grid (without ships) are printed on the screen. Each hit and miss made by the player and AI is displayed on player's ship map (AI's hit and miss) or AI's grid (player's hit and miss).
    *   Message is printed with each hit or miss, by both sides.
    *   Game ends when one side loses all his ships (i.e. no limit to the number of bombs)
    *   Player is given option to play again or quit the game, upon which "Game Over" is shown.
*   V2.0 (complete)
    *   Player choice of grid size, with instructions on how big the grid should be to fit the current fleet of ships.
    *   Customization of ship size and number of ships. Including add a ship, delete a ship, and modify an existing ship.
    *   Computer AI level: easy, medium, hard. Easy AI drops bomb completely random, but it does not drop on the same spot twice. Medium builds upon Easy, but also has a feature to actively search for the next site to drop bomb after an initial hit. Hard builds upon Medium, but also marks the area surrounding a sunk ship so that no future drop will be made in that area.
    *   Player can choose to generate his ship map manually, or pick random generation.
    *   Player can choose "coin toss" option to decide which sides starts the game first. If player wins the coin toss, player goes first; if loses, AI goes first.
*   V3.0 (future development)
    *   A separate way of gaming in which the number of bombs are limited. The goal is to see how many hits one side can make before the bombs run out.
    *   Accordingly, a scoring system will be established, e.g. a hit = 100 pts, a miss = -10 pts, sinking a ship = 500 pts, etc.
*   V4.0 (future development)
    *   Player can set up accounts in the game, view his previous game play history (e.g. win rate against different level of AI, highest score, etc.) 
    *   Two-player mode. After setting up accounts, two players can play against each other.


# Background

Battleship is a popular game due to its simplicity in implementation and rules. I used to play it with classmates in middle school (during class) by simply drawing and marking the grids. It can even be played as a memorization game with each person having the layout of the ship map in the head and call out the coordination of bomb drop on the rival's map, similar to a blind chess game. A Battleship Game Simulator can easily recreate all the fun and tactical brainstorm inherent in the game, but also eliminate the hassle of drawing the grids, memorizing the ship maps, or worse, donating greenbacks to Hasbro.


# Functional Specifications



1.  A struct called Ship that stores ship category name, ship size, number of ship left, damage, and number of ships that haven't been deployed yet (used exclusively during ship map generation). This struct is used to during game play and player's ship map generation.
1.  A struct called GameInfo that contains an array of Ship struct, which stores all the ship information, along with the ship map grid, and the number of ships remaining. Each player has his own GameInfo struct. GameInfo is updated during the game play.
1.  A struct called ShipSet that stores ship category name, size, the initial quantity of each ship category, and whether the ship is enabled (set to 0 when the ship category is deleted by user).
1.  A struct called GameSettings containing the row and column value of grid, an array of ShipSet struct that stores ship name, size, quantity, and whether it is enabled. GameSettings struct also contains variable recording the total number of ships, the total types of ship categories, AI level, and minimum ship map area needed to fit all the ships.
1.  A struct called Coordinate containing the row and col values of a space on the ship map grid.
1.  A function to initialize user settings. Upon initialization, user settings are set to the default value, in which the ship map grid would be 10 rows by 10 columns. Default AI level is "easy". Default number of ship categories is 5, as shown below. The general rule[^2] for ship sizes is as follows. Each player has ONE ship of each size in default.


    1.  Aircraft carrier:  5
    1.  Battleship: 4
    1.  Submarine: 3
    1.  Cruiser: 3
    1.  Destroyer: 2
1.  Further functions to get user customization of game settings. Variables that allow customization include AI level (choose between easy, medium, and hard), addition, deletion or modification of a ship category, and resizing the ship map grid.
1.  A function to pass user settings to actual game settings. It is important to notice that both user settings and game settings are of the same GameSettings data type. But user settings is used exclusively at the beginning of the game when user is allowed to make modifications to the settings parameters. Once user completes customization, information from user settings is transferred to game settings, which will be used exclusively for the game play. No alteration is allowed for the game settings. 
1.  A function to initialize GameInfo, based on the information from GameSettings. This shall happen for the player and the AI, thus each side would have his own GameInfo struct. 
1.  A function to instruct player how to generate his ship map and create the map based on the coordinate pair provided. It would go through all the ships one by one and ask player where he wishes to place the ship on the grid. A function is needed to get Coordinates from player. Player would provide two coordinates, one for the bow (beginning) and the other stern (end) of the ship. Further functions will check whether the user-input coordinates are valid based on the rules detailed below. If invalid, player is prompt to enter coordinates again. The rules[^3] of ship arrangement are as follows. 


    1.  "Ships must be placed horizontally or vertically (never diagonally) across grid spaces, and can't hang over the outer grid boundary."
    1.  "Ships may not be placed next to each other (this is defined as the immediate surrounding spaces of a ship should not contain any parts of another ship), nor are ships allowed to share a space with another ship."
1.  A function to print out updated ship map as user is deploying the ships, such that user has an idea of the layout of the fleet.
1.  A function for AI to generate its ship map and store it in AI's grid, following the same ship arrangement rules. The coordinate generation for AI is completely random.
1.  A function to let player decide who to go first. Player can choose either to go first, or let opponent (AI, in this case) go first, or take a coin toss guess. If player guesses the coin toss correctly, he will go first.
1.  A print function to print two grids side-by-side on the screen. Left side is player's grid with ships on it, the right side AI's without ship. A hit is maked by "X", whereas miss "/". Both grids are labeled on the row A - J, and column 1 - 10. Above the grids will also show how many ships are still afloat for each ship categories as a reminder for the player the progression in the game.
1.  A function to get user input of which cell on the AI's grid to attack. User input is in the form of "Row-Column". E.g. User input "B-3" will attempt a hit on row B column 3.
1.  A similar function to generate AI's choice of hit. How AI generates its choice is dependent on its AI level. An easy AI would drop bomb randomly on the grid (but it would not drop on the same spot twice), even after it gets a hit. A medium AI behaves just like an easy one, except after it gets an initial hit, it would then hit only horizontally or vertically starting adjacent to the initial hit. The choice of whether to go horizontal or vertical is random. If there is no hit, AI will choose randomly among the remaining options. If it is a hit, AI will continue hitting in the same or opposite direction until the target ship is sunk. After a ship is sunk, medium AI resumes random hitting. A hard AI behaves just like a medium one, except after sinking a ship, it would label the area surrounding the ship as "/" (miss), because according to the rules, ships cannot be assigned right next to each other. This way, a hard AI reduces the ship map area for random hitting, which in turn increases its likelihood to get a hit.
1.  A function to update GameInfo for both sides (which ship got hit, how many hits already incurred on the ship, how many ships sunk in total, where on the grid is a ship hit or a miss, etc.) after the player or AI drops a bomb. If a hit occurs, a message is printed indicating the hit and also telling the player or AI whether the ship is still afloat after the hit. If a miss occurs, the message printed would be "Miss!". When a ship is sunk, relevant information will be printed additionally (e.g. "Your destroyer has been sunk!"
1.  A function to decide winner by noticing which side sinks all of the opponents' ships first. 
1.  A function to ask user whether to play again.
1.  The main menu function that integrates all the functions listed above.
1.  All cases of user-input contain the option for user to cancel the current operation and go back to previous menu or main menu.


# Design


## Game settings struct

**typedef** **struct**{ _// specific to each player, used during game play_ \
    char* name; \
    int size; \
    int quant; _// initial quantity of this specific ship_ \
    int enable; _// 1 = ship exist, 2 = ship deleted_ \
} ShipSet; \
 \
**typedef** **struct**{ \
    ShipSet* shipSettings; \
    size_t row; _// row of the grid_ \
    size_t col; _// col of the grid_ \
    size_t typesShips; _// total types of ships_ \
    int totalShips; _// total number of ships_ \
    int AIlvl; _// the smartness of AI_ \
    int minShipMapArea; _// mininum of ship map area (overestimation) based on the current ship sizes and quantity_ \
} GameSettings;


## Game Info struct

**typedef** **struct**{ _// used during game play_ \
    char* name; \
    int size; \
    int shipLeft; _// how many ships still left in the game_ \
    int damage; _// each hit increments damage. When damage == ship's size, ship sinks_ \
    int undeployed; _// number of ships of this specific category that have not been deployed yet_ \
} Ship; \
 \
**typedef** **struct**{ _// specific to each player, used during game play_ \
    Ship* ships; \
    char** shipMap; _// the grid with ship arrangement info in it_ \
    int totalShipLeft; \
} GameInfo;


## Process Design



*   Process() is the overall function for the game app. It contains the declaration of all structs and key variables necessary for the game, and calls two main functions.
    *   iniUserSettings(): to initialize user settings (a game settings struct) for the game and populate the game settings with default values (see Settings Design for details).
    *   mainMenu(): start of the game where user input a choice to either start the game or customize game settings.


## Main Menu Design

int mainMenu(int mainMenuChoice, GameSettings* pMySettings, GameSettings* pUserInput, char*** pRelayGrid, GameInfo* pMyGame, GameInfo* pAIGame, Coordinate* pBow, Coordinate* pStern, Coordinate* pBomb);



*   mainMenu() contains a switch statement on mainMenuChoice. Based on user's input for mainMenuChoice, mainMenu() calls the following core functions
    *   1: gamePlay() (see Game Play Design for details)
    *   2: int getAIlvl(GameSettings* pUserInput); _// get user-input for how difficult he/she wishes the AI to be_
    *   3: int getShipInfo(int shipMenuChoice, GameSettings* pUserInput, char* shipName, int* pShipSize, int* pShipQuant);  _// for each shipMenuChoice, get the information about the ship that user wants to modify_
    *   4: int getGridDimen(int* pRow, int* pCol, GameSettings* pUserInput); _// get user-input grid row and col values_
*   Choice 1 is to start the game. Choices 2 to 4 are for game customization purposes.
*   After the switch statement, if the user input choices 2 to 4 for customization, a further function updateUserSettings() (see Settings Design for details) is called to update game settings according to user's customization.


## Settings Design


    void iniUserSettings(GameSettings* pUserInput); _// Initialize the user- input settings to its default status_ \



    void updateUserSettings(int mainMenuChoice, int shipMenuChoice, GameSettings* pUserInput, int inputAIlvl, char* shipName, int shipSize, int shipQuant, int shipIndex, int returnGGD, int newRow, int newCol); _// update the setting values in userInput after customization_ \



    void setGameSettings(GameSettings* pMySettings, GameSettings* pUserInput); _// initialize game settings based on userInput. mySettings is used throughout the game and cannot be modified, whereas userInput interacts with user and can be changed._


## Game Play Design


    int gamePlay(GameSettings* pMySettings, GameSettings* pUserInput, char*** pRelayGrid, GameInfo* pMyGame, GameInfo* pAIGame, Coordinate* pBow, Coordinate* pStern, Coordinate* pBomb); _// control the game procession_ \




*   gamePlay() first calls setGameSettings() to copy the settings from userInput into the actual game settings (mySettings).
*   Then, it initializes GameInfo by calling iniGameInfo() (see Initialization Design) for both the player and AI. 
*   After that, it asks user whether they want to generate the ship map by hand or have the computer randomly generate a map for him/her. This is achieved by calling either genPlayerShipMap() or genAIShipMap() (see details of both functions in Ship Map Design).
*   Later, it calls moveByMove() (see Execution Design for details) to allow player and AI to conduct the play and analyze outcome of each round. 


## Initialization Design


    char** iniGrid(size_t row, size_t col); _// initialize the grid with given row and column number. A grid is the basis for a ship map. _


    void iniGameInfo(GameSettings* pMySettings, GameInfo* pMyGame); _// initialize GameInfo struct based on the parameters in GameSettings. iniGrid() is called inside to initialize a ship map for the player or AI._


## Ship Map Design


    **typedef** **struct** { _// record a user-input coordinate_ \
    char row; _// must be from A to max row representation_ \
    int col; _// must be from 1 to max row_ \
} Coordinate;


    int genPlayerShipMap(GameSettings* pMySettings, GameInfo* pMyGame, Coordinate* pBow, Coordinate* pStern); _// generate player's shipMap in the grid inside myGame. getShipLocation() is called inside to get user-input coordinates for ship's bow and stern._


    int getShipLocation(GameSettings* pMySettings, GameInfo* pMyGame, Coordinate* pBow, Coordinate* pStern, int shipIndex, int shipOrder); _// call a function getCoordinate() top prompt user to enter coordinates for ship bow and stern, and then call a series of functions to determine whether the ship position based on user-input coordinates is valid_ \



    void genAIShipMap(GameSettings* pMySettings, GameInfo* pGame, int forPlayer); _// generate a ship map for AI to use, or a random ship map for player. forPlayer variable is a flag to determine whether the function is called for AI or player. A function randomCoordinate() is called inside to generate a random coordinate within the given boundary of the ship map._


     \
Coordinate randomCoordinate(GameSettings* pMySettings, GameInfo* pGame, int isGenMap); _// generate one coordiate for AI. isGenMap: 1 = function used during ship map generation; 2 = function used during game play_


## Execution Design


    int moveByMove(int choice, GameSettings* pMySettings, GameInfo* pMyGame, GameInfo* pAIGame, Coordinate* pBomb, char*** pRelayGrid); _// contains a while loop to simulate the actual game loop. Each player follows the other in dropping bombs until one side's ships all sink. A function move() is called inside to simulate the bomb drop. A printResult() function prints out the outcome after each bomb drop on the screen_ \



    int move(GameSettings* pMySettings, GameInfo* pGame, Coordinate* pBomb, Coordinate* pPreHit, char*** pRelayGrid, int AIlvl, int returnMove, int isPlayer); _// drop bomb on opponent's map, then decide whether it's a hit or miss. isPlayer is a flag to indicate whether the current move is made by player or AI. If isPlayer == 1 (player making the move), getCoordinate() is called to get a valid coordinate. Otherwise, AICoordinate() is called to generate a coordinate for AI to drop bomb. The AI coordinate generation is dependent on the AI level (i.e. game difficulty level)_


    Coordinate AIcoordinate(GameSettings* pMySettings, GameInfo* pMyGame, int AIlvl, Coordinate* pPreHit, int returnMove); _// generate a coordinate based on AI level for AI move in the game. Two further functions are called inside: mediumAI() and hardAI(), to generate difficulty-appropriate AI move (easy AI simply drops bomb randomly, so no need for a different function. Just use randomCoordinate() suffices)_ \



     \



<!-- Footnotes themselves at the bottom. -->
## Notes

[^1]:
     Similar to this [Hasbro Battleship Game](https://www.hasbro.com/en-us/product/battleship:2560F81B-5056-9047-F55A-F26A61C519C3)

[^2]:

     [http://battleship.wikia.com/wiki/Battleship_(game)](http://battleship.wikia.com/wiki/Battleship_(game))

[^3]:

     [http://battleship.wikia.com/wiki/Battleship_(game)](http://battleship.wikia.com/wiki/Battleship_(game))


<!-- GD2md-html version 1.0β11 -->
