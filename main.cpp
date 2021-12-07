#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHRandom.h>
#include <string>
#include <iostream>
#include <cmath>

#include "GlobalVariables.h"

using namespace std;

// Structure to store x and y boundaries of boxes
struct Boundaries{
    int startX, startY;
    int endX, endY;
};

// Returns new boundaries variable with specified values. Acts like constructor.
Boundaries newBoundaries(int startX,int endX,int startY,int endY){
    Boundaries r;
    r.startX = startX;
    r.endX = endX;
    r.startY = startY;
    r.endY = endY;
    return r;
}

// return random int from lower to upper
int getRand(int lower=0, int upper=255){
    return Random.RandInt()%(upper-lower)+lower;
}

// COLOR FUNCTIONS___________________________________________________________

// return (r,g,b) parameters as 1 int 24 bit color
int get24BitColor(int r, int g, int b){
     return (r<<16)|(g<<8)|b;
}

// return x parameter as 24-bit equivalent of (x,x,x) rgb color
int get24BitColor(int x){
     return get24BitColor(x,x,x);
}

// return (r,g,b) parameters as 1 int 24 bit color
int getRand24BitColor(){
     return (getRand()<<16)|(getRand()<<8)|getRand();
}

// get a random color within a distance d from color col
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

// set LCD font color to random color
void setRandomFontColor(){
    int color=get24BitColor(getRand(),getRand(),getRand());
    LCD.SetFontColor(color);
}

// displays 5x5 grid of possible palettes at different distances from color.
void displayPossiblePalette(int color, int distance, int xs, int ys)
{
    int lowerX=30+xs, upperX=70+xs;
    int lowerY=30+ys, upperY=70+ys;
    
    for(int i=lowerX;i<upperX;i++)
    {
        for(int j=lowerY;j<upperY;j++)
        {
            LCD.SetFontColor(getNearbyColor(color,distance));
            LCD.DrawPixel(i,j);
            
        }
    }
}

// WRITING AND COLOR UTILITY FUNCTIONS______________________________________________

// print param text centered at (xPos, yPos)
void writeCenteredAt(const char *text, int xPos, int yPos)
{
    xPos-=strlen(text) * CHAR_WIDTH/2;
    yPos-=CHAR_HEIGHT/2;    
    LCD.WriteAt(text, xPos, yPos);
}

// overloaded function to print single char centered at (xPos, yPos)
void writeCenteredAt(char c, int xPos, int yPos)
{
    string text="";
    text.push_back(c);
    writeCenteredAt(text.c_str(), xPos, yPos);
}

// makes and displays an icon with corner at (xPos, yPos)
FEHIcon::Icon iconCornerAt(char *text, int xPos, int yPos, int fontColor, int borderColor=BGCOLOR)
{
    FEHIcon::Icon button;
    button.SetProperties(text, xPos, yPos, strlen(text)*CHAR_WIDTH, CHAR_HEIGHT, borderColor, fontColor);
    button.Draw();
    return button;
}

// makes and displays an icon centered at (xPos, yPos)
FEHIcon::Icon iconCenteredAt(char *text, int xPos, int yPos, int fontColor, int borderColor=BGCOLOR)
{
    xPos -= strlen(text) * CHAR_WIDTH/2;
    yPos -= CHAR_HEIGHT/2;

    FEHIcon::Icon button;
    button.SetProperties(text, xPos, yPos, strlen(text)*CHAR_WIDTH, CHAR_HEIGHT, borderColor, fontColor);
    button.Draw();
    return button;
}

