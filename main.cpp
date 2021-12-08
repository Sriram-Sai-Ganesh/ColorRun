/*
SOFTWARE DESIGN PROJECT :   COLOR RUN
FEH 1281.02H                PAC 8:00
12/08/21                    Team F2
SRIRAM SAI GANESH           CALVIN MILUSH

main.cpp            NOTE: the scores.txt file must be in the same directory as main.cpp.

*/


#include <FEHLCD.h>
#include <FEHSD.h>
#include <vector>
#include <FEHUtility.h>
#include <FEHRandom.h>
#include <string>
#include <iostream>
#include <cmath>

// header file containing global variables
#include "GlobalVariables.h"

using namespace std;

// returns a 'Boundaries' struct with the parameters as struct variable values. 
Boundaries newBoundaries(int startX,int endX,int startY,int endY){
    Boundaries r;
    r.startX = startX;
    r.endX = endX;
    r.startY = startY;
    r.endY = endY;
    return r;
}

// returns random int from lower to upper parameters
int getRand(int lower=0, int upper=255){
    return Random.RandInt()%(upper-lower)+lower;
}

// COLOR FUNCTIONS___________________________________________________________

// return the (r,g,b) parameters as 1 int 24 bit color
int get24BitColor(int r, int g, int b){
     return (r<<16)|(g<<8)|b;
}

// returns x parameter as 24-bit equivalent of (x,x,x) rgb color
int get24BitColor(int x){
     return get24BitColor(x,x,x);
}

// gets a fully random color in 24bit format.
int getRand24BitColor(){
     return (getRand()<<16)|(getRand()<<8)|getRand();
}

// gets a random color within a distance 'd' from color 'col'. 
// Used to get another hex color very similar in shade to 'color' parameter.
int getNearbyColor(int color, int distance)
{
    int r = (color & 0xff0000) >> 16;
    int g = (color & 0x00ff00) >> 8;
    int b = (color & 0x0000ff);
    int colorArr[]={r,g,b};
    int lower, upper;
    for(int i=0;i<3;i++)
    {
        if(colorArr[i]-distance<0){
            lower=0;
        }
        else lower=colorArr[i]-distance;
        if(colorArr[i]+distance>255){
            upper=255;
        }
        else upper=colorArr[i]+distance;

        colorArr[i]=getRand(lower, upper);
    }
    return get24BitColor(colorArr[0],colorArr[1],colorArr[2]);
}

// get a color from the current palettes
int getRandPaletteColor(int distance=GlobalPaletteDistance)
{
    return getNearbyColor(difficultyColors[globalDifficultySetting], distance);
}

// set the Proteus LCD font color to a randomly generated color.
void setRandomFontColor(){
    int color=get24BitColor(getRand(),getRand(),getRand());
    LCD.SetFontColor(color);
}

// computes and returns distance between parameter points (x1,y1) and (x2, y2).
float getDistance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
} 

// WRITING AND COLOR UTILITY FUNCTIONS______________________________________________

// print param 'text' centered at param (xPos, yPos)
void writeCenteredAt(const char *text, int xPos, int yPos)
{
    xPos-=strlen(text) * CHAR_WIDTH/2;
    yPos-=CHAR_HEIGHT/2;    
    LCD.WriteAt(text, xPos, yPos);
}

// overloaded function to print single char param 'c' centered at (xPos, yPos)
void writeCenteredAt(char c, int xPos, int yPos)
{
    string text="";
    text.push_back(c);
    writeCenteredAt(text.c_str(), xPos, yPos);
}

// makes and draws an FEH icon with corner at (xPos, yPos) with param 'text' text, 
// param fontColor font color, and default param borderColor as BGCOLOR global variable.
FEHIcon::Icon iconCornerAt(char *text, int xPos, int yPos, int fontColor, int borderColor=BGCOLOR)
{
    FEHIcon::Icon button;
    button.SetProperties(text, xPos, yPos, strlen(text)*CHAR_WIDTH, CHAR_HEIGHT, borderColor, fontColor);
    button.Draw();
    return button;
}

// makes and draws an FEH icon centered at (xPos, yPos) with param 'text' text, 
// param fontColor font color, and default param borderColor as BGCOLOR global variable.
FEHIcon::Icon iconCenteredAt(char *text, int xPos, int yPos, int fontColor, int borderColor=BGCOLOR)
{
    xPos -= strlen(text) * CHAR_WIDTH/2;
    yPos -= CHAR_HEIGHT/2;

    FEHIcon::Icon button;
    button.SetProperties(text, xPos, yPos, strlen(text)*CHAR_WIDTH, CHAR_HEIGHT, borderColor, fontColor);
    button.Draw();
    return button;
}

