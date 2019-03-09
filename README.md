# GAME_battleship
A console version of Hasbro's battleship game

_The following is converted from google doc using GD2md-html version 1.0β11_  
_Project derived from a school project of COP2220 in FAU in Summer 2018_  

## How to Launch the Game
* Clone all the source and header files.
* Create a new project in an IDE (e.g. Eclipse) and add all the files into the project.
* Build the projects.
* Run the executible file in a console (e.g. Terminal for Mac).
* Main menu upon game launch:
![alt text](https://github.com/FanchenBao/GAME_battleship/ "Main Menu")

## How to Play
There are five options on the main menu. Enter corresponding number to choose which option to go for.
1. Play the game immediately with default settings on game difficulty (easy), ship map design (10x10), ship size and number of ships (default five ships: Aircraft Carrier, Battleship, Submarine, Cruiser, and Destroyer). You will then be asked how you wish to deploy the ships, either manually or randomly. Following this, you will make a decision whether you want to go first, or let AI go first, or leave it to chance. After that, the game officially starts. Make sure you the console is wide enough to hold the ship maps for both the player and AI. Information regarding the current status of your and AI's fleet and level of difficulty are presented at the top. 
    * Enter a coordinate (e.g. A-1) to drop the bomb on AI's map.
    * You will be notified whether you have missed or made a hit.
    * AI will drop a bomb on you and you will be notified whether AI has missed or made a hit.
    * On the ship map, miss is represented by '/', hit 'X'
    * Game play screen shot: ![alt text](https://github.com/FanchenBao/GAME_battleship/ "Game Play")
    * When you or AI wins the game, you decide whether to play again. If you choose to play again, you will be directed back to the main menu.
    * Game win screen shot: ![alt text](https://github.com/FanchenBao/GAME_battleship/ "Game Win")
2. Select AI difficulty (easy, medium, and hard). Try all of them out and it is not difficult to notice how AI improves upon its decision making at higher difficulty level.
3. Customize ship. You can add a new ship cateogry (e.g. Sailboat), decide how big the ship should be (e.g. size 2), and how many of that particular ship you want to put on the map (e.g. quantity 2)
    * Customize ship screen shot: ![alt text](https://github.com/FanchenBao/GAME_battleship/ "Customize Ship")
    * Notice from the example above, a warning is given at the end of ship customization, indicating that the ship map is too small, and you should resize the ship map before starting the game. This requirement is in place due to the rule in ship deployment. With the current ship map size, it is most likely impossible to fit all the ships according to the deployment requirement. Therefore, you must resize the ship map
4. Resize ship map. You will be provided with the optimal area for the ship map size based on the current status of fleet (i.e. number of ships and size of ships). You can choose number of rows and columns with a product larger than the optimal value provided. If the ship size you decide upon is too small a warning will be given. 
    * Resize ship map screen shot: ![alt text](https://github.com/FanchenBao/GAME_battleship/ "Resize Ship Map")
    * Notice from the example above that 133 is the suggested optimal ship map area. The initial attempt to set row = col = 5 is rejected for being too small. Later attempt to set row = col = 12 is successful. Row and col values do not have to be equal (i.e. ship map can be rectangle in shape).
    * After resizing the ship map, you can start the game again. And now the game play will show the customization.
    * New game play after customization: ![alt text](https://github.com/FanchenBao/GAME_battleship/ "Game Play After Customization")

## Project Maintenance
This project is currently being maintained. If any bug shows up, please contact me at jonathonbao@gmail.com. Have fun!


## © 2018 Fanchen Bao 




# Development Information (you can skip this)
*   Header Comment of Source Code file
    ```
    /
    * Battleship Game Simulator
    * project4.c
    * author: Fanchen Bao
    * date: 
    */

    ```
## Goals (Requirements): 

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
    * Submarine should take two bomb drop to register a hit.
*   V4.0 (future development)
    *   Player can set up accounts in the game, view his previous game play history (e.g. win rate against different level of AI, highest score, etc.) 
    *   Two-player mode. After setting up accounts, two players can play against each other.


## Background

Battleship is a popular game due to its simplicity in implementation and rules. I used to play it with classmates in middle school (during class) by simply drawing and marking the grids. It can even be played as a memorization game with each person having the layout of the ship map in the head and call out the coordination of bomb drop on the rival's map, similar to a blind chess game. A Battleship Game Simulator can easily recreate all the fun and tactical brainstorm inherent in the game, but also eliminate the hassle of drawing the grids, memorizing the ship maps, or worse, donating greenbacks to Hasbro.


## Functional Specifications



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


## Design


### Game settings struct
```C
typedef struct{ // specific to each player, used during game play
    char* name;
    int size;
    int quant; // initial quantity of this specific ship
    int enable; // 1 = ship exist, 2 = ship deleted
} ShipSet; 

typedef struct{ 
    ShipSet* shipSettings; 
    size_t row; // row of the grid
    size_t col; // col of the grid
    size_t typesShips; // total types of ships
    int totalShips; // total number of ships
    int AIlvl; // the smartness of AI
    int minShipMapArea; // mininum of ship map area (overestimation) based on the current ship sizes and quantity
} GameSettings;
```


### Game Info struct
```C
typedef struct{ // used during game play
    char* name; 
    int size; 
    int shipLeft; // how many ships still left in the game
    int damage; // each hit increments damage. When damage == ship's size, ship sinks
    int undeployed; // number of ships of this specific category that have not been deployed yet
} Ship; 
 
typedef struct{ // specific to each player, used during game play
    Ship* ships; 
    char shipMap; // the grid with ship arrangement info in it
    int totalShipLeft; 
} GameInfo;
```

### Process Design
*   Process() is the overall function for the game app. It contains the declaration of all structs and key variables necessary for the game, and calls two main functions.
    *   iniUserSettings(): to initialize user settings (a game settings struct) for the game and populate the game settings with default values (see Settings Design for details).
    *   mainMenu(): start of the game where user input a choice to either start the game or customize game settings.


### Main Menu Design
```C
int mainMenu(int mainMenuChoice, GameSettings* pMySettings, GameSettings* pUserInput, char*** pRelayGrid, GameInfo* pMyGame, GameInfo* pAIGame, Coordinate* pBow, Coordinate* pStern, Coordinate* pBomb);
```
*   mainMenu() contains a switch statement on mainMenuChoice. Based on user's input for mainMenuChoice, mainMenu() calls the following core functions
```C
gamePlay() (see Game Play Design for details)

// get user-input for how difficult he/she wishes the AI to be
int getAIlvl(GameSettings* pUserInput);

// for each shipMenuChoice, get the information about the ship that user wants to modify
int getShipInfo(int shipMenuChoice, GameSettings* pUserInput, char* shipName, int* pShipSize, int* pShipQuant);

// get user-input grid row and col values
int getGridDimen(int* pRow, int* pCol, GameSettings* pUserInput);
```
*   Choice 1 is to start the game. Choices 2 to 4 are for game customization purposes.
*   After the switch statement, if the user input choices 2 to 4 for customization, a further function updateUserSettings() (see Settings Design for details) is called to update game settings according to user's customization.


### Settings Design
```C
// Initialize the user- input settings to its default status
void iniUserSettings(GameSettings* pUserInput);

// update the setting values in userInput after customization
void updateUserSettings(int mainMenuChoice, int shipMenuChoice, GameSettings* pUserInput, int inputAIlvl, char* shipName, int shipSize, int shipQuant, int shipIndex, int returnGGD, int newRow, int newCol); 

// initialize game settings based on userInput. mySettings is used throughout the game and cannot be modified, whereas userInput interacts with user and can be changed.
void setGameSettings(GameSettings* pMySettings, GameSettings* pUserInput);
```

### Game Play Design
```C
// control the game procession
int gamePlay(GameSettings* pMySettings, GameSettings* pUserInput, char*** pRelayGrid, GameInfo* pMyGame, GameInfo* pAIGame, Coordinate* pBow, Coordinate* pStern, Coordinate* pBomb);
```
*   gamePlay() first calls setGameSettings() to copy the settings from userInput into the actual game settings (mySettings).
*   Then, it initializes GameInfo by calling iniGameInfo() (see Initialization Design) for both the player and AI. 
*   After that, it asks user whether they want to generate the ship map by hand or have the computer randomly generate a map for him/her. This is achieved by calling either genPlayerShipMap() or genAIShipMap() (see details of both functions in Ship Map Design).
*   Later, it calls moveByMove() (see Execution Design for details) to allow player and AI to conduct the play and analyze outcome of each round. 


### Initialization Design
```C
// initialize the grid with given row and column number. A grid is the basis for a ship map.
char** iniGrid(size_t row, size_t col); 

// initialize GameInfo struct based on the parameters in GameSettings. iniGrid() is called inside to initialize a ship map for the player or AI.
void iniGameInfo(GameSettings* pMySettings, GameInfo* pMyGame); 
```

### Ship Map Design
```C
typedef struct { // record a user-input coordinate
    char row; // must be from A to max row representation
    int col; // must be from 1 to max row
} Coordinate;

// generate player's shipMap in the grid inside myGame. 
// getShipLocation() is called inside to get user-input coordinates for ship's bow and stern.
int genPlayerShipMap(GameSettings* pMySettings, GameInfo* pMyGame, Coordinate* pBow, Coordinate* pStern); 

// call a function getCoordinate() top prompt user to enter coordinates for ship bow and stern, 
// and then call a series of functions to determine whether the ship position based on 
// user-input coordinates is valid
int getShipLocation(GameSettings* pMySettings, GameInfo* pMyGame, Coordinate* pBow, Coordinate* pStern, int shipIndex, int shipOrder); 

// generate a ship map for AI to use, or a random ship map for player. 
// forPlayer variable is a flag to determine whether the function is called for AI or player. 
// A function randomCoordinate() is called inside to generate a random coordinate within 
// the given boundary of the ship map.
void genAIShipMap(GameSettings* pMySettings, GameInfo* pGame, int forPlayer); 

// generate one coordiate for AI. isGenMap: 
//1 = function used during ship map generation; 2 = function used during game play
Coordinate randomCoordinate(GameSettings* pMySettings, GameInfo* pGame, int isGenMap); 
```

### Execution Design
```C
// contains a while loop to simulate the actual game loop. 
// Each player follows the other in dropping bombs until one side's ships all sink. 
// A function move() is called inside to simulate the bomb drop. 
// A printResult() function prints out the outcome after each bomb drop on the screen
int moveByMove(int choice, GameSettings* pMySettings, GameInfo* pMyGame, GameInfo* pAIGame, Coordinate* pBomb, char*** pRelayGrid); 

// drop bomb on opponent's map, then decide whether it's a hit or miss. 
// isPlayer is a flag to indicate whether the current move is made by player or AI. 
// If isPlayer == 1 (player making the move), getCoordinate() is called to get a valid coordinate. 
// Otherwise, AICoordinate() is called to generate a coordinate for AI to drop bomb. 
// The AI coordinate generation is dependent on the AI level (i.e. game difficulty level)
int move(GameSettings* pMySettings, GameInfo* pGame, Coordinate* pBomb, Coordinate* pPreHit, char*** pRelayGrid, int AIlvl, int returnMove, int isPlayer); 

// generate a coordinate based on AI level for AI move in the game. 
// Two further functions are called inside: mediumAI() and hardAI(), 
// to generate difficulty-appropriate AI move (easy AI simply drops bomb randomly, 
// so no need for a different function. Just use randomCoordinate() suffices)
Coordinate AIcoordinate(GameSettings* pMySettings, GameInfo* pMyGame, int AIlvl, Coordinate* pPreHit, int returnMove); 
```
### Notes

[^1]:
     Similar to this [Hasbro Battleship Game](https://www.hasbro.com/en-us/product/battleship:2560F81B-5056-9047-F55A-F26A61C519C3)

[^2 ^3]:
     [Battleship Game Wiki](http://battleship.wikia.com/wiki/Battleship_(game))

<!-- GD2md-html version 1.0β11 -->