// print menu title in color and with animation
int printMenuTitleCenteredAt(int xPos, int yPos)
{
    char firstWord[]={"Color"};
    char secondWord[]={"Run"};

    int sleepDurationMS = 200;
    int letterOverlap=2;
    int lineSpacing=20;
    int bottomPadding = 10;

    int totalWidth=(strlen(firstWord)+strlen(secondWord)-letterOverlap)*CHAR_WIDTH;
    
    int firstWordXStart=xPos-(totalWidth/2);
    int firstWordYStart=yPos+CHAR_HEIGHT/2;
    
    int secondWordXStart=xPos+(totalWidth/2)-(letterOverlap*(CHAR_WIDTH));
    int secondWordYStart=firstWordYStart+lineSpacing;
    for(int i=0, lim=strlen(firstWord);i<lim;i++) {
        // setRandomFontColor();
        LCD.SetFontColor(getRandPaletteColor());
        writeCenteredAt(firstWord[i],firstWordXStart+(i*(CHAR_WIDTH+2)),firstWordYStart);
        Sleep(sleepDurationMS);
    }
    for(int i=0, lim=strlen(secondWord);i<lim;i++) {
        // setRandomFontColor();
        LCD.SetFontColor(getRandPaletteColor());
        writeCenteredAt(secondWord[i],secondWordXStart+(i*(CHAR_WIDTH+2)),secondWordYStart);
        Sleep(sleepDurationMS);
    }
    

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

    while(!LCD.Touch(x, y))
    {
        for(int i=0, lim=strlen(firstWord);i<lim;i++) {
            // setRandomFontColor();
            LCD.SetFontColor(getRandPaletteColor());
            writeCenteredAt(firstWord[i],firstWordXStart+(i*(CHAR_WIDTH+2)),firstWordYStart);
            Sleep(sleepDurationMS);
        }
        for(int i=0, lim=strlen(secondWord);i<lim;i++) {
            // setRandomFontColor();
            LCD.SetFontColor(getRandPaletteColor());
            writeCenteredAt(secondWord[i],secondWordXStart+(i*(CHAR_WIDTH+2)),secondWordYStart);
            Sleep(sleepDurationMS);
        }
    }
}

// print menu and return choice number based on button press
int getMenuInput(){

    LCD.Clear();

    int sleepDurationMS=200;
    int titleHeight=printMenuTitleCenteredAt(XLIM/2,menu_start_y+menu_button_height);
    const int numOptions=sizeof menuOptions / sizeof menuOptions[0];

    LCD.SetFontColor(WHITE);

    int startY=menu_start_y+titleHeight;
    int xPos=XLIM/2;

    for(int i=0;i<numOptions;i++){

        int yPos=startY+menu_button_height*(i+1);

        writeCenteredAt(menuOptions[i],xPos,yPos);
        // Sleep(sleepDurationMS);
    }

    // LCD.SetFontColor(RED);
    // LCD.DrawPixel(XLIM/2,menu_button_height);        // Center of Screen

    int touchX, touchY;

    int buttonStartX=xPos-(menu_word_length_limit*CHAR_WIDTH)/2;
    int buttonEndX=xPos+(menu_word_length_limit*CHAR_WIDTH)/2;
    while(1){

        // while(!LCD.Touch(&touchX, &touchY));
        rainbowTitle(XLIM/2,menu_start_y+menu_button_height,&touchX, &touchY);
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

// pause until icon is pressed
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

// initializes array size rows x columns to 0.
void initializeToZero(int arr[screenRows][screenColumns], int rows = screenRows, int columns = screenColumns)
{
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<columns;arr[i][j]=0,j++);
    }
}

// DRAWING FUNCTIONS FOR GAMEPLAY______________________________________________

// draws single cloud centered at xPos and yPos.
void drawCloudCenteredAt(int clouds[screenRows][screenColumns], int xPos, int yPos, int width, int height, int color, int distance){
    
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            if(xPos+i<screenColumns){
                clouds[yPos][xPos+i]=getNearbyColor(color, distance);
                if(yPos+j<screenRows){
                    clouds[yPos+j][xPos+i]=getNearbyColor(color, distance);
                }
                if(yPos-j>=0){
                    clouds[yPos-j][xPos+i]=getNearbyColor(color, distance);
                }
            }
            if(xPos-i>=0)
            {
                clouds[yPos][xPos-i]=getNearbyColor(color, distance);
                if(yPos+j<screenRows){
                    clouds[yPos+j][xPos-i]=getNearbyColor(color, distance);
                }
                if(yPos-j>=0){
                    clouds[yPos-j][xPos-i]=getNearbyColor(color, distance);
                }
            }
            
        }

    }
}