// print menu title in color and with animation. returns menu title height to orient other menu options.
int printMenuTitleCenteredAt(int xPos, int yPos)
{
    char firstWord[]={"Color"};
    char secondWord[]={"Run"};

    // variables controlling color change behavior
    int sleepDurationMS = 200;
    int letterOverlap=2;
    int lineSpacing=20;
    int bottomPadding = 10;

    // variables controlling display properties
    int totalWidth=(strlen(firstWord)+strlen(secondWord)-letterOverlap)*CHAR_WIDTH;    
    int firstWordXStart=xPos-(totalWidth/2);
    int firstWordYStart=yPos+CHAR_HEIGHT/2;
    
    int secondWordXStart=xPos+(totalWidth/2)-(letterOverlap*(CHAR_WIDTH));
    int secondWordYStart=firstWordYStart+lineSpacing;
    
    // print each letter in colors similar to the chosen difficulty
    for(int i=0, lim=strlen(firstWord);i<lim;i++) {
        LCD.SetFontColor(getRandPaletteColor());
        writeCenteredAt(firstWord[i],firstWordXStart+(i*(CHAR_WIDTH+2)),firstWordYStart);
        Sleep(sleepDurationMS);
    }
    for(int i=0, lim=strlen(secondWord);i<lim;i++) {
        LCD.SetFontColor(getRandPaletteColor());
        writeCenteredAt(secondWord[i],secondWordXStart+(i*(CHAR_WIDTH+2)),secondWordYStart);
        Sleep(sleepDurationMS);
    }
    
    // returns total height of title
    return 2*CHAR_HEIGHT+lineSpacing+bottomPadding;
}

// rainbow title from current palette
void rainbowTitle(int xPos, int yPos, int *x, int *y)
{
    char firstWord[]={"Color"};
    char secondWord[]={"Run"};

    int sleepDurationMS = 30;
    int letterOverlap=2;
    int lineSpacing=20;
    int bottomPadding = 10;

    int totalWidth=(strlen(firstWord)+strlen(secondWord)-letterOverlap)*CHAR_WIDTH;
    
    int firstWordXStart=xPos-(totalWidth/2);
    int firstWordYStart=yPos+CHAR_HEIGHT/2;
    
    int secondWordXStart=xPos+(totalWidth/2)-(letterOverlap*(CHAR_WIDTH));
    int secondWordYStart=firstWordYStart+lineSpacing;

    int loopCounter=0;
    int letterJumpHeight=3;
    while(!LCD.Touch(x, y))
    {
        loopCounter++;
        for(int i=0, lim=strlen(firstWord);i<lim;i++) {
            if(loopCounter%2){
                LCD.SetFontColor(BLACK);
                writeCenteredAt(firstWord[i],firstWordXStart+(i*(CHAR_WIDTH+2)),firstWordYStart+letterJumpHeight);
                LCD.SetFontColor(getRandPaletteColor());
                writeCenteredAt(firstWord[i],firstWordXStart+(i*(CHAR_WIDTH+2)),firstWordYStart);
            }
            else{
                LCD.SetFontColor(BLACK);
                writeCenteredAt(firstWord[i],firstWordXStart+(i*(CHAR_WIDTH+2)),firstWordYStart);
                LCD.SetFontColor(getRandPaletteColor());
                writeCenteredAt(firstWord[i],firstWordXStart+(i*(CHAR_WIDTH+2)),firstWordYStart+letterJumpHeight);
            }
            Sleep(sleepDurationMS);
        }
        for(int i=0, lim=strlen(secondWord);i<lim;i++) {

            if(loopCounter%2==0){
                LCD.SetFontColor(BLACK);
                writeCenteredAt(secondWord[i],secondWordXStart+(i*(CHAR_WIDTH+2)),secondWordYStart);
                LCD.SetFontColor(getRandPaletteColor());
                writeCenteredAt(secondWord[i],secondWordXStart+(i*(CHAR_WIDTH+2)),secondWordYStart+letterJumpHeight);
            }
            else{
                LCD.SetFontColor(BLACK);
                writeCenteredAt(secondWord[i],secondWordXStart+(i*(CHAR_WIDTH+2)),secondWordYStart+letterJumpHeight);
                LCD.SetFontColor(getRandPaletteColor());
                writeCenteredAt(secondWord[i],secondWordXStart+(i*(CHAR_WIDTH+2)),secondWordYStart);
            }
            Sleep(sleepDurationMS);

            // LCD.SetFontColor(getRandPaletteColor());
            // writeCenteredAt(secondWord[i],secondWordXStart+(i*(CHAR_WIDTH+2)),secondWordYStart);
            // Sleep(sleepDurationMS);
        }
    }
}

// print menu title and options, return user choice number
int getMenuInput(){

    LCD.Clear();

    // variables controlling function behavior
    int sleepDurationMS=200;
    int titleHeight=printMenuTitleCenteredAt(XLIM/2,menu_start_y+menu_button_height);
    const int numOptions=sizeof menuOptions / sizeof menuOptions[0];

    LCD.SetFontColor(WHITE);

    int startY=menu_start_y+titleHeight;
    int xPos=XLIM/2;

    // display menu options from array
    for(int i=0;i<numOptions;i++){

        int yPos=startY+menu_button_height*(i+1);

        writeCenteredAt(menuOptions[i],xPos,yPos);
        
    }

    // set button start and end limits
    int touchX, touchY;

    int buttonStartX=xPos-(menu_word_length_limit*CHAR_WIDTH)/2;
    int buttonEndX=xPos+(menu_word_length_limit*CHAR_WIDTH)/2;
    while(1){

        // run rainbow title until valid button press
        rainbowTitle(XLIM/2,menu_start_y+menu_button_height,&touchX, &touchY);

        // if screen is touched, calculate which button was pressed. return if value is a valid menu option
        if(touchX>buttonStartX && touchX<buttonEndX)
        {
            touchY-=startY;
            touchY-=menu_button_height/2;
            touchY/=menu_button_height;

            if(touchY>=0 && touchY<numOptions)return touchY;
        }
    }
}

