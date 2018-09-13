/*
 * utility.c
 *
 *  Created on: Jul 28, 2018
 *      Author: fanchen
 */

#include "utility.h"

int getUserChoice(int isChoiceNum){
    // get a user choice. if isChoiceNum = 1, user input is a single digit integer, otherwise a single char
    // function always return an int, thus a char will be converted to its corresponding int
    char strInput[MAX_CAPACITY] = {'\0'};
    char* pStrInput = 0;
    char cChoice = '\0'; // record char choice
    int iChoice = 0; // record int choice
    char buffer = '\0';

    // housekeeping
    int count = 0, offset = 0;

    while(1){
        memset(strInput, 0, MAX_CAPACITY * sizeof(strInput[0]));
        count = 0;
        offset = 0;
        if(fgets(strInput, MAX_CAPACITY - 1, stdin) == NULL){
            printf("\nError! Please input an option.\n");
            continue;
        }

        pStrInput = strInput;
        if (isChoiceNum){
            if(sscanf(pStrInput, " %d %c", &iChoice, &buffer) == 1)
                return iChoice;
        }
        else{
            while(sscanf(pStrInput, " %c%n", &cChoice, &offset) > 0){
                pStrInput += offset;
                count++;
            }
            if(count == 1)// only one input, so far so good
                return (int)cChoice;
        }
        printf("\nError! Please input one valid option.\n");
    }

}

void getUserString(char* dest){
    // get a string input from user. Specifically, this is to get input of a ship category name
    while(fgets(dest, MAX_CAPACITY -1 , stdin) == NULL)
        printf("\nError! Please make an input.\n");

    dest[strlen(dest)-1] = '\0';

    return;
}