// draws clouds onto 2d array image
Boundaries drawClouds(int clouds[screenRows][screenColumns], int numberOfClouds, int color, int distance)
{   
    int cloudStartX=0, cloudStartY=0;
    int cloudEndY=screenRows/3;
    int cloudXIncrement = screenColumns/numberOfClouds;

    
    int cloudStartWidth=15, cloudStartHeight=5;
    int cloudEndWidth=30, cloudEndHeight=10;
    
    for(int i=0;i<numberOfClouds;i++)
    {
        int xPos = getRand(cloudStartX+cloudXIncrement*i, cloudStartX+cloudXIncrement*(i+1));
        int yPos = getRand(cloudStartY, cloudEndY);
        int w = getRand(cloudStartWidth, cloudEndWidth);
        int h = getRand(cloudStartHeight, cloudEndHeight);

        drawCloudCenteredAt(clouds, xPos, yPos, w, h, color, distance);
    }
    // return boundary of modified indices in array
    return newBoundaries(0,screenColumns, 0, screenRows/3);
}

// draws colored background into 'background' 2D array
void drawBackground(int background[screenRows][screenColumns],int color, int colorDistance)
{
    for(int i=0;i<screenRows;i++)
    {
        for(int j=0;j<screenColumns;j++)
        {
            background[i][j]=getNearbyColor(color,colorDistance);
        }
    }
}

// draws obstacles
void drawObstacle(int obstacle[screenRows][screenColumns], int obstacleY, int w, int h, int color, int colorDistance)
{
    int startX=screenColumns-w,endX=screenColumns;
    int startY=obstacleY-h, endY=obstacleY;

    for(int i=startX; i<endX; i++)
    {
        for(int j=startY; j<endY; j++)
        obstacle[j][i]=getNearbyColor(color,colorDistance);
    }
}

// draws ground into 'ground' array
Boundaries drawGround(int ground[screenRows][screenColumns], int groundHeight, int color, int distance)
{
    for(int i=groundHeight;i<screenRows;i++)
    {
        for(int j=0;j<screenColumns;j++)
        {
            ground[i][j]=getNearbyColor(color, distance);
        }
    }
    return newBoundaries(0,screenColumns, groundHeight, screenRows);
}

// draws sprite into 'sprite' array
Boundaries drawSprite(int sprite[screenRows][screenColumns], int xPos, int yPos, int width, int height, int color, int distance)
{
    for(int i=yPos;i<yPos+height;i++){
        for(int j=xPos;j<xPos+width;j++){
            // sprite[i][j]=color;
            sprite[i][j]=getNearbyColor(color, distance);
        }
    }
    return newBoundaries(yPos, yPos+height, xPos, xPos+width);
}

//checks if two layers have any intersection
bool checkCollision(int sprite[screenRows][screenColumns], int obstacles[screenRows][screenColumns])
{
    for(int i=0;i<screenRows;i++){
        for(int j=0;j<screenColumns;j++){
            if(sprite[i][j]>0 && obstacles[i][j]>0)return true;
        }
    }
    return false;
}

// moves entire array 1 space up
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

// moves entire array n spaces up
void moveUp(int arr[screenRows][screenColumns], int n){
    for(int i=0;i<n;i++)
    {
        moveUp(arr);
    }
}

// moves entire array 1 space down
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