// wait for button with (text) press
void waitForButton(char *text, int xPos, int yPos){

    LCD.SetFontColor(GRAY);
    LCD.WriteAt(text,xPos,yPos);

    int touchX, touchY;
    while(1){
        while(!LCD.Touch(&touchX, &touchY));
        if(touchX>xPos && touchX<xPos+strlen(text)* CHAR_WIDTH && touchY>yPos-CHAR_HEIGHT && touchY<yPos+strlen(text)*CHAR_HEIGHT)return;
    }
}

// pause thread until icon is pressed
void waitForIconPress(FEHIcon::Icon button){
    int touchX,touchY;
    while (1)
    {
        if(LCD.Touch(&touchX,&touchY))
        {
            if(button.Pressed(touchX,touchY,1))
            {
                Sleep(300);
                return;
            }
        }
    }
}

// initializes array size rows x columns to 0. default values for parameters are screenRows and screenColumns
void initializeToZero(int arr[screenRows][screenColumns], int rows = screenRows, int columns = screenColumns)
{
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<columns;arr[i][j]=0,j++);
    }
}

// DRAWING FUNCTIONS FOR GAMEPLAY______________________________________________

// draws single 'planet' circle with radius (radius) centered at (xPos, yPos) in the array (planets). the planet has a color (color) with variation (distance).
void drawPlanetCenteredAt(int planets[screenRows][screenColumns], int xPos, int yPos, int radius, int color, int distance){
    
    // ensure that the drawing area isn't going off the screen
    if(xPos-radius < 0){
        xPos+=radius;
    }
    if(xPos+radius >XLIM){
        xPos-=radius;
    }

    // iterate over all possible spaces in a square around (xPos, yPos)
    for(int i=0;i<radius;i++)
    {
        for(int j=0;j<radius;j++)
        {
            // check if position is within radius, and color pixel if so. Repeat for all points.
            if(xPos+i<screenColumns && getDistance(xPos+i, yPos, xPos, yPos)<=radius){
                planets[yPos][xPos+i]=getNearbyColor(color, distance);
                
                if(yPos+j<screenRows && getDistance(xPos+i, yPos+j, xPos, yPos)<=radius){
                    planets[yPos+j][xPos+i]=getNearbyColor(color, distance);
                }
                if(yPos-j>=0 && getDistance(xPos+i, yPos-j, xPos, yPos)<=radius){
                    planets[yPos-j][xPos+i]=getNearbyColor(color, distance);
                }
            }
            if(xPos-i>=0 && getDistance(xPos-i, yPos, xPos, yPos)<=radius)
            {
                planets[yPos][xPos-i]=getNearbyColor(color, distance);
                if(yPos+j<screenRows && getDistance(xPos-i, yPos+j, xPos, yPos)<=radius){
                    planets[yPos+j][xPos-i]=getNearbyColor(color, distance);
                }
                if(yPos-j>=0 && getDistance(xPos-i, yPos-j, xPos, yPos)<=radius){
                    planets[yPos-j][xPos-i]=getNearbyColor(color, distance);
                }
            }

        }

    }
}

// repeatedly calls the drawPlanetCenteredAt function to draw multiple planets onto 2d array (planets).
Boundaries drawPlanets(int planets[screenRows][screenColumns], int numberOfPlanets, int color, int colorDistance, int radius)
{   

    // variables controlling function behavior
    // int planetStartX=50, planetStartY=0;
    int planetStartX=50, planetStartY=30;
    int planetEndY=screenRows/3;
    int planetXIncrement = screenColumns/(numberOfPlanets+1);

    int planetStartWidth=15, planetStartHeight=5;
    int planetEndWidth=30, planetEndHeight=10;
    
    for(int i=0;i<numberOfPlanets;i++){
        int xPos = getRand(planetStartX+planetXIncrement*i, planetStartX+planetXIncrement*(i+1));
        int yPos = getRand(planetStartY, planetEndY);
        int rad = getRand(planetStartWidth, planetEndWidth);

        drawPlanetCenteredAt(planets, xPos, yPos, rad, color, colorDistance);
        color=getNearbyColor(color, colorDistance+5);
        drawPlanetCenteredAt(planets, xPos, yPos, rad-(rad/4), color, colorDistance);
        color=getNearbyColor(color, colorDistance+5);
        drawPlanetCenteredAt(planets, xPos, yPos, rad-(rad/2), color, colorDistance);
    }
    // return boundary of modified indices in array
    return newBoundaries(0,screenColumns, 0, screenRows/3);
}

// draws randomly colored background as a series of colors onto the 'background' 2D array. 
// the background has a color (color) with variation (distance), 
// and a (starPercentChance)% chance of drawing stars.
void drawBackground(int background[screenRows][screenColumns],int color, int colorDistance, int starPercentChance)
{
    for(int i=0;i<screenRows;i++){
        for(int j=0;j<screenColumns;j++){
            if(getRand(0,350)<starPercentChance)
                background[i][j]=getNearbyColor(difficultyColors[globalDifficultySetting],100);
            else
                background[i][j]=getNearbyColor(color,colorDistance);
        }
        color=getNearbyColor(color,colorDistance);
    }
}

