#include <FEHLCD.h>
#include <FEHUtility.h>
#include <FEHRandom.h>
#include <string>
#include <cmath>

using namespace std;

// Proteus display pixel limits
#define XLIM 319
#define YLIM 239

// character properties
#define CHAR_HEIGHT 17
#define CHAR_WIDTH 12

// colors
int BGCOLOR=BLACK;

//button sleep duration
#define BUTTON_PRESSED_SLEEP 300

// 1 2 or 3 index for Easy, Medium or Hard
int globalDifficultySetting=0;
char difficultyOptions[][10] = { "Easy","Medium", "Hard"};
const int difficultyColors[]={GREEN, ORANGE, RED};
const int GlobalPaletteDistance=80;

// menu settings
#define menu_start_y 15
#define menu_button_height 25

// longest menu word
#define menu_word_length_limit 20

const char menuOptions[][menu_word_length_limit] = { "Play","Instructions","Difficulty","Leaderboard","Credits"};

// return random int from lower to upper
int getRand(int lower=0, int upper=255){
    return Random.RandInt()%(upper-lower)+lower;
}

// return (r,g,b) parameters as 1 int 24 bit color
int get24BitColor(int r, int g, int b){
     return (r<<16)|(g<<8)|b;
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

// doesn't work smh. supposed to display grid of possible color palette squares
void displayPaletteGrid(int color, int start=0, int step=10){
    int distance=0;
    LCD.SetFontColor(color);

    for(int j=0;j<5;j++){
        for(int i=0;i<5;i++){
            displayPossiblePalette(color, distance,i*41,j*41);
            distance+=10;
        }
    }
}

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

    int sleepDurationMS = 20;
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

    int sleepDurationMS = 20;
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
        Sleep(sleepDurationMS);
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

// display "play game" message and wait for "quit" button press
void play(){
    LCD.Clear();
    writeCenteredAt("Play Game Here",XLIM/3,20);
    waitForButton("Quit", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT);
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
            waitForButton("Next",buttonX, buttonY);
            Sleep(BUTTON_PRESSED_SLEEP);
            LCD.Clear();
            yPos=startY+lineSpacing*3;
        }
    }

    // writeCenteredAt("Instructions Here",XLIM/3,20);
    waitForButton("Quit", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT);
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
    waitForButton("Quit", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT);
    return;
}

// display "credits" message and wait for "quit" button press
void displayCredits(){
    LCD.Clear();
    writeCenteredAt("Credits Here",XLIM/3,20);
    waitForButton("Quit", XLIM-CHAR_WIDTH*5, YLIM-CHAR_HEIGHT);
    return;
}

int main() {
    // Clear background
    LCD.SetBackgroundColor(BLACK);
    // LCD.Clear();

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
        }

    }
    return 0;
}