// moves entire array n spaces down
void moveDown(int arr[screenRows][screenColumns], int n){
    for(int i=0;i<n;i++)
    {
        moveDown(arr);
    }
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

// updates sprite position to perform jump animation
float getYShift(int sprite[screenRows][screenColumns], float airTime, float jumpSpeed)
{
    float t = airTime;
    
    float stretch = 0.1;
    stretch+=jumpSpeed/1000.0;
    float yShift = 80.25;
    float xShift = sqrt(yShift);

    return -(pow(stretch*t-xShift, 2))+yShift;
}

// displays screen onto proteus display
void displayScreen(int screen[screenRows][screenColumns])
{
    // startx and starty are coords to top left corner of screen
    // initialized to center screen on proteus display
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

// draws all non-zero pixels from 'image' onto 'screen', restricting loop to boundary coordinates.
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

// draws all non-zero pixels from 'image' onto 'screen'.
void drawOntoScreen(int screen[screenRows][screenColumns], int image[screenRows][screenColumns])
{
    Boundaries defaultBoundaries = newBoundaries(0,screenColumns, 0,screenRows);
    drawOntoScreen(screen, image, defaultBoundaries);
}

// draws all non-zero pixels from 'image' onto 'screen'.
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

// moves the entire screen
void moveLeft(int screen[screenRows][screenColumns])
{
    int firstCol[screenRows];
    for(int i=0;i<screenRows;i++)
    {
        firstCol[i]=screen[i][0];
        for(int j=0;j<screenColumns-1;j++)
        {
           screen[i][j]=screen[i][j+1];
        }
        screen[i][screenColumns-1]=firstCol[i];
    }


}

// moves the entire screen 'spaces' times
void moveLeft(int screen[screenRows][screenColumns], int spaces)
{
    for(int i=0;i<spaces;i++)
    {
        moveLeft(screen);   
    }

}

// moves the entire screen
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

// draws all non-zero pixels from 'image' onto 'screen', restricting loop to boundary coordinates.
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
// FUNCTIONS HANDLING MAIN MENU IO____________________________________________________

// display "play game" message and wait for "quit" button press
void play(){
    
    LCD.Clear();
    
    int screenDrawSleep=1;

    // velocities for different layers, 0-99
    // moveSpaces decide how many pixels to shift by each frame
    int backgroundVelocity=95;
    int backgroundMoveSpaces=2;

    int cloudVelocity=98;
    int cloudMoveSpaces=2;

    int groundVelocity=99;
    int groundMoveSpaces=2;

    int obstacleVelocity=99;
    int obstacleMoveSpaces=2;

   
    // create arrays for each layer, and initialize to zero
    int screen[screenRows][screenColumns];
    initializeToZero(screen);
   
    int background[screenRows][screenColumns];
    initializeToZero(background);
   
    int clouds[screenRows][screenColumns];
    initializeToZero(clouds);
    int numberOfClouds=5;
    int cloudColor=0xefefef, cloudColorDistance=10;

    int ground[screenRows][screenColumns];
    initializeToZero(ground);
    int groundHeight=50;

    int obstacle[screenRows][screenColumns];
    initializeToZero(obstacle);
    int obstacleTimer=200;
    int obY=screenRows-groundHeight, obColor=BLUE, obColorDistance=10;
    int obW=30, obH=40;

    int sprite[screenRows][screenColumns];
    initializeToZero(sprite);
    int spriteWidth=20, spriteHeight=20;
    int spriteX = 40+spriteWidth+(YLIM-screenColumns)/2, spriteY =screenRows-groundHeight-spriteHeight; 
    int spriteColor=YELLOW, spriteColorDistance=10;
    float spriteJumpSpeed=150;

    
    // initialize different discrete layers
    drawBackground(background, SKYBLUE, 15);
    Boundaries cloudBoundaries = drawClouds(clouds,numberOfClouds, cloudColor, cloudColorDistance);
    Boundaries groundBoundaries = drawGround(ground, screenRows-groundHeight, 0x9b7653-get24BitColor(globalDifficultySetting*32), 20);
    Boundaries spriteBoundaries = drawSprite(sprite, spriteX, spriteY, spriteWidth, spriteHeight, spriteColor, spriteColorDistance);

    // combine separate layers into 'screen' array
    drawOntoScreen(screen, background);
    drawOntoScreen(screen, clouds, cloudBoundaries);
    drawOntoScreen(screen, ground, groundBoundaries);


    // drawOntoScreen(screen, obstacles);
    drawOntoScreen(screen, sprite, spriteBoundaries);
    
    // display 'screen' to Proteus display
    displayScreen(screen);

    int loopCounter=1;
   
    bool flag=1;
    float airTime=-1;
    int timeSinceObstacle=0;
    bool spawnObstacle=0, preppedObstacle=0;
    FEHIcon::Icon pauseButton = iconCornerAt("PAUSE", XLIM-20, 20, BLACK);
    while(flag){
        if(checkCollision(sprite, obstacle))
        {
            Sleep(5.0);
            return;
        }
        if(loopCounter%(100-backgroundVelocity)==0){
            moveLeft(background,backgroundMoveSpaces);
        }   

        if(loopCounter%(100-cloudVelocity)==0){
            moveLeft(clouds,cloudMoveSpaces);
        }

        if(loopCounter%(100-groundVelocity)==0){
            moveLeft(ground,groundMoveSpaces);
        }

        if(!spawnObstacle && loopCounter%obstacleTimer==0){
            spawnObstacle = 1;
            cout<<"spawning\n";
        }
        // cout<<"\n"<<loopCounter;
        if(spawnObstacle)
        {
            if(!preppedObstacle){
                initializeToZero(obstacle);
                drawObstacle(obstacle, obY, obW, obH, obColor, obColorDistance);
                preppedObstacle=1;
            }

            // moveLeftNoWrap(obstacle, obstacleMoveSpaces);
            if(obstacle[obY-obH+1][0]!=0)
            {
                timeSinceObstacle=0;
                spawnObstacle = 0;
                preppedObstacle = 0;
            }
            timeSinceObstacle++;
        }
        moveLeftNoWrap(obstacle, obstacleMoveSpaces);

        initializeToZero(sprite);
        if(airTime>=0)
        {
            int yShift = getYShift(sprite, airTime, spriteJumpSpeed);
            if(yShift<=0){
                airTime=-1;
                }
            else {
                drawSprite(sprite, spriteX, spriteY-yShift, spriteWidth, spriteHeight, spriteColor, spriteColorDistance);
                airTime++;
            }
        }
        else {
            drawSprite(sprite, spriteX, spriteY, spriteWidth, spriteHeight, spriteColor, spriteColorDistance);
        }
        

        drawOntoScreen(screen, background);
        drawOntoScreen(screen, clouds);
        drawOntoScreen(screen, ground);
        drawOntoScreen(screen, obstacle);
        drawOntoScreen(screen, sprite);
        
        displayScreen(screen);

        Sleep(screenDrawSleep);
        loopCounter++;
        loopCounter%=100000;
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
            else if(pauseButton.Pressed(touchX, touchY, 0))
            {
                Sleep(400);
                waitForIconPress(pauseButton);
            }
        }
    }
    
    FEHIcon::Icon quitButton = iconCornerAt("QUIT", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT, WHITE);
    waitForIconPress(quitButton);
    
    return;
}

// display "instructions" message and wait for "quit" button press
void displayInstructions(){
    
    LCD.Clear();
    int titleColor = GRAY;
    int instructionsColor = WHITE;
    char instructions[][25] = {"Taaap the top","half of the screen","to jump.","Tap the bottom","half of the screen","to duck.","Jump over ____","Duck under ____","Beat the high score!"};

    int startX=5, startY=5;
    int lineSpacing=10;
    
    int printWaitDuration=600;
    

    int buttonX=XLIM-5*CHAR_WIDTH, buttonY=YLIM-2*CHAR_HEIGHT;


    int xPos=startX;
    int yPos=startY+lineSpacing*3;


    for(int i=0, lim=sizeof(instructions)/sizeof(instructions[0]); i<lim; i++){

        LCD.SetFontColor(titleColor);
        LCD.WriteAt("Instructions:",startX, startY);
        LCD.SetFontColor(instructionsColor);
        LCD.WriteAt(instructions[i],xPos,yPos);
        Sleep(printWaitDuration);
        yPos+=CHAR_HEIGHT+lineSpacing;

        if((i+1)%3==0 && i<lim-1){
            FEHIcon::Icon nextButton = iconCornerAt("Next", buttonX, buttonY, instructionsColor);
            waitForIconPress(nextButton);
            // waitForButton("Next",buttonX, buttonY);
            Sleep(BUTTON_PRESSED_SLEEP);
            LCD.Clear();
            yPos=startY+lineSpacing*3;
        }
    }

    // writeCenteredAt("Instructions Here",XLIM/3,20);
    // waitForButton("Quit", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT);
   FEHIcon::Icon quitButton = iconCornerAt("QUIT", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT, instructionsColor);
    waitForIconPress(quitButton);
   return;
}

// display "set difficulty" message and wait for "quit" button press
void setDifficulty(){
    
    
    LCD.Clear();

    FEHIcon::Icon difficultyButton = iconCenteredAt(difficultyOptions[globalDifficultySetting], XLIM/2, YLIM/2, difficultyColors[globalDifficultySetting]);
    FEHIcon::Icon quitButton = iconCenteredAt("QUIT", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT, WHITE);
    while (true)
    {
        float x, y;
        while(!LCD.Touch(&x, &y));
        if (difficultyButton.Pressed(x,y,0))
        {
            globalDifficultySetting++;
            LCD.Clear();
            quitButton = iconCenteredAt("QUIT", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT, WHITE);
            globalDifficultySetting%=(sizeof difficultyOptions/sizeof difficultyOptions[0]);

            difficultyButton=iconCenteredAt(difficultyOptions[globalDifficultySetting], XLIM/2, YLIM/2, difficultyColors[globalDifficultySetting]);
            Sleep(BUTTON_PRESSED_SLEEP);

        }
        else if (quitButton.Pressed(x,y,0))
        {
            break;
        }
    }
    return;
}

// display "leaderboard" placeholder and wait for "quit" button press
void displayLeaderboard(){
    LCD.Clear();
    writeCenteredAt("Leaderboard Here",XLIM/3,20);
    // waitForButton("Quit", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT);
    FEHIcon::Icon quitButton = iconCornerAt("QUIT", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT, GRAY);
    waitForIconPress(quitButton);
    return;
}

// display "credits" message and wait for "quit" button press
void displayCredits(){
    LCD.Clear();

    writeCenteredAt("Color Run is by:",XLIM/3,20);
    writeCenteredAt("Calvin Milush",XLIM/2,20+CHAR_HEIGHT+10);
    writeCenteredAt("Sriram Sai Ganesh",XLIM/2,20+CHAR_HEIGHT*2+10);
    FEHIcon::Icon quitButton = iconCornerAt("QUIT", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT, GRAY);
    waitForIconPress(quitButton);

    // writeCenteredAt("Credits Here",XLIM/3,20);
    // waitForButton("Quit", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT);
    return;
}

int main() {
    // Clear background
    LCD.SetBackgroundColor(BLACK);
    // LCD.Clear();

  
    // TESTING
    // int img[screenRows][screenColumns];
    // initializeToZero(img);
    // drawObstacleCenteredAt(img, screenColumns/2,screenRows,10,50,BLUE,5);
    // LCD.Clear();
    // for(int i=0;i<screenRows;i++)
    // {
    //     for(int j=0;j<screenColumns;j++)
    //     {   
    //         LCD.SetFontColor(img[i][j]);
    //         LCD.WriteAt(".",j,i);
                  
    //     }
    // }
    // Sleep(6.0);   
    // cout<<"\n\nDONEEEE";
    // return 0;

    while (1) {
        
        LCD.Update();
        LCD.Clear();
        

        // get menu choice
        int choice=getMenuInput();
        
        // // display user selected choice
        // char text[]={"You chose \""};
        // strcat(text,menuOptions[choice]);
        // strcat(text,"\".");
        // LCD.SetFontColor(LIGHTSKYBLUE);
        // writeCenteredAt(text,XLIM/2,YLIM-CHAR_HEIGHT);

        Sleep(200);

        // perform action based on menu choice
        switch(choice){
            case 0:{
                play();
                break;
            }
            case 1:{
                displayInstructions();
                break;
            }
            case 2:{
                setDifficulty();
                break;
            }
            case 3:{
                displayLeaderboard();
                break;
            }
            case 4:{
                displayCredits();
                break;
            }
            case 5:{
                return 0;
                break;
            }
        }

    }
    return 0;
}