// draws randomly colored obstacle as a series of colors onto the 'obstacle' 2D array. 
// the obstacle has a color near to (color) with variation (distance),
// with dimensions (w, h).
void drawObstacle(int obstacle[screenRows][screenColumns], int obstacleY, int w, int h, int color, int colorDistance)
{
    int startX=screenColumns-w,endX=screenColumns;
    int startY=obstacleY-h, endY=obstacleY;

    for(int i=startX; i<endX; i++)
    {
        for(int j=startY; j<endY; j++){
            obstacle[j][i]=getNearbyColor(color,colorDistance);
        }
            color=getNearbyColor(color,colorDistance);
    }
    // recursively call function to draw interior rectangles for obstacle
    if(w>0 && h>0){
        drawObstacle(obstacle, obstacleY, w-5, h-5, color, colorDistance);
    }
}

// draws randomly colored ground as a series of colors onto the 'ground' 2D array. 
// the ground has a color (color) with variation (distance),
// and starts at height (groundHeight)
Boundaries drawGround(int ground[screenRows][screenColumns], int groundHeight, int color, int distance)
{

    for(int i=groundHeight;i<screenRows;i++)
    {
        for(int j=0;j<screenColumns;j++)
        {
            // set pixel to be a color near the base color
            ground[i][j]=getNearbyColor(color, 20);
        }
        // base color changes based on layer of ground
        color = getNearbyColor(color, 30);
    }
    // return limits of ground
    return newBoundaries(0,screenColumns, groundHeight, screenRows);
}

// draws sprite into 'sprite' array. 
// Sprite is drawn at (xPos,yPos) and has 
Boundaries drawSprite(int sprite[screenRows][screenColumns], int xPos, int yPos, int color, int distance, int figureNumber)
{
    // get figures from GlobalVariables.h and draw onto the 'sprite' array
    for(int i=yPos, row=0;row<20;i++, row++){
        for(int j=xPos, col=0;col<20;j++, col++){
            if(figure[figureNumber][row][col]){
                sprite[i][j]=getNearbyColor(color, distance-5);
            }
        }
    }
    // return boundaries of sprite.
    return newBoundaries(yPos, yPos+20, xPos, xPos+20);
}

//checks if two layers have any intersection at all, 
// checking if common position pixels are colored.
bool checkCollision(int sprite[screenRows][screenColumns], int obstacles[screenRows][screenColumns])
{
    for(int i=0;i<screenRows;i++){
        for(int j=0;j<screenColumns;j++){
            if(sprite[i][j]>0 && obstacles[i][j]>0)return true;
        }
    }
    return false;
}

// moves entire 2D array 1 space up
void moveUp(int screen[screenRows][screenColumns])
{
    for(int col=0;col<screenColumns;col++)
    {
        int first = screen[0][col];
        for(int row=0;row<screenRows-1;row++)
        {
            screen[row][col]=screen[row+1][col];
        }
        screen[screenRows-1][col]=first;
    }
}

// moves entire 2D array n spaces up
void moveUp(int arr[screenRows][screenColumns], int n){
    for(int i=0;i<n;i++)
    {
        // iteratively call overloaded function
        moveUp(arr);
    }
}

// moves entire 2D array 1 space down
void moveDown(int screen[screenRows][screenColumns])
{
    for(int col=0;col<screenColumns;col++)
    {
        int last = screen[screenRows-1][col];
        for(int row=0;row<screenRows-1;row++)
        {
            screen[row][col]=screen[row][col+1];
        }
        screen[0][col]=last;

    }
}

// moves entire 2D array n spaces down
void moveDown(int arr[screenRows][screenColumns], int n){
    for(int i=0;i<n;i++)
    {
        // recursively call overloaded function
        moveDown(arr);
    }
}

// displays (screen) 2D array onto proteus display, centered, on the 240x320 screen.
void displayScreen(int screen[screenRows][screenColumns])
{
    // startx and starty are coords to top left corner of screen
    // initialized to center (screen) on proteus display
    int startX = (XLIM-screenColumns)/2, startY = (YLIM-screenRows)/2;  
    for(int i=0;i<screenRows;i++)
    {
        for(int j=0;j<screenColumns;j++)
        {
            LCD.SetFontColor(screen[i][j]);
            LCD.DrawPixel(startX+j, startY+i);
        }
    }
}

// Joins 2 layers of a display, drawing (image) on top of (screen).
// restricts iteration to the start and end x and y specified in (coordLimits) struct.
void drawOntoScreen(int screen[screenRows][screenColumns], int image[screenRows][screenColumns], Boundaries coordLimits)
{
    for(int i=coordLimits.startY;i<coordLimits.endY;i++)
    {
        for(int j=coordLimits.startX;j<coordLimits.endX;j++)
        {
            if(image[i][j]>0)
            {
                screen[i][j] = image[i][j];
            }
        }
    }

}

// Joins 2 layers of a display, drawing (image) on top of (screen).
// does not restrict any iterations
void drawOntoScreen(int screen[screenRows][screenColumns], int image[screenRows][screenColumns])
{
    Boundaries defaultBoundaries = newBoundaries(0,screenColumns, 0,screenRows);
    drawOntoScreen(screen, image, defaultBoundaries);
}

// Joins 2 layers of a display, drawing (image) on top of (screen).
// draws the first (column) columns of (image) onto the last (column) columns of (screen).
void drawOntoScreen(int screen[screenRows][screenColumns], int image[screenRows][screenColumns], int column)
{
    for(int i=0;i<screenRows;i++)
    {
        for(int j=0;j<column;j++)
        {
            if(image[i][j]>0)
            {
                screen[i][screenColumns-j-column] = image[i][j];
            }
        }
    }

}

