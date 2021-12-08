/*
SOFTWARE DESIGN PROJECT :   COLOR RUN
FEH 1281.02H                PAC 8:00
12/08/21                    Team F2
SRIRAM SAI GANESH           CALVIN MILUSH

GlobalVariables.h

*/

#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHRandom.h>
#include <string>
#include <iostream>
#include <cmath>

#ifndef GLOBAL_VARIABLES_H // header guard
#define GLOBAL_VARIABLES_H


// Proteus display pixel limits
extern int XLIM = 319;
extern int YLIM = 239;

// character properties
extern int CHAR_HEIGHT = 17;
extern int CHAR_WIDTH = 12;

//button sleep duration
extern int BUTTON_PRESSED_SLEEP = 300;


// menu settings
extern int menu_start_y = 0;
extern int menu_button_height = 25;


// longest width of a main menu word, main menu options
extern const int menu_word_length_limit=20;
extern const char menuOptions[][menu_word_length_limit] = { "Play","Instructions","Difficulty","Leaderboard","Credits", "Quit"};

// variables to display screen
const int screenRows = 200, screenColumns=300;

// colors
extern int BGCOLOR=BLACK;

// 0 1 or 2 index for Easy, Medium or Hard
extern int globalDifficultySetting=0;
extern char difficultyOptions[][10] = { "Easy","Medium", "Hard"};
extern const int difficultyColors[]={GREEN, ORANGE, RED};
extern const int GlobalPaletteDistance=80;

// sprite bit images
extern const int figure[3][20][20]
          ={{{0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,1,1,0,0,1,1,1,1,0,0,1,1,0,0,0},
            {0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0},
            {0,0,0,0,1,1,0,0,1,1,1,0,1,1,1,0,0,0,0,0},
            {0,0,0,0,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0},
            {0,0,1,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0},
            {0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0}},
            {{0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,0,1,1,1,0,1,1,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,0,0,0,0,0},
            {0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0}}, 
            {{0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,1,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,0},
            {0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0},
            {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0},
            {0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
            {0,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,0},
            {0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0},
            {0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0},
            {0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0},
            {0,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
            {0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0}}};

// Structure to store x and y boundaries of boxes
struct Boundaries{
    int startX=0, startY=0;
    int endX=YLIM, endY=XLIM;
}defaultBoundaries;

// Structure to store Leaderboard score with initials
struct LeaderboardScore{
    char initials[4];
    int score;
};

#endif