// moves the entire 2D array 1 position left.
// elements left of the array are moved to the right, wrapping the image.
void moveLeft(int arr[screenRows][screenColumns])
{
    int firstCol[screenRows];
    for(int i=0;i<screenRows;i++)
    {
        firstCol[i]=arr[i][0];
        for(int j=0;j<screenColumns-1;j++)
        {
           arr[i][j]=arr[i][j+1];
        }
        arr[i][screenColumns-1]=firstCol[i];
    }


}

// iteratively calls the moveLeft(arr) function to move entire 2D array left 'spaces' times
void moveLeft(int arr[screenRows][screenColumns], int spaces)
{
    for(int i=0;i<spaces;i++)
    {
        moveLeft(arr);   
    }

}

// moves the entire 2D array left.
// sets rightmost column elements to 0.
void moveLeftNoWrap(int screen[screenRows][screenColumns])
{
    for(int i=0;i<screenRows;i++)
    {
        for(int j=0;j<screenColumns-1;j++)
        {
           screen[i][j]=screen[i][j+1];
        }
        screen[i][screenColumns-1]=0;
    }

}

// moves the entire screen 'spaces' times
void moveLeftNoWrap(int screen[screenRows][screenColumns], int spaces)
{
    for(int i=0;i<spaces;i++)
    {
        moveLeftNoWrap(screen);   
    }

}

// draws all non-zero pixels from 'image' onto 'screen'. 
// Draws the first (column) columns of the image onto the last (column) columns of the screen.
Boundaries drawOntoEndOfScreen(int screen[screenRows][screenColumns], int image[screenRows][screenColumns], int numberOfColumns)
{
    Boundaries r = newBoundaries(screenColumns-numberOfColumns, numberOfColumns, 0, screenRows);
    numberOfColumns%=screenColumns;
    for(int rowCounter=0;rowCounter<screenRows;rowCounter++)
    {
        for(int i=numberOfColumns-1;i>=0;i--)
        {
            screen[rowCounter][screenColumns-numberOfColumns+i] = image[rowCounter][i];
        }
    }

    return r;
}

// shifts array 'yShift' units in + or - direction.
void yShiftArray(int arr[screenRows][screenColumns], int yShift)
{
    if(yShift==0)
    return;
    if(yShift>0)
    {
        moveUp(arr, yShift);
    }
    else moveDown(arr, abs(yShift));
}

// updates sprite position to perform jump animation, returning parabolic positions.
// the height returned is calculated using (airTime), the total time spent in the air,
// and (jumpSpeed), the speed of ascent and descent of the jump.
float getYShift(int sprite[screenRows][screenColumns], float airTime, float jumpSpeed)
{
    float t = airTime;
    
    float stretch = 0.1;
    stretch+=jumpSpeed/1000.0;
    float yShift = 80.25;
    float xShift = sqrt(yShift);

    return -(pow(stretch*t-xShift, 2))+yShift;
}


// FUNCTIONS HANDLING MAIN MENU IO____________________________________________________.

// display "play game" message and wait for "quit" button press
void play(){
    
    LCD.Clear();

    // VARIABLES***********************************************

    // variables deciding overall game behavior
    int score=0;
    int screenDrawSleep=0;

    // velocities for different layers, range from 0-99.
    // moveSpaces decide how many pixels to shift 2D array at each frame.
    int backgroundVelocity=98;
    int backgroundMoveSpaces=2+globalDifficultySetting;

    int planetVelocity=60;
    int planetMoveSpaces=1;

    int groundVelocity=99;
    int groundMoveSpaces=2+globalDifficultySetting;

    int obstacleVelocity=99;
    int obstacleMoveSpaces=2+globalDifficultySetting;


    // create arrays for each layer, and initialize to zero
    int screen[screenRows][screenColumns];
    initializeToZero(screen);
   
    int background[screenRows][screenColumns];
    initializeToZero(background);
    // variables responsible for specific layer attributes
    int starPercentChance=1;
   
    int planets[screenRows][screenColumns];
    initializeToZero(planets);
    int numberOfPlanets=1;
    int planetRadius=10;
    int planetColor=0xfefefe, planetColorDistance=10;

    int ground[screenRows][screenColumns];
    initializeToZero(ground);
    int groundHeight=50;
    int groundColor=0xfefefe, groundColorDistance=200;

    int obstacle[screenRows][screenColumns];
    initializeToZero(obstacle);
    int baseObstacleTimer=100;
    int obstacleTimer=baseObstacleTimer+(4-globalDifficultySetting)+getRand(1,100);
    int obY=screenRows-groundHeight, obColor=BLUE, obColorDistance=10;
    int obH=50, obW=35;
    int obArea=50*35;
    int obstaclePercentChance=((globalDifficultySetting+2)*10+60);

    int sprite[screenRows][screenColumns];
    initializeToZero(sprite);
    int spriteX = 60+(YLIM-screenColumns)/2, spriteY =screenRows-groundHeight-20; 
    int spriteColor=YELLOW, spriteColorDistance=10;
    float spriteJumpSpeed=50+(60*globalDifficultySetting);

    // INITIAL DRAWING****************************************************

    // initialize different discrete layers
    drawBackground(background,0x111111, 5,starPercentChance);

    Boundaries planetBoundaries = drawPlanets(planets,numberOfPlanets, getRand24BitColor(), 20,planetRadius);
    Boundaries groundBoundaries = drawGround(ground, screenRows-groundHeight, getRand24BitColor(), 50);
    Boundaries spriteBoundaries = drawSprite(sprite, spriteX, spriteY, spriteColor, spriteColorDistance, 1);

    // combine separate layers into 'screen' array
    drawOntoScreen(screen, background);
    drawOntoScreen(screen, planets, planetBoundaries);
    drawOntoScreen(screen, ground, groundBoundaries);

    // drawOntoScreen(screen, obstacles);
    drawOntoScreen(screen, sprite, spriteBoundaries);
    
    // display 'screen' to Proteus display
    displayScreen(screen);


    // GAME LOOP ************************************************************
    // variables

    int loopCounter=1;
   
    bool flag=1;
    float airTime=-1;
    int timeSinceObstacle=0;
    bool spawnObstacle=0, preppedObstacle=0;
    
    while(flag){
        score++;
        // if sprite collides with obstacle, end.
        if(checkCollision(sprite, obstacle)){
            Sleep(3.0);
            break;
        }

        // move 2D arrays of each layer based on their velocities (defined earlier)
        if(loopCounter%(100-backgroundVelocity)==0){
            moveLeft(background,backgroundMoveSpaces);
        }   
        if(loopCounter%(100-planetVelocity)==0){
            moveLeft(planets,planetMoveSpaces);
        }
        if(loopCounter%(100-groundVelocity)==0){
            moveLeft(ground,groundMoveSpaces);
        }

        // check if obstacle doesn't exist, and get random chance of spawn
        if(!spawnObstacle && obstaclePercentChance>getRand(1,100)&& loopCounter%(obstacleTimer)==0){
            // set random chance of spawn, and obstacle criteria based on game difficulty.
            obstacleTimer=baseObstacleTimer+(5-globalDifficultySetting)+getRand(1,100);
            spawnObstacle = 1;

            // random obstacle width and height based on difficulty
            obH = getRand(20,55);
            obW = (obArea*(0.7+globalDifficultySetting/5.0))/obH;

            obColor = getRand24BitColor();

            if(obColor<get24BitColor(100,100,100))  // reduce chance that obstacle color is not too dark
                obColor = getRand24BitColor();

            obColorDistance=getRand(1,30);
        }

        // if need to spawn obstacle, start spawn
        if(spawnObstacle)
        {
            // if not prepared, prepare obstacle
            if(!preppedObstacle){
                initializeToZero(obstacle);
                // get layer of obstacle from draw function
                drawObstacle(obstacle, obY, obW, obH, obColor, obColorDistance);
                preppedObstacle=1;
            }

            // if obstacle hits left border, despawn immediately
            if(obstacle[obY-obH+1][0]!=0)
            {
                timeSinceObstacle=0;
                spawnObstacle = 0;
                preppedObstacle = 0;
            }
            // increase time obstacle has been on screen.
            timeSinceObstacle++;
        }

        // move obstacle left without wrapping 
        // (unlike other layers, which wrap around)
        moveLeftNoWrap(obstacle, obstacleMoveSpaces);
        

        // get sprite color near existing color, for smooth color transition.
        spriteColor = getNearbyColor(spriteColor, 5);
        if(spriteColor<get24BitColor(150,150,150))
        {
            spriteColor+=get24BitColor(6,6,6);
        }
        spriteColor = getNearbyColor(spriteColor, 5);
        
        // set sprite layer to zero.
        initializeToZero(sprite);
        if(airTime>=0)  // if sprite is already jumping
        {
            int yShift = getYShift(sprite, airTime, spriteJumpSpeed);   // get Y position based on airTime
            if(yShift<=0){  
                // if sprite is at or below ground, reset airTime.
                airTime=-1;
                }
            else {
                // else draw sprite at yShift y position.
                drawSprite(sprite, spriteX, spriteY-yShift, spriteColor, spriteColorDistance, 2);
                airTime++;
            }
        }
        else {
            // if not jumping, alternate drawing two running models of sprite, 
            // to give appearance of run animation.
            // speed of run is based on global difficulty.
            drawSprite(sprite, spriteX, spriteY, spriteColor, spriteColorDistance, loopCounter/(32/(globalDifficultySetting+1)) % 2);
        }
        
        // SCREEN DRAWING AND DISPLAY*****************************************************

        // compress all layers onto the main screen layer
        drawOntoScreen(screen, background);
        drawOntoScreen(screen, planets);
        drawOntoScreen(screen, ground);
        drawOntoScreen(screen, obstacle);
        drawOntoScreen(screen, sprite);

        // Draw screen onto proteus display
        displayScreen(screen);

        // Set LCD font color to display running scoreboard
        LCD.SetFontColor(WHITE);
        writeCenteredAt("Score:", XLIM/5, 10);
        LCD.SetFontColor(BLACK);
        LCD.FillRectangle(XLIM/5+CHAR_WIDTH*3.5, 0, 200, 18);
        LCD.SetFontColor(WHITE);
        writeCenteredAt(to_string(score).c_str(), XLIM/5+CHAR_WIDTH*7, 10);


        Sleep(screenDrawSleep);
        
        loopCounter++;
        loopCounter%=100000;

        // watch for touch to jump sprite
        int touchX, touchY;
        if(LCD.Touch(&touchX, &touchY))
        {
            if(touchX>XLIM-30 && touchY>YLIM-30)
            {
                Sleep(200);
                flag=0;
            }
            else if (airTime<0 && touchY>YLIM/2)
            {
                airTime=1;
            }
            
        }
    }
    score--;

    //POST LOOP/COLLISION GAME END BEHAVIOR*****************************************

    // icon for quit button
    FEHIcon::Icon quitButton = iconCornerAt("QUIT", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT, WHITE);

    // arrays for letter-scrolling display
    int characterInts[3];
    char characterChars[3];
    char characterStrings[3][2];
    char characterStringFinal[4];
    characterStringFinal[3] = '\0';

    // initialize arrays with capital letters
    for (int i = 0; i < 3; i++){
        characterInts[i] = 65;
        characterChars[i] = (char)characterInts[i];
        characterStringFinal[i] = characterChars[i];
        characterStrings[i][0] = characterChars[i];
        characterStrings[i][1] = '\0';
    }

    // Set LCD Color and create icons for letter-scrolling buttons
    LCD.SetFontColor(BLACK);
    LCD.FillRectangle(XLIM/2-CHAR_WIDTH*1.5, YLIM/2-CHAR_WIDTH/2-1, CHAR_WIDTH*3, CHAR_HEIGHT);
    FEHIcon::Icon character1 = iconCenteredAt(characterStrings[0], XLIM/2-CHAR_WIDTH, YLIM/2, WHITE);
    FEHIcon::Icon character2 = iconCenteredAt(characterStrings[1], XLIM/2, YLIM/2, WHITE);
    FEHIcon::Icon character3 = iconCenteredAt(characterStrings[2], XLIM/2+CHAR_WIDTH, YLIM/2, WHITE);

    // check touches
    int touchX, touchY;
    while (1)
    {
        if (LCD.Touch(&touchX,&touchY))
        {
            // quit
            if (quitButton.Pressed(touchX,touchY,1))
            {
                break;
            }
            // increment character1 if touched, change label string.
            else if (character1.Pressed(touchX,touchY,1))
            {
                if (characterInts[0] < 90)
                    characterInts[0] += 1;
                else
                    characterInts[0] = 65;
                characterChars[0] = (char)characterInts[0];
                characterStringFinal[0] = characterChars[0];
                characterStrings[0][0] = characterChars[0];
                character1.ChangeLabelString(characterStrings[0]);
            }
            // increment character 2 if touched, change label string.
            else if (character2.Pressed(touchX,touchY,1))
            {
                if (characterInts[1] < 90)
                    characterInts[1] += 1;
                else
                    characterInts[1] = 65;
                characterChars[1] = (char)characterInts[1];
                characterStringFinal[1] = characterChars[1];
                characterStrings[1][0] = characterChars[1];
                character2.ChangeLabelString(characterStrings[1]);
            }
            // increment character 3 if touched, change label string.
            else if (character3.Pressed(touchX,touchY,1))
            {
                if (characterInts[2] < 90)
                    characterInts[2] += 1;
                else
                    characterInts[2] = 65;
                characterChars[2] = (char)characterInts[2];
                characterStringFinal[2] = characterChars[2];
                characterStrings[2][0] = characterChars[2];
                character3.ChangeLabelString(characterStrings[2]);
            }
            Sleep(200);
        }
    }

    // open score file to read scores
    FEHFile *scoreptr = SD.FOpen("scores.txt", "r");
    // vectors to hold all scores
    vector<LeaderboardScore> scoresVec;
    LeaderboardScore temp;
    // scan all scores into vector
    while(SD.FScanf(scoreptr, "%d%s", &(temp.score), &(temp.initials)) != EOF)
    {
        scoresVec.push_back(temp);
    }
    SD.FClose(scoreptr);
    
    // open score file to write scores, adding latest
    scoreptr = SD.FOpen("scores.txt", "w");
    int scoreWritten = 0;

    // iterate through all scores
    for (int j = 0; j < scoresVec.size(); j++){
        // if score is not inserted, print new score when it is higher than current iteration score
        if (score > (scoresVec.at(j)).score && !scoreWritten){
            SD.FPrintf(scoreptr, "%d\t%s\n", score, characterStringFinal);
            j--;
            scoreWritten = 1;
        }
        else{// if score is already in vector, finish printing iteration scores.
            SD.FPrintf(scoreptr, "%d\t%s\n", (scoresVec.at(j)).score, (scoresVec.at(j)).initials);
        }
    }
    // if not printed at the end, print score at bottom.
    if (!scoreWritten){
        SD.FPrintf(scoreptr, "%d\t%s\n", score, characterStringFinal);
    }
    // close output file.
    SD.FClose(scoreptr);
    // return to main menu.
    return;
}

// display "instructions" message and wait for "quit" button press
void displayInstructions(){
    
    LCD.Clear();
    // set variables controlling function behavior
    int titleColor = difficultyColors[globalDifficultySetting];
    int instructionsColor = WHITE;
    char instructions[][25] = {"Tap the bottom","half of the screen","to jump."," ","Jump over obstacles to","beat the high score!"};

    int startX=5, startY=5;
    int lineSpacing=10;
    
    int printWaitDuration=300;
    
    int buttonX=XLIM-5*CHAR_WIDTH, buttonY=YLIM-2*CHAR_HEIGHT;

    int xPos=startX;
    int yPos=startY+lineSpacing*3;

    LCD.SetFontColor(titleColor);
    LCD.WriteAt("Instructions:",startX, startY);

    // iterate through each string, and print to instructions page.
    for(int i=0, lim=sizeof(instructions)/sizeof(instructions[0]); i<lim; i++){
        // print instructions line
        LCD.SetFontColor(instructionsColor);
        LCD.WriteAt(instructions[i],xPos,yPos);
        Sleep(printWaitDuration);
        yPos+=CHAR_HEIGHT+lineSpacing;
    }
    
    // wait for user to hit 'quit' icon
    FEHIcon::Icon quitButton = iconCornerAt("QUIT", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT, instructionsColor);
    waitForIconPress(quitButton);
    
    // return to main menu
    return;
}

// display "set difficulty" message and wait for "quit" button press
void setDifficulty(){
    
    LCD.Clear();

    // draw button at center of screen
    FEHIcon::Icon difficultyButton = iconCenteredAt(difficultyOptions[globalDifficultySetting], XLIM/2, YLIM/2, difficultyColors[globalDifficultySetting]);
    // draw quit button
    FEHIcon::Icon quitButton = iconCenteredAt("QUIT", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT, WHITE);
    
    while (true)
    {
        float x, y;
        while(!LCD.Touch(&x, &y));
        if (difficultyButton.Pressed(x,y,0))
        {
            // if difficulty button is pressed increment globalDifficultySetting variable
            globalDifficultySetting++;
            globalDifficultySetting%=(sizeof difficultyOptions/sizeof difficultyOptions[0]);
            
            // clear LCD and re-draw screen
            LCD.Clear();
            quitButton = iconCenteredAt("QUIT", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT, WHITE);
            difficultyButton=iconCenteredAt(difficultyOptions[globalDifficultySetting], XLIM/2, YLIM/2, difficultyColors[globalDifficultySetting]);
            Sleep(BUTTON_PRESSED_SLEEP);

        }
        else if (quitButton.Pressed(x,y,0))
        {
            // if quit button is pressed break and return to main menu
            break;
        }
    }
    return;
}

// display the leaderboard from scores file
void displayLeaderboard(){
        
    LCD.Clear();

    // initialize pointer to scores file
    FEHFile *scoreptr = SD.FOpen("scores.txt", "r");
    int scoreHolder;
    char initialHolder[10];
    // write exactly 14 scores to screen
    for (int i = 1; i <= 14; i++)
    {   
        if (SD.FScanf(scoreptr, "%d%s", &scoreHolder, &initialHolder) == EOF){   
            //if no more scores available, stop display
            break;
        }
        LCD.WriteAt(i, 0, (i-1)*CHAR_HEIGHT);
        LCD.WriteAt('.', CHAR_WIDTH*1*to_string(i).length(), (i-1)*CHAR_HEIGHT);
        LCD.WriteAt(initialHolder, CHAR_WIDTH*3, (i-1)*CHAR_HEIGHT);
        LCD.WriteAt(scoreHolder, CHAR_WIDTH*8, (i-1)*CHAR_HEIGHT);
    }

    // close file input stream
    SD.FClose(scoreptr);

    // draw quit icon and wait for press.
    FEHIcon::Icon quitButton = iconCornerAt("QUIT", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT, GRAY);
    waitForIconPress(quitButton);
    return;
}

// display credits on screen
void displayCredits(){

    LCD.Clear();

    // set color based on global difficulty
    LCD.SetFontColor(difficultyColors[globalDifficultySetting]);
    writeCenteredAt("Color Run is by:",XLIM/3,20);

    LCD.SetFontColor(WHITE);
    writeCenteredAt("Calvin Milush",XLIM/2,20+CHAR_HEIGHT+15);
    writeCenteredAt("Sriram Sai Ganesh",XLIM/2,20+CHAR_HEIGHT*2+15);
    writeCenteredAt("Group F2",XLIM/2,20+CHAR_HEIGHT*6+15);
    writeCenteredAt("FEH 1281.02H PAC 8:00",XLIM/2,20+CHAR_HEIGHT*8+15);

    // draw quit icon and wait for press
    FEHIcon::Icon quitButton = iconCornerAt("QUIT", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT, GRAY);
    waitForIconPress(quitButton);
    
    // return to main menu
    return;
}

int main() {

    // Clear background
    LCD.SetBackgroundColor(BLACK);
    bool isFinished=0;
    while (!isFinished) {
        
        LCD.Update();
        LCD.Clear();

        // get menu choice
        int choice=getMenuInput();
        Sleep(BUTTON_PRESSED_SLEEP);

        // perform action based on menu choice
        switch(choice){
            case 0:{
                // handle play button press
                play();
                break;
            }
            case 1:{
                // handle Instructions button press
                displayInstructions();
                break;
            }
            case 2:{
                // handle Difficulty button press
                setDifficulty();
                break;
            }
            case 3:{
                // handle Leaderboard button press
                displayLeaderboard();
                break;
            }
            case 4:{
                // handle Credits button press
                displayCredits();
                break;
            }
            case 5:{
                // handle Quit button press
                isFinished=1;
            }
        }
    }
    return 0;
}