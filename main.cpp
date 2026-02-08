#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <string.h>

using namespace std;

bool rainActive = true;  // start with rain
bool snowActive = false; // start without snow (for Level 4)

// Global variables for bird movement
float bird1_x = -0.5f;
float bird2_x = 0.0f;
float bird3_x = 0.5f;
float bird4_x = -0.3f;
float bird5_x = 0.2f;
float bird6_x = 0.7f;
float bird7_x = -0.6f;
float bird8_x = 0.4f;

float birdSpeed = 0.0045f; // bird moving speed

/* ================= MENU SYSTEM ================= */
bool showMenu = true;  // Show menu at start
int menuState = 0;     // 0 = main menu, 1 = level selection
int selectedLevel = 1; // Currently selected level in menu
int hoveredButton = -1; // -1 = none, 0-4 = level buttons, 5 = start game

// Button structure
struct Button {
    float x1, y1, x2, y2;
    int id;
    const char* text;
    bool isHovered;
    bool isSelected;
};

Button levelButtons[5]; // Level 1-5 buttons
Button startButton;     // Start game button
Button backButton;      // Back to main menu button

void initMenuButtons() {
    // Level selection buttons
    for (int i = 0; i < 5; i++) {
        levelButtons[i].x1 = -0.4f;
        levelButtons[i].y1 = 0.5f - i * 0.15f;
        levelButtons[i].x2 = 0.4f;
        levelButtons[i].y2 = 0.4f - i * 0.15f;
        levelButtons[i].id = i + 1;

        // Set level names
        switch(i) {
            case 0: levelButtons[i].text = "LEVEL 1: BEGINNER (RAINY)"; break;
            case 1: levelButtons[i].text = "LEVEL 2: CITY RUSH HOUR"; break;
            case 2: levelButtons[i].text = "LEVEL 3: FOGGY MOUNTAIN"; break;
            case 3: levelButtons[i].text = "LEVEL 4: SNOWY PEAKS"; break;
            case 4: levelButtons[i].text = "LEVEL 5: DESERT HIGHWAY"; break;
        }

        levelButtons[i].isHovered = false;
        levelButtons[i].isSelected = (i == 0); // Level 1 selected by default
    }

    // Start game button
    startButton.x1 = -0.2f;
    startButton.y1 = -0.2f;
    startButton.x2 = 0.2f;
    startButton.y2 = -0.1f;
    startButton.id = 5;
    startButton.text = "START GAME";
    startButton.isHovered = false;
    startButton.isSelected = false;

    // Back button (for level selection menu)
    backButton.x1 = -0.2f;
    backButton.y1 = -0.4f;
    backButton.x2 = 0.2f;
    backButton.y2 = -0.3f;
    backButton.id = 6;
    backButton.text = "BACK";
    backButton.isHovered = false;
    backButton.isSelected = false;
}

void drawButton(Button btn) {
    // Button background
    if (btn.isSelected) {
        glColor3ub(0, 150, 255); // Blue for selected
    } else if (btn.isHovered) {
        glColor3ub(100, 200, 255); // Light blue for hover
    } else {
        glColor3ub(50, 100, 200); // Normal blue
    }

    glBegin(GL_QUADS);
        glVertex2f(btn.x1, btn.y1);
        glVertex2f(btn.x2, btn.y1);
        glVertex2f(btn.x2, btn.y2);
        glVertex2f(btn.x1, btn.y2);
    glEnd();

    // Button border
    glColor3ub(255, 255, 255);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
        glVertex2f(btn.x1, btn.y1);
        glVertex2f(btn.x2, btn.y1);
        glVertex2f(btn.x2, btn.y2);
        glVertex2f(btn.x1, btn.y2);
    glEnd();

    // Button text
    glColor3ub(255, 255, 255);
    int textLen = strlen(btn.text);
    float textWidth = textLen * 0.015f; // Approximate width
    float xPos = (btn.x1 + btn.x2) / 2 - textWidth / 2;
    float yPos = (btn.y1 + btn.y2) / 2 - 0.01f;

    glRasterPos2f(xPos, yPos);
    for (int i = 0; btn.text[i] != 0; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, btn.text[i]);
    }
}

void drawMainMenu() {
    // Semi-transparent background
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.1f, 0.1f, 0.2f, 0.9f);
    glBegin(GL_QUADS);
        glVertex2f(-1, -1);
        glVertex2f(1, -1);
        glVertex2f(1, 1);
        glVertex2f(-1, 1);
    glEnd();
    glDisable(GL_BLEND);

    // Game title
    glColor3ub(255, 255, 0);
    glRasterPos2f(-0.4f, 0.8f);
    const char* title = "5-LEVEL RACING CHALLENGE";
    for(int i=0; title[i]!=0; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, title[i]);

    // Instructions
    glColor3ub(200, 200, 255);
    glRasterPos2f(-0.45f, 0.65f);
    const char* instr = "Press 1-5 to select level, then S to start!";
    for(int i=0; instr[i]!=0; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, instr[i]);

    // Level selection buttons
    for (int i = 0; i < 5; i++) {
        drawButton(levelButtons[i]);
    }

    // Start button
    drawButton(startButton);
}

void drawLevelSelectionMenu() {
    // Semi-transparent background
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.1f, 0.1f, 0.2f, 0.9f);
    glBegin(GL_QUADS);
        glVertex2f(-1, -1);
        glVertex2f(1, -1);
        glVertex2f(1, 1);
        glVertex2f(-1, 1);
    glEnd();
    glDisable(GL_BLEND);

    // Menu title
    glColor3ub(255, 255, 0);
    glRasterPos2f(-0.3f, 0.8f);
    const char* title = "SELECT LEVEL";
    for(int i=0; title[i]!=0; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, title[i]);

    // Currently selected level info
    glColor3ub(200, 255, 200);
    glRasterPos2f(-0.4f, 0.7f);
    char levelInfo[100];
    sprintf(levelInfo, "Selected: Level %d", selectedLevel);
    for(int i=0; levelInfo[i]!=0; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, levelInfo[i]);

    // Level descriptions
    const char* descriptions[5] = {
        "Beginner level with rain effects",
        "City traffic with faster opponents",
        "Foggy mountain with reduced visibility",
        "Snowy terrain with slippery roads and snowfall",
        "Desert highway with dust storms"
    };

    glColor3ub(255, 200, 200);
    glRasterPos2f(-0.4f, 0.6f);
    for(int i=0; descriptions[selectedLevel-1][i]!=0; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, descriptions[selectedLevel-1][i]);

    // Control instructions
    glColor3ub(200, 200, 255);
    glRasterPos2f(-0.4f, 0.4f);
    const char* controls = "Controls: W=Up, S=Down, A=Left, D=Right, N=Night, H=Heavy Rain";
    for(int i=0; controls[i]!=0; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, controls[i]);

    // Start button
    startButton.x1 = -0.2f;
    startButton.y1 = 0.0f;
    startButton.x2 = 0.2f;
    startButton.y2 = 0.1f;
    startButton.text = "START LEVEL";
    drawButton(startButton);

    // Back button
    drawButton(backButton);
}

/* ================= GAME LEVELS ================= */
int currentLevel = 1;
const int MAX_LEVEL = 5; // Increased to 5 levels
int scoreToNextLevel = 100; // Score needed to reach next level
bool levelTransition = false;
int transitionTimer = 0;
const int TRANSITION_DURATION = 60; // 2 seconds at 30fps

/* ================= ROAD & SKY ================= */
const float ROAD_LEFT   = -0.4f;
const float ROAD_RIGHT  =  0.4f;
const float ROAD_BOTTOM = -1.0f;
const float ROAD_TOP    =  0.5f;
const float SKY_TOP     =  1.0f;

/* ================= TRAIN ================= */
float trainY = ROAD_TOP;
float trainSpeed = 0.004f;

/* ================= LANES ================= */
float laneX[3] = {-0.3f, 0.0f, 0.3f};
int currentLane = 1;
float playerY = -0.7f;
float playerSpeed = 0.05f;
const float PLAYER_TOP_LIMIT = ROAD_TOP - 0.15f;
const float PLAYER_BOTTOM_LIMIT = ROAD_BOTTOM + 0.15f;

/* ================= GAME STATE ================= */
bool running = false;
bool gameStarted = false;
int scoreCounter = 0;
bool nightMode = false;  // night/day toggle
bool heavyRain = false;  // heavy rain toggle
bool fogActive = false;  // fog effect for level 3
bool desertWind = false; // dust storm for level 5

/* ================= LEVEL PROPERTIES ================= */
int currentOpponents = 3;  // Start with 3 opponents
float currentOpponentSpeed = 0.007f;
float currentTrainSpeed = 0.004f;
float currentBirdSpeed = 0.0045f;
float currentPlayerSpeed = 0.05f;
bool narrowRoad = false; // For level 5

/* ================= RANDOM ================= */
unsigned int seedVal;

int simpleRand(){
    seedVal = seedVal * 1103515245 + 12345;
    return (seedVal / 65536) % 32768;
}

/* ================= BASIC RECT ================= */
void drawRectCol(float x1,float y1,float x2,float y2,
                 unsigned char r,unsigned char g,unsigned char b) {
    glColor3ub(r,g,b);
    glBegin(GL_QUADS);
        glVertex2f(x1,y1); glVertex2f(x2,y1);
        glVertex2f(x2,y2); glVertex2f(x1,y2);
    glEnd();
}

/* ================= HELPER SHAPES ================= */
void drawCircleApprox(float cx, float cy, float radius){
    float dx[12] = {1, 0.866f, 0.5f, 0, -0.5f, -0.866f, -1, -0.866f, -0.5f, 0, 0.5f, 0.866f};
    float dy[12] = {0, 0.5f, 0.866f, 1, 0.866f, 0.5f, 0, -0.5f, -0.866f, -1, -0.866f, -0.5f};
    glBegin(GL_TRIANGLE_FAN);
    for(int i=0;i<12;i++)
        glVertex2f(cx + dx[i]*radius, cy + dy[i]*radius);
    glVertex2f(cx + dx[0]*radius, cy + dy[0]*radius);
    glEnd();
}

void drawCloudApprox(float cx, float cy, float scale){
    if(nightMode || currentLevel == 3 || currentLevel == 5)
        glColor3ub(80, 80, 80); // dark clouds
    else if(currentLevel == 4)
        glColor3ub(200, 200, 200); // light gray for snow level
    else
        glColor3ub(255,255,255); // white clouds

    drawCircleApprox(cx, cy, 0.05f*scale);
    drawCircleApprox(cx+0.03f*scale, cy+0.02f*scale, 0.05f*scale);
    drawCircleApprox(cx+0.06f*scale, cy, 0.05f*scale);
    drawCircleApprox(cx+0.03f*scale, cy-0.02f*scale, 0.05f*scale);
}

/* ================= STARS ================= */
const int N_STARS = 100;
float starX[N_STARS];
float starY[N_STARS];
float starSize[N_STARS];

void initStars() {
    for (int i = 0; i < N_STARS; i++) {
        starX[i] = -1.0f + (simpleRand() % 2000) / 1000.0f;
        starY[i] = 0.5f + (simpleRand() % 1000) / 1000.0f;
        starSize[i] = 0.002f + (simpleRand() % 10) / 5000.0f;
    }
}

/* ================= SNOWFLAKES ================= */
const int N_SNOW = 300; // Increased number of snowflakes for better effect
float snowX[N_SNOW];
float snowY[N_SNOW];
float snowSpeed[N_SNOW];
float snowDrift[N_SNOW]; // For sideways drifting effect

void initSnow() {
    for (int i = 0; i < N_SNOW; i++) {
        snowX[i] = -1.0f + (simpleRand() % 2000) / 1000.0f;
        snowY[i] = 1.0f + (simpleRand() % 1000) / 1000.0f; // Start from top
        snowSpeed[i] = 0.003f + (simpleRand() % 15) / 5000.0f;
        snowDrift[i] = (simpleRand() % 100) / 5000.0f - 0.01f; // Small sideways drift
    }
}

/* ================= FOG PARTICLES ================= */
const int N_FOG = 80;
float fogX[N_FOG];
float fogY[N_FOG];
float fogAlpha[N_FOG];

void initFog() {
    for (int i = 0; i < N_FOG; i++) {
        fogX[i] = -1.0f + (simpleRand() % 2000) / 1000.0f;
        fogY[i] = -1.0f + (simpleRand() % 2000) / 1000.0f;
        fogAlpha[i] = 0.1f + (simpleRand() % 10) / 50.0f;
    }
}

/* ================= DUST PARTICLES (Desert Level) ================= */
const int N_DUST = 60;
float dustX[N_DUST];
float dustY[N_DUST];
float dustSize[N_DUST];

void initDust() {
    for (int i = 0; i < N_DUST; i++) {
        dustX[i] = -1.0f + (simpleRand() % 2000) / 1000.0f;
        dustY[i] = -0.5f + (simpleRand() % 1500) / 1000.0f;
        dustSize[i] = 0.005f + (simpleRand() % 15) / 1000.0f;
    }
}

/* ================= TREES, LAMPS & HOUSES ================= */
float treeOffsetY = 0.0f;

void drawTreeShadow(float x, float y, float scale){
    glColor3ub(70, 70, 70);
    glBegin(GL_POLYGON);
        glVertex2f(x + 0.01f*scale, y);
        glVertex2f(x + 0.06f*scale, y - 0.02f*scale);
        glVertex2f(x + 0.15f*scale, y - 0.09f*scale);
        glVertex2f(x - 0.04f*scale, y - 0.01f*scale);
    glEnd();

    drawCircleApprox(x + 0.10f*scale, y - 0.06f*scale, 0.05f*scale);
    drawCircleApprox(x + 0.08f*scale, y - 0.08f*scale, 0.05f*scale);
    drawCircleApprox(x + 0.09f*scale, y - 0.009f*scale, 0.05f*scale);
}

void drawApple(float x, float y, float scale) {
    if (nightMode || currentLevel == 3)
        glColor3ub(180, 0, 0);
    else if(currentLevel == 4)
        glColor3ub(200, 50, 50); // Darker red in snow
    else if(currentLevel == 5)
        glColor3ub(160, 0, 0); // Darker for desert
    else
        glColor3ub(255, 0, 0);
    drawCircleApprox(x, y, 0.017f*scale);
}

void drawTree(float x, float y, float scale){
    // Different tree types for different levels
    if(currentLevel == 4) { // Snow level - pine trees with snow
        drawRectCol(x - 0.01f*scale, y, x + 0.01f*scale, y + 0.08f*scale, 101, 67, 33);

        // Snow on branches
        glColor3ub(255, 255, 255);
        drawCircleApprox(x, y + 0.12f*scale, 0.03f*scale);
        drawCircleApprox(x - 0.02f*scale, y + 0.09f*scale, 0.025f*scale);
        drawCircleApprox(x + 0.02f*scale, y + 0.09f*scale, 0.025f*scale);

        // Tree color
        if(nightMode) glColor3ub(0, 80, 0);
        else glColor3ub(0, 120, 0);

        drawCircleApprox(x, y + 0.10f*scale, 0.04f*scale);
        drawCircleApprox(x - 0.03f*scale, y + 0.09f*scale, 0.035f*scale);
        drawCircleApprox(x + 0.03f*scale, y + 0.09f*scale, 0.035f*scale);

    } else if(currentLevel == 5) { // Desert level - cactus
        glColor3ub(0, 100, 0);
        drawRectCol(x - 0.015f*scale, y, x + 0.015f*scale, y + 0.12f*scale, 0, 100, 0);
        drawRectCol(x - 0.01f*scale, y + 0.04f*scale, x + 0.04f*scale, y + 0.08f*scale, 0, 100, 0);
        drawRectCol(x - 0.04f*scale, y + 0.06f*scale, x + 0.01f*scale, y + 0.10f*scale, 0, 100, 0);

    } else { // Normal trees for other levels
        drawTreeShadow(x, y, scale);
        drawRectCol(x - 0.01f*scale, y, x + 0.01f*scale, y + 0.08f*scale, 101, 67, 33);

        if(nightMode || currentLevel == 3) glColor3ub(0, 120, 0);
        else glColor3ub(0, 180, 0);

        drawCircleApprox(x, y + 0.10f*scale, 0.04f*scale);
        drawCircleApprox(x - 0.03f*scale, y + 0.09f*scale, 0.035f*scale);
        drawCircleApprox(x + 0.03f*scale, y + 0.09f*scale, 0.035f*scale);

        // Only draw apples in early levels
        if(currentLevel <= 2) {
            drawApple(x + 0.05f*scale, y + 0.11f*scale, scale);
            drawApple(x - 0.05f*scale, y + 0.11f*scale, scale);
            drawApple(x + 0.03f*scale, y + 0.05f*scale, scale);
        }
    }
}

void drawLamp(float x, float y, float scale){
    glColor3ub(100, 100, 100);
    drawRectCol(x - 0.005f*scale, y, x + 0.005f*scale, y + 0.1f*scale, 100, 100, 100);

    glColor3ub(255, 255, 150);
    drawRectCol(x - 0.025f*scale, y + 0.1f*scale, x + 0.025f*scale, y + 0.2f*scale, 255, 255, 150);

    glColor3ub(255, 255, 255);
    drawRectCol(x - 0.02f*scale, y + 0.12f*scale, x + 0.02f*scale, y + 0.18f*scale, 255, 255, 255);

    if (nightMode || currentLevel == 3) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 0.6f, 0.6f);
        drawRectCol(x - 0.03f*scale, y + 0.1f*scale, x + 0.03f*scale, y + 0.2f*scale, 255, 255, 150);
        glDisable(GL_BLEND);
    }
}

void drawHouse(float x, float y, float width, float height){
    // Different houses for different levels
    if(currentLevel == 4) { // Snow level - cabin with snow
        drawRectCol(x, y, x + width * 1.5f, y + height * 1.5f, 139, 90, 43); // Wooden cabin

        // Snow on roof
        glColor3ub(255, 255, 255);
        glBegin(GL_TRIANGLES);
            glVertex2f(x - 0.01f * 1.5f, y + height * 1.5f);
            glVertex2f(x + width * 1.5f + 0.01f, y + height * 1.5f);
            glVertex2f(x + width * 1.5f / 2, y + height * 1.5f + 0.05f);
        glEnd();

    } else if(currentLevel == 5) { // Desert level - desert house
        drawRectCol(x, y, x + width * 1.5f, y + height * 1.5f, 210, 180, 140); // Sand color

        glColor3ub(150,50,50);
        glBegin(GL_TRIANGLES);
            glVertex2f(x - 0.01f * 1.5f, y + height * 1.5f);
            glVertex2f(x + width * 1.5f + 0.01f, y + height * 1.5f);
            glVertex2f(x + width * 1.5f / 2, y + height * 1.5f + 0.05f);
        glEnd();

    } else if(currentLevel == 3) { // Foggy level - dark house
        drawRectCol(x, y, x + width * 1.5f, y + height * 1.5f, 80, 80, 100);
         glColor3ub(150,50,50);
        glBegin(GL_TRIANGLES);
            glVertex2f(x - 0.01f * 1.5f, y + height * 1.5f);
            glVertex2f(x + width * 1.5f + 0.01f, y + height * 1.5f);
            glVertex2f(x + width * 1.5f / 2, y + height * 1.5f + 0.05f);
        glEnd();
    } else {
        if(nightMode)
            drawRectCol(x, y, x + width * 1.5f, y + height * 1.5f, 100,100,120);
        else
            drawRectCol(x, y, x + width * 1.5f, y + height * 1.5f, 200,180,150);

        glColor3ub(150,50,50);
        glBegin(GL_TRIANGLES);
            glVertex2f(x - 0.01f * 1.5f, y + height * 1.5f);
            glVertex2f(x + width * 1.5f + 0.01f, y + height * 1.5f);
            glVertex2f(x + width * 1.5f / 2, y + height * 1.5f + 0.05f);
        glEnd();
    }

    float winW = width * 0.2f * 1.5f, winH = height * 0.25f * 1.5f;

    if(nightMode || currentLevel == 3) {
        drawRectCol(x + 0.2f * 1.5f, y + 0.75f * 1.5f, x + 0.2f * 1.5f + winW, y + 0.75f * 1.5f + winH, 255,255,150);
        drawRectCol(x + 1.0f * 1.5f, y + 0.75f * 1.5f, x + 1.0f * 1.5f + winW, y + 0.75f * 1.5f + winH, 255,255,150);
    }
    else {
        drawRectCol(x + 0.2f * 1.5f, y + 0.75f * 1.5f, x + 0.2f * 1.5f + winW, y + 0.75f * 1.5f + winH, 255,255,255);
        drawRectCol(x + 1.0f * 1.5f, y + 0.75f * 1.5f, x + 1.0f * 1.5f + winW, y + 0.75f * 1.5f + winH, 255,255,255);
    }

    float doorW = width * 0.3f * 1.5f, doorH = height * 0.5f * 1.5f;
    if(nightMode || currentLevel == 3)
        drawRectCol(x + (width * 1.5f / 2) - (doorW / 2), y, x + (width * 1.5f / 2) + (doorW / 2), y + doorH, 139,69,19);
    else
        drawRectCol(x + (width * 1.5f / 2) - (doorW / 2), y, x + (width * 1.5f / 2) + (doorW / 2), y + doorH, 255,255,255);
}

void drawTreesAndHouses(){
    float y = ROAD_BOTTOM + 0.02f + treeOffsetY;
    while(y < ROAD_TOP){
        // Draw different scenery based on level
        if(currentLevel == 5) { // Desert level - fewer trees, more rocks
            // Draw cactus instead of trees
            if(simpleRand() % 3 == 0) {
                drawTree(ROAD_LEFT - 0.23f, y, 1.0f);
            }
            if(simpleRand() % 3 == 0) {
                drawTree(ROAD_RIGHT + 0.16f, y, 1.0f);
            }

            // Draw desert rocks
            glColor3ub(120, 120, 120);
            drawCircleApprox(ROAD_RIGHT + 0.3f, y - 0.02f, 0.03f);
            drawCircleApprox(ROAD_LEFT - 0.35f, y + 0.03f, 0.025f);

        } else {
            // Normal scenery for other levels
            drawTree(ROAD_LEFT - 0.23f, y, 1.0f);
            drawLamp(ROAD_LEFT - 0.04f, y, 1.0f);
            drawTree(ROAD_RIGHT + 0.16f, y, 1.0f);
            drawLamp(ROAD_RIGHT + 0.05f, y, 1.0f);

            drawHouse(ROAD_RIGHT + 0.5f, y - 0.05f, 0.05f, 0.08f);
            drawHouse(ROAD_RIGHT + 0.35f, y - 0.05f, 0.04f, 0.07f);
            drawHouse(ROAD_RIGHT + 0.42f, y - 0.05f, 0.05f, 0.06f);
        }

        y += 0.25f;
    }
}

/* ================= OPPONENTS ================= */
const int MAX_OPPONENTS = 8;
float opY[MAX_OPPONENTS];
int opLane[MAX_OPPONENTS];
int opColor[MAX_OPPONENTS];
float opSpeed[MAX_OPPONENTS]; // Individual speeds for harder levels

/* ================= CAR SCALE ================= */
const float CAR_SCALE_X = 0.5f;
const float CAR_SCALE_Y = 0.25f;
const float CAR_HALF_HEIGHT = 0.3f * CAR_SCALE_Y;
const float MIN_DIST = 0.35f;

/* ================= CAR SHAPE ================= */
void drawCar(unsigned char bodyR, unsigned char bodyG, unsigned char bodyB){
    glColor3ub(bodyR, bodyG, bodyB);
    glBegin(GL_POLYGON);
        glVertex2f(-0.05f,-0.3f);
        glVertex2f(0.05f,-0.3f);
        glVertex2f(0.09f,-0.285f);
        glVertex2f(0.1f,-0.21f);
        glVertex2f(0.1f,-0.17f);
        glVertex2f(0.09f,-0.1f);
        glVertex2f(0.09f,0.1f);
        glVertex2f(0.1f,0.17f);
        glVertex2f(0.1f,0.21f);
        glVertex2f(0.09f,0.285f);
        glVertex2f(0.05f,0.3f);
        glVertex2f(-0.05f,0.3f);
        glVertex2f(-0.09f,0.285f);
        glVertex2f(-0.1f,0.21f);
        glVertex2f(-0.1f,0.17f);
        glVertex2f(-0.09f,0.1f);
        glVertex2f(-0.09f,-0.1f);
        glVertex2f(-0.1f,-0.17f);
        glVertex2f(-0.1f,-0.21f);
        glVertex2f(-0.095f,-0.285f);
    glEnd();

    glColor3ub(0,0,0);
    glBegin(GL_POLYGON);
        glVertex2f(0.025f,-0.22f);
        glVertex2f(0.05f,-0.17f);
        glVertex2f(-0.05f,-0.17f);
        glVertex2f(-0.025f,-0.22f);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(0.05f,0.05f);
        glVertex2f(0.07f,0.15f);
        glVertex2f(-0.07f,0.15f);
        glVertex2f(-0.05f,0.05f);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(0.075f,-0.18f);
        glVertex2f(0.075f,0.1f);
        glVertex2f(0.053f,0.01f);
        glVertex2f(0.053f,-0.13f);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.075f,-0.18f);
        glVertex2f(-0.075f,0.1f);
        glVertex2f(-0.053f,0.01f);
        glVertex2f(-0.053f,-0.13f);
    glEnd();

    glColor3ub(210,214,0);
    glBegin(GL_POLYGON);
        glVertex2f(0.05f,0.27f);
        glVertex2f(0.08f,0.26f);
        glVertex2f(0.09f,0.285f);
        glVertex2f(0.05f,0.3f);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.05f,0.27f);
        glVertex2f(-0.08f,0.26f);
        glVertex2f(-0.09f,0.285f);
        glVertex2f(-0.05f,0.3f);
    glEnd();
}

/* ================= PLAYER & OPPONENT ================= */
void drawPlayer(){
    glPushMatrix();
        glTranslatef(laneX[currentLane], playerY, 0.0f);
        glScalef(CAR_SCALE_X, CAR_SCALE_Y, 1.0f);
        drawCar(214,0,0);
    glPopMatrix();
}

void drawOpponent(int i){
    unsigned char r=255,g=255,b=255;
    // Different color schemes for different levels
    if(currentLevel == 1) {
        if(opColor[i]==1){ r=51; g=51; b=255; }
        if(opColor[i]==2){ r=0; g=255; b=0; }
        if(opColor[i]==3){ r=255; g=153; b=0; }
    } else if(currentLevel == 2) {
        if(opColor[i]==1){ r=51; g=51; b=255; }
        if(opColor[i]==2){ r=255; g=0; b=0; }
        if(opColor[i]==3){ r=255; g=0; b=255; }
        if(opColor[i]==4){ r=0; g=255; b=255; }
    } else if(currentLevel == 3) {
        if(opColor[i]==1){ r=100; g=100; b=100; }
        if(opColor[i]==2){ r=150; g=150; b=150; }
        if(opColor[i]==3){ r=200; g=200; b=200; }
        if(opColor[i]==4){ r=100; g=0; b=0; }
    } else if(currentLevel == 4) {
        if(opColor[i]==1){ r=200; g=200; b=255; }
        if(opColor[i]==2){ r=150; g=150; b=255; }
        if(opColor[i]==3){ r=100; g=100; b=255; }
        if(opColor[i]==4){ r=50; g=50; b=200; }
    } else if(currentLevel == 5) {
        if(opColor[i]==1){ r=210; g=105; b=30; } // Chocolate
        if(opColor[i]==2){ r=139; g=69; b=19; }  // Saddle Brown
        if(opColor[i]==3){ r=160; g=82; b=45; }  // Sienna
        if(opColor[i]==4){ r=205; g=133; b=63; } // Peru
        if(opColor[i]==5){ r=188; g=143; b=143; } // Rosy Brown
    }

    glPushMatrix();
        glTranslatef(laneX[opLane[i]], opY[i], 0);
        glRotatef(180,0,0,1);
        glScalef(CAR_SCALE_X, CAR_SCALE_Y, 1.0f);
        drawCar(r,g,b);
    glPopMatrix();
}

/* ================= OPPONENT LOGIC ================= */
void initOpponents(){
    for(int i=0;i<currentOpponents;i++){
        opY[i] = ROAD_TOP - CAR_HALF_HEIGHT - i*MIN_DIST;
        opLane[i] = simpleRand()%3;
        opColor[i] = (simpleRand()%3) + 1;
        opSpeed[i] = currentOpponentSpeed;

        // Different opponent behavior for higher levels
        if(currentLevel >= 2) {
            opColor[i] = (simpleRand()%4) + 1;
            if(currentLevel >= 3) {
                opColor[i] = (simpleRand()%4) + 1;
                // Random speed variations for fog level
                opSpeed[i] = currentOpponentSpeed * (0.8f + (simpleRand()%40)/100.0f);
            }
            if(currentLevel >= 4) {
                opColor[i] = (simpleRand()%4) + 1;
                // Slower but more opponents for snow level
                opSpeed[i] = currentOpponentSpeed * (0.7f + (simpleRand()%30)/100.0f);
            }
            if(currentLevel >= 5) {
                opColor[i] = (simpleRand()%5) + 1;
                // Fast but predictable patterns for desert
                opSpeed[i] = currentOpponentSpeed * (1.0f + (simpleRand()%20)/100.0f);
            }
        }
    }
}

void resetOpponent(int i){
    opY[i] = ROAD_TOP - CAR_HALF_HEIGHT;
    opLane[i] = simpleRand()%3;
    opColor[i] = (simpleRand()%3) + 1;
    opSpeed[i] = currentOpponentSpeed;

    if(currentLevel >= 2) {
        opColor[i] = (simpleRand()%4) + 1;
        if(currentLevel >= 3) {
            opColor[i] = (simpleRand()%4) + 1;
            opSpeed[i] = currentOpponentSpeed * (0.8f + (simpleRand()%40)/100.0f);
        }
        if(currentLevel >= 4) {
            opColor[i] = (simpleRand()%4) + 1;
            opSpeed[i] = currentOpponentSpeed * (0.7f + (simpleRand()%30)/100.0f);
        }
        if(currentLevel >= 5) {
            opColor[i] = (simpleRand()%5) + 1;
            opSpeed[i] = currentOpponentSpeed * (1.0f + (simpleRand()%20)/100.0f);
        }
    }

    scoreCounter += 10 * currentLevel;
}

/* ================= LEVEL MANAGEMENT ================= */
void advanceToNextLevel() {
    if(currentLevel < MAX_LEVEL) {
        currentLevel++;
        levelTransition = true;
        transitionTimer = TRANSITION_DURATION;

        // Apply level-specific effects
        switch(currentLevel) {
            case 2: // City Rush Hour
                currentOpponents = 4;
                currentOpponentSpeed = 0.009f;
                currentTrainSpeed = 0.005f;
                currentBirdSpeed = 0.006f;
                currentPlayerSpeed = 0.045f;
                scoreToNextLevel = 250;
                nightMode = false;
                rainActive = false;
                snowActive = false;
                break;

            case 3: // Foggy Mountain Pass
                currentOpponents = 4;
                currentOpponentSpeed = 0.008f; // Slower but variable
                currentTrainSpeed = 0.004f;
                currentBirdSpeed = 0.003f; // Birds fly slower in fog
                currentPlayerSpeed = 0.04f;
                scoreToNextLevel = 450;
                nightMode = true;
                fogActive = true;
                rainActive = false;
                snowActive = false;
                initFog();
                break;

            case 4: // Snowy Mountain
                currentOpponents = 4;
                currentOpponentSpeed = 0.007f; // Slower due to snow
                currentTrainSpeed = 0.003f;
                currentBirdSpeed = 0.002f; // Few birds in snow
                currentPlayerSpeed = 0.035f;
                scoreToNextLevel = 700;
                nightMode = false;
                snowActive = true; // Activate snowfall
                rainActive = false;
                initSnow();
                break;

            case 5: // Desert Highway (Final Level)
                currentOpponents = 4;
                currentOpponentSpeed = 0.07f; // Fast in desert
                currentTrainSpeed = 0.006f;
                currentBirdSpeed = 0.009f;
                currentPlayerSpeed = 0.05f;
                scoreToNextLevel = 1000; // High score for final level
                nightMode = false;
                desertWind = true;
                narrowRoad = false;
                rainActive = false;
                snowActive = false;
                initDust();
                break;
        }

        initOpponents();
    }
}

void drawTransitionEffect() {
    if(transitionTimer > 0) {
        float alpha = (float)transitionTimer / TRANSITION_DURATION;
        if(alpha > 0.5) alpha = 1.0 - alpha;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Different transition colors for different levels
        switch(currentLevel) {
            case 2:
                glColor4f(0.0f, 0.5f, 1.0f, alpha * 0.6f); // Blue for city
                break;
            case 3:
                glColor4f(0.5f, 0.5f, 0.5f, alpha * 0.7f); // Gray for fog
                break;
            case 4:
                glColor4f(1.0f, 1.0f, 1.0f, alpha * 0.8f); // White for snow
                break;
            case 5:
                glColor4f(1.0f, 0.65f, 0.0f, alpha * 0.7f); // Orange for desert
                break;
            default:
                glColor4f(1.0f, 1.0f, 0.5f, alpha * 0.7f);
        }

        drawRectCol(-1, -1, 1, 1, 255, 255, 255);

        // Draw level up text with level-specific message
        char levelText[100];
        char levelName[50];

        switch(currentLevel) {
            case 1: strcpy(levelName, "BEGINNER"); break;
            case 2: strcpy(levelName, "CITY RUSH HOUR"); break;
            case 3: strcpy(levelName, "FOGGY MOUNTAIN"); break;
            case 4: strcpy(levelName, "SNOWY PEAKS"); break;
            case 5: strcpy(levelName, "DESERT HIGHWAY"); break;
            default: strcpy(levelName, "LEVEL UP!");
        }

        sprintf(levelText, "LEVEL %d: %s", currentLevel, levelName);

        glColor3f(1.0f, 1.0f, 0.0f);
        glRasterPos2f(-0.25f, 0.0f);
        for(int i=0; levelText[i]!=0; i++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, levelText[i]);

        glDisable(GL_BLEND);
        transitionTimer--;
    } else {
        levelTransition = false;
    }
}

/* ================= COLLISION ================= */
bool checkCollision(){
    for(int i=0;i<currentOpponents;i++){
        if(opLane[i]==currentLane){
            float dy = playerY - opY[i];
            if(dy<0) dy=-dy;
            // Different collision distances for different levels
            float collisionDist = 0.25f;
            if(currentLevel == 3) collisionDist = 0.28f; // Harder to see in fog
            if(currentLevel == 5 && narrowRoad) collisionDist = 0.22f; // Narrow road
            if(dy < collisionDist) return true;
        }
    }
    return false;
}

/* ================= TEXT ================= */
void drawText(float x,float y,const char* s){
    glColor3f(1,1,1);
    glRasterPos2f(x,y);
    for(int i=0;s[i]!=0;i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s[i]);
}

/* ================= WEATHER EFFECTS ================= */
/* ================= RAIN ================= */
const int N_RAIN = 200;
float rainX[N_RAIN];
float rainY[N_RAIN];
float rainSpeed = 0.04f;

void initRain(){
    for(int i=0;i<N_RAIN;i++){
        rainX[i] = -1.0f + (simpleRand()%2000)/1000.0f;
        rainY[i] = -1.0f + (simpleRand()%2000)/1000.0f;
    }
}

void drawRain(){
    if(!rainActive) return;

    glColor3ub(180, 200, 255);
    glLineWidth(1);
    glBegin(GL_LINES);
    for(int i=0;i<N_RAIN;i++){
        glVertex2f(rainX[i], rainY[i]);
        glVertex2f(rainX[i], rainY[i]-0.05f);
    }
    glEnd();
}

void updateRain(){
    for(int i=0;i<N_RAIN;i++){
        rainY[i] -= rainSpeed * (heavyRain ? 3.0f : 1.0f);
        if(rainY[i] < -1.0f){
            rainY[i] = 1.0f;
            rainX[i] = -1.0f + (simpleRand()%2000)/1000.0f;
        }
    }
}

/* ================= SNOWFALL ================= */
void drawSnowfall() {
    if(!snowActive) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 0.9f); // White snowflakes

    // Draw snowflakes as small circles
    for(int i = 0; i < N_SNOW; i++) {
        // Draw snowflake as a small circle
        drawCircleApprox(snowX[i], snowY[i], 0.005f);

        // Add some variation - some snowflakes as points for performance
        if(i % 3 == 0) {
            glPointSize(2.0f);
            glBegin(GL_POINTS);
            glVertex2f(snowX[i], snowY[i]);
            glEnd();
        }
    }
    glDisable(GL_BLEND);
}

void updateSnowfall() {
    if(!snowActive) return;

    for(int i = 0; i < N_SNOW; i++) {
        // Move snowflake downward
        snowY[i] -= snowSpeed[i];

        // Add sideways drift for realistic effect
        snowX[i] += snowDrift[i];

        // If snowflake goes below bottom, reset it to top
        if(snowY[i] < -1.0f) {
            snowY[i] = 1.0f + (simpleRand() % 500) / 1000.0f;
            snowX[i] = -1.0f + (simpleRand() % 2000) / 1000.0f;
            snowSpeed[i] = 0.002f + (simpleRand() % 15) / 5000.0f;
            snowDrift[i] = (simpleRand() % 100) / 5000.0f - 0.01f;
        }

        // Wrap around horizontally
        if(snowX[i] < -1.2f) snowX[i] = 1.2f;
        if(snowX[i] > 1.2f) snowX[i] = -1.2f;
    }
}

/* ================= FOG ================= */
void drawFog(){
    if(!fogActive) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for(int i=0;i<N_FOG;i++){
        glColor4f(0.7f, 0.7f, 0.7f, fogAlpha[i]);
        drawCircleApprox(fogX[i], fogY[i], 0.05f + fogAlpha[i] * 0.1f);
    }
    glDisable(GL_BLEND);
}

void updateFog(){
    for(int i=0;i<N_FOG;i++){
        fogY[i] -= 0.001f;
        fogX[i] += sin(fogY[i] * 5.0f) * 0.0005f;
        if(fogY[i] < -1.0f){
            fogY[i] = 1.0f;
            fogX[i] = -1.0f + (simpleRand()%2000)/1000.0f;
            fogAlpha[i] = 0.1f + (simpleRand()%10) / 50.0f;
        }
    }
}

/* ================= DUST STORM ================= */
void drawDust(){
    if(!desertWind) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.76f, 0.70f, 0.50f, 0.3f); // Dust color
    for(int i=0;i<N_DUST;i++){
        drawCircleApprox(dustX[i], dustY[i], dustSize[i]);
    }
    glDisable(GL_BLEND);
}

void updateDust(){
    for(int i=0;i<N_DUST;i++){
        dustY[i] -= 0.003f;
        dustX[i] += 0.001f + sin(dustY[i] * 3.0f) * 0.001f;
        if(dustY[i] < -1.0f){
            dustY[i] = 0.5f;
            dustX[i] = -1.0f + (simpleRand()%2000)/1000.0f;
            dustSize[i] = 0.005f + (simpleRand()%15) / 1000.0f;
        }
    }
}

void drawBird(float x, float y, float scale) {
    // Different bird colors for different levels
    if(currentLevel == 3 || currentLevel == 4) {
        glColor3ub(150, 150, 150); // Gray birds in fog/snow
    } else if(currentLevel == 5) {
        glColor3ub(139, 69, 19); // Brown birds in desert
    } else {
        glColor3ub(0, 0, 0); // Black birds normally
    }

    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(-x, -y, 0);

    glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x - 0.1f * scale, y - 0.1f * scale);
        glVertex2f(x, y);
        glVertex2f(x + 0.1f * scale, y - 0.1f * scale);
    glEnd();

    glPopMatrix();
}

/* ================= SKY & FIELD ================= */
void drawSky(){
    // Different sky colors for different levels
    if(currentLevel == 1) {
        if(nightMode)
            drawRectCol(-2, ROAD_TOP, 2, SKY_TOP, 10, 10, 40);
        else
            drawRectCol(-2, ROAD_TOP, 2, SKY_TOP, 102,179,255);
    } else if(currentLevel == 2) { // City - orange sunset
        drawRectCol(-2, ROAD_TOP, 2, SKY_TOP, 255, 153, 51);
    } else if(currentLevel == 3) { // Foggy - gray sky
        drawRectCol(-2, ROAD_TOP, 2, SKY_TOP, 100, 100, 120);
    } else if(currentLevel == 4) { // Snowy - light blue sky
        drawRectCol(-2, ROAD_TOP, 2, SKY_TOP, 173, 216, 230);
    } else if(currentLevel == 5) { // Desert - yellow sky
        drawRectCol(-2, ROAD_TOP, 2, SKY_TOP, 255, 228, 181);
    }

    // Draw sun/moon
    if(currentLevel == 3 || nightMode) {
        glColor3ub(200, 200, 200);
        drawCircleApprox(-0.8f, 0.85f, 0.06f); // Moon in fog/night
    } else if(currentLevel != 3) {
        // Sun position varies by level
        float sunX = -0.8f;
        float sunY = 0.85f;
        unsigned char sunR = 255, sunG = 204, sunB = 0;

        if(currentLevel == 2) {
            sunX = -0.7f; sunY = 0.8f;
            sunR = 255; sunG = 140; sunB = 0; // Orange sun
        } else if(currentLevel == 4) {
            sunX = -0.6f; sunY = 0.75f;
            sunR = 255; sunG = 255; sunB = 200; // Pale sun in snow
        } else if(currentLevel == 5) {
            sunX = -0.9f; sunY = 0.9f;
            sunR = 255; sunG = 165; sunB = 0; // Bright desert sun
        }

        glColor3ub(sunR, sunG, sunB);
        drawCircleApprox(sunX, sunY, 0.08f);
    }

    // Stars only in night/fog levels
    if(nightMode || currentLevel == 3){
        glColor3ub(255, 255, 200);
        for (int i = 0; i < N_STARS; i++) {
            drawCircleApprox(starX[i], starY[i], starSize[i]);
        }
    }

    // Birds with level-based speed and quantity
    if(currentLevel != 3 && currentLevel != 4) { // Fewer birds in fog/snow
        drawBird(bird1_x, 0.85f, 0.2f);
        drawBird(bird2_x, 0.9f, 0.2f);
        if(currentLevel <= 2) {
            drawBird(bird3_x, 0.70f, 0.2f);
            drawBird(bird4_x, 0.75f, 0.2f);
        }
        if(currentLevel == 5) { // More birds in desert
            drawBird(bird5_x, 0.8f, 0.2f);
            drawBird(bird6_x, 0.6f, 0.2f);
        }
    }

    // Clouds vary by level
    if(currentLevel == 1) {
        drawCloudApprox(0.2f, 0.8f, 1.0f);
        drawCloudApprox(0.5f, 0.9f, 1.2f);
    } else if(currentLevel == 2) {
        drawCloudApprox(0.2f, 0.8f, 1.5f); // Big city clouds
        drawCloudApprox(-0.3f, 0.85f, 1.3f);
    } else if(currentLevel == 3) {
        drawCloudApprox(0.0f, 0.8f, 2.0f); // Heavy fog clouds
        drawCloudApprox(0.5f, 0.7f, 1.8f);
    } else if(currentLevel == 4) {
        drawCloudApprox(0.2f, 0.9f, 1.8f); // Snow clouds
        drawCloudApprox(-0.4f, 0.85f, 1.5f);
    } else if(currentLevel == 5) {
        // Few clouds in desert
        if(simpleRand() % 100 < 20) {
            drawCloudApprox(0.7f, 0.9f, 1.0f);
        }
    }
}

void drawField(){
    // Different field colors for different levels
    if(currentLevel == 1) {
        if(nightMode) {
            drawRectCol(-2, ROAD_BOTTOM, 2, ROAD_TOP, 0, 30, 0);
        } else {
            drawRectCol(-2, ROAD_BOTTOM, 2, ROAD_TOP, 34, 139, 34);
        }
    } else if(currentLevel == 2) { // City - darker green
        drawRectCol(-2, ROAD_BOTTOM, 2, ROAD_TOP, 0, 100, 0);
    } else if(currentLevel == 3) { // Foggy - grayish green
        drawRectCol(-2, ROAD_BOTTOM, 2, ROAD_TOP, 60, 80, 60);
    } else if(currentLevel == 4) { // Snowy - white ground with snow patches
        drawRectCol(-2, ROAD_BOTTOM, 2, ROAD_TOP, 240, 240, 240);
        // Draw some snow patches
        glColor3ub(255, 255, 255);
        for(int i=0; i<20; i++) {
            float x = -1.8f + (simpleRand()%1800)/1000.0f;
            float y = ROAD_BOTTOM + 0.1f + (simpleRand()%1200)/1000.0f;
            if(y < ROAD_TOP) {
                drawCircleApprox(x, y, 0.03f + (simpleRand()%10)/200.0f);
            }
        }
    } else if(currentLevel == 5) { // Desert - sand color
        drawRectCol(-2, ROAD_BOTTOM, 2, ROAD_TOP, 244, 164, 96);
        // Draw some desert rocks
        glColor3ub(160, 160, 160);
        for(int i=0; i<15; i++) {
            float x = -1.8f + (simpleRand()%1800)/1000.0f;
            float y = ROAD_BOTTOM + 0.1f + (simpleRand()%1200)/1000.0f;
            if(y < ROAD_TOP) {
                drawCircleApprox(x, y, 0.02f + (simpleRand()%5)/200.0f);
            }
        }
    }
}

/* ================= ROAD ================= */
void drawRoad(){
    // Road color changes with levels
    float roadLeft = ROAD_LEFT;
    float roadRight = ROAD_RIGHT;

    // Narrow road for desert level
    if(narrowRoad && currentLevel == 5) {
        roadLeft = -0.45f;
        roadRight = 0.45f;
    }

    unsigned char roadColorR = 51, roadColorG = 51, roadColorB = 51;

    if(currentLevel == 1) {
        roadColorR = roadColorG = roadColorB = 51;
    } else if(currentLevel == 2) { // City - dark gray
        roadColorR = roadColorG = roadColorB = 40;
    } else if(currentLevel == 3) { // Foggy - lighter gray
        roadColorR = roadColorG = roadColorB = 10;
    } else if(currentLevel == 4) { // Snowy - gray with snow on sides
        roadColorR = roadColorG = roadColorB = 100;
        // Draw snow on road edges
        glColor3ub(255, 255, 255);
        drawRectCol(roadLeft - 0.05f, ROAD_BOTTOM, roadLeft, ROAD_TOP, 255, 255, 255);
        drawRectCol(roadRight, ROAD_BOTTOM, roadRight + 0.05f, ROAD_TOP, 255, 255, 255);
    } else if(currentLevel == 5) { // Desert - light brown
        roadColorR = 160; roadColorG = 140; roadColorB = 100;
    }

    drawRectCol(roadLeft, ROAD_BOTTOM, roadRight, ROAD_TOP, roadColorR, roadColorG, roadColorB);

    // Road markings
    if(currentLevel != 3) { // No markings in heavy fog
        glColor3f(1,1,1);
        glLineWidth(3);
        glBegin(GL_LINES);
            glVertex2f((roadLeft + roadRight)/2 - 0.15f, ROAD_BOTTOM);
            glVertex2f((roadLeft + roadRight)/2 - 0.12f, ROAD_TOP);
            glVertex2f((roadLeft + roadRight)/2 + 0.15f, ROAD_BOTTOM);
            glVertex2f((roadLeft + roadRight)/2 + 0.12f, ROAD_TOP);
        glEnd();

        // Draw dashed line in the middle (if not foggy)
        if(currentLevel != 3) {
            float lineStartY = ROAD_BOTTOM + 0.1f;
            float lineSpacing = 0.1f;

            glBegin(GL_LINES);
            for (float y = lineStartY; y < ROAD_TOP; y += lineSpacing * 2) {
                glVertex2f((roadLeft + roadRight)/2, y);
                glVertex2f((roadLeft + roadRight)/2, y + lineSpacing);
            }
            glEnd();
        }
    }
}

/* ================= TRAIN ================= */
void drawTrainLine(){
    glColor3ub(0,0,0);
    glLineWidth(4);
    glBegin(GL_LINES);
        glVertex2f(-0.87f, ROAD_BOTTOM); glVertex2f(-0.87f, ROAD_TOP);
        glVertex2f(-0.73f, ROAD_BOTTOM); glVertex2f(-0.73f, ROAD_TOP);
    glEnd();
    float y = ROAD_BOTTOM;
    while(y < ROAD_TOP){
        glBegin(GL_LINES);
            glVertex2f(-0.87f,y); glVertex2f(-0.73f,y);
        glEnd();
        y += 0.08f;
    }
}

/* ================= LIGHT CONE ================= */
void drawLightCone(float cx, float cy, float width, float height){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 0.6f, 0.3f);
    glBegin(GL_TRIANGLES);
        glVertex2f(cx - width/2, cy);
        glVertex2f(cx + width/2, cy);
        glVertex2f(cx, cy - height);
    glEnd();
    glDisable(GL_BLEND);
}

/* ================= TRAIN CAR ================= */
void drawTrainCar(float y){
    // Train color changes with levels
    unsigned char trainRed = 200, trainGreen = 0, trainBlue = 0;

    if(currentLevel == 1) {
        trainRed = 200;
    } else if(currentLevel == 2) {
        trainRed = 180; trainGreen = 0; trainBlue = 20; // Dark red
    } else if(currentLevel == 3) {
        trainRed = 150; trainGreen = 150; trainBlue = 150; // Gray train in fog
    } else if(currentLevel == 4) {
        trainRed = 200; trainGreen = 200; trainBlue = 200; // White train in snow
    } else if(currentLevel == 5) {
        trainRed = 160; trainGreen = 120; trainBlue = 80; // Brown train in desert
    }

    drawRectCol(-0.90f, y-0.05f, -0.70f, y+0.05f, trainRed, trainGreen, trainBlue);
    drawRectCol(-0.88f, y-0.08f, -0.72f, y-0.05f, 0,0,0);

    float headY = y-0.07f;
    float headX1 = -0.85f;
    float headX2 = -0.75f;

    if(nightMode || currentLevel == 3)
        glColor3ub(255,255,150);
    else if(currentLevel == 4)
        glColor3ub(255,255,200); // Brighter lights in snow
    else
        glColor3ub(255,255,100);

    drawCircleApprox(headX1, headY, 0.015f);
    drawCircleApprox(headX2, headY, 0.015f);

    if(nightMode || currentLevel == 3){
        drawLightCone(headX1, headY, 0.06f, 0.25f);
        drawLightCone(headX2, headY, 0.06f, 0.25f);
    }
}

void drawTrain(){
    drawTrainLine();
    glPushMatrix();
        glTranslatef(0,trainY,0);
        drawTrainCar(0.0f);
        drawTrainCar(-0.13f);
        drawTrainCar(-0.26f);
        // More train cars in higher levels
        if(currentLevel >= 2) drawTrainCar(-0.39f);
        if(currentLevel >= 3) drawTrainCar(-0.52f);
        if(currentLevel >= 4) drawTrainCar(-0.65f);
        if(currentLevel >= 5) drawTrainCar(-0.78f);
    glPopMatrix();
}

/* ================= MOUSE INPUT ================= */
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Convert mouse coordinates to OpenGL coordinates
        float glX = (x / 500.0f) * 2.0f - 1.0f;  // Window width is 1000
        float glY = 1.0f - (y / 450.0f) * 2.0f;   // Window height is 900

        if (showMenu) {
            if (menuState == 0) { // Main menu
                // Check level buttons
                for (int i = 0; i < 5; i++) {
                    if (glX >= levelButtons[i].x1 && glX <= levelButtons[i].x2 &&
                        glY <= levelButtons[i].y1 && glY >= levelButtons[i].y2) {
                        // Select this level
                        for (int j = 0; j < 5; j++) {
                            levelButtons[j].isSelected = false;
                        }
                        levelButtons[i].isSelected = true;
                        selectedLevel = i + 1;
                        glutPostRedisplay();
                        return;
                    }
                }

                // Check start button
                if (glX >= startButton.x1 && glX <= startButton.x2 &&
                    glY <= startButton.y1 && glY >= startButton.y2) {
                    // Start the game with selected level
                    showMenu = false;
                    gameStarted = true;
                    running = true;
                    currentLevel = selectedLevel;
                    scoreCounter = 0;

                    // Initialize based on selected level
                    switch(selectedLevel) {
                        case 1:
                            currentOpponents = 3;
                            currentOpponentSpeed = 0.007f;
                            currentTrainSpeed = 0.004f;
                            currentBirdSpeed = 0.0045f;
                            currentPlayerSpeed = 0.05f;
                            scoreToNextLevel = 100;
                            nightMode = false;
                            fogActive = false;
                            snowActive = false;
                            desertWind = false;
                            narrowRoad = false;
                            rainActive = true;
                            break;
                        case 2:
                            currentOpponents = 4;
                            currentOpponentSpeed = 0.009f;
                            currentTrainSpeed = 0.005f;
                            currentBirdSpeed = 0.006f;
                            currentPlayerSpeed = 0.045f;
                            scoreToNextLevel = 250;
                            nightMode = false;
                            fogActive = false;
                            snowActive = false;
                            desertWind = false;
                            narrowRoad = false;
                            rainActive = false;
                            break;
                        case 3:
                            currentOpponents = 5;
                            currentOpponentSpeed = 0.008f;
                            currentTrainSpeed = 0.004f;
                            currentBirdSpeed = 0.003f;
                            currentPlayerSpeed = 0.04f;
                            scoreToNextLevel = 450;
                            nightMode = true;
                            fogActive = true;
                            snowActive = false;
                            desertWind = false;
                            narrowRoad = false;
                            rainActive = false;
                            initFog();
                            break;
                        case 4:
                            currentOpponents = 6;
                            currentOpponentSpeed = 0.007f;
                            currentTrainSpeed = 0.003f;
                            currentBirdSpeed = 0.002f;
                            currentPlayerSpeed = 0.035f;
                            scoreToNextLevel = 700;
                            nightMode = false;
                            fogActive = false;
                            snowActive = true; // Activate snowfall
                            desertWind = false;
                            narrowRoad = false;
                            rainActive = false;
                            initSnow();
                            break;
                        case 5:
                            currentOpponents = 7;
                            currentOpponentSpeed = 0.012f;
                            currentTrainSpeed = 0.006f;
                            currentBirdSpeed = 0.009f;
                            currentPlayerSpeed = 0.05f;
                            scoreToNextLevel = 1000;
                            nightMode = false;
                            fogActive = false;
                            snowActive = false;
                            desertWind = true;
                            narrowRoad = true;
                            rainActive = false;
                            initDust();
                            break;
                    }

                    playerY = -0.7f;
                    currentLane = 1;
                    initOpponents();
                    initRain();
                    initStars();
                    glutPostRedisplay();
                    return;
                }
            }
        }
    }
}

void mouseMotion(int x, int y) {
    // Convert mouse coordinates to OpenGL coordinates
    float glX = (x / 500.0f) * 2.0f - 1.0f;
    float glY = 1.0f - (y / 450.0f) * 2.0f;

    if (showMenu) {
        // Reset all hover states
        for (int i = 0; i < 5; i++) {
            levelButtons[i].isHovered = false;
        }
        startButton.isHovered = false;
        backButton.isHovered = false;

        if (menuState == 0) { // Main menu
            // Check level buttons
            for (int i = 0; i < 5; i++) {
                if (glX >= levelButtons[i].x1 && glX <= levelButtons[i].x2 &&
                    glY <= levelButtons[i].y1 && glY >= levelButtons[i].y2) {
                    levelButtons[i].isHovered = true;
                    glutPostRedisplay();
                    return;
                }
            }

            // Check start button
            if (glX >= startButton.x1 && glX <= startButton.x2 &&
                glY <= startButton.y1 && glY >= startButton.y2) {
                startButton.isHovered = true;
                glutPostRedisplay();
                return;
            }
        }
    }
}

/* ================= DISPLAY ================= */
void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    if (showMenu) {
        // Draw background for menu
        drawSky();
        drawField();
        drawTreesAndHouses();
        drawTrain();
        drawRoad();

        if (menuState == 0) {
            drawMainMenu();
        } else {
            drawLevelSelectionMenu();
        }
    } else {
        // Draw the game
        drawSky();
        drawField();
        drawTreesAndHouses();
        drawTrain();
        drawRoad();

        // Draw weather effects - fog first (background effect)
        drawFog();

        // Draw game objects
        if(!gameStarted){
            // Display current level selection
            char levelText[100];
            sprintf(levelText, "Selected Level: %d - Press S to Start", currentLevel);
            glColor3f(1.0f, 1.0f, 0.0f);
            glRasterPos2f(-0.3f, 0.0f);
            for(int i=0; levelText[i]!=0; i++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, levelText[i]);

            // Show level descriptions before starting
            glColor3f(0.8f, 0.8f, 1.0f);
            glRasterPos2f(-0.45f, -0.1f);
            const char* desc = "Press 1-5 to change level before starting";
            for(int i=0; desc[i]!=0; i++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, desc[i]);
        } else {
            for(int i=0;i<currentOpponents;i++) drawOpponent(i);
            drawPlayer();

            char buf[32];
            sprintf(buf,"Score: %d",scoreCounter);
            drawText(-0.95f,0.70f,buf);

            sprintf(buf,"Level: %d/%d",currentLevel,MAX_LEVEL);
            drawText(-0.95f,0.65f,buf);

            if(currentLevel < MAX_LEVEL) {
                sprintf(buf,"Next: %d",scoreToNextLevel);
                drawText(-0.95f,0.60f,buf);
            }

            // Display level-specific warnings
            if(currentLevel == 3) {
                glColor3f(1.0f, 1.0f, 0.5f);
                glRasterPos2f(-0.15f, 0.8f);
                const char* warning = "FOGGY - REDUCED VISIBILITY";
                for(int i=0; warning[i]!=0; i++)
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, warning[i]);
            } else if(currentLevel == 4) {
                glColor3f(0.8f, 0.8f, 1.0f);
                glRasterPos2f(-0.12f, 0.8f);
                const char* warning = "SNOWY - SLIPPERY ROAD";
                for(int i=0; warning[i]!=0; i++)
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, warning[i]);
            } else if(currentLevel == 5) {
                glColor3f(1.0f, 0.7f, 0.3f);
                glRasterPos2f(-0.15f, 0.8f);
                const char* warning = "DESERT - NARROW ROAD";
                for(int i=0; warning[i]!=0; i++)
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, warning[i]);
            }

            if(!running)
                drawText(-0.30f,0,"GAME OVER - Press R");
        }

        // Draw weather effects - rain and snow in front of objects
        drawRain();
        drawSnowfall();
        drawDust();

        // Draw level transition effect if active
        if(levelTransition) {
            drawTransitionEffect();
        }
    }

    glFlush();
}

/* ================= TIMER ================= */
void timerFunc(int){
    // Update birds with level-based speed
    bird1_x += currentBirdSpeed;
    bird2_x += currentBirdSpeed;
    bird3_x += currentBirdSpeed;
    bird4_x += currentBirdSpeed;
    bird5_x += currentBirdSpeed;
    bird6_x += currentBirdSpeed;

    // wrap around
    if(bird1_x > 1.2f) bird1_x = -1.2f;
    if(bird2_x > 1.2f) bird2_x = -1.2f;
    if(bird3_x > 1.2f) bird3_x = -1.2f;
    if(bird4_x > 1.2f) bird4_x = -1.2f;
    if(bird5_x > 1.2f) bird5_x = -1.2f;
    if(bird6_x > 1.2f) bird6_x = -1.2f;

    if(running && !levelTransition && !showMenu){
        for(int i=0;i<currentOpponents;i++){
            opY[i] -= opSpeed[i]; // Use individual speeds
            if(opY[i] < ROAD_BOTTOM + CAR_HALF_HEIGHT)
                resetOpponent(i);
        }
        if(checkCollision()) running=false;

        // Check if player should advance to next level
        if(currentLevel < MAX_LEVEL && scoreCounter >= scoreToNextLevel) {
            advanceToNextLevel();
        }

        treeOffsetY -= currentOpponentSpeed*0.5f;
        if(treeOffsetY < -0.25f) treeOffsetY += 0.25f;

        // Update weather effects
        updateFog();
        updateSnowfall();
        updateDust();
    }

    if (!showMenu) {
        trainY -= currentTrainSpeed;
        if(trainY < ROAD_BOTTOM - 0.2f) trainY = ROAD_TOP;
        updateRain();
    }

    glutPostRedisplay();
    glutTimerFunc(30,timerFunc,0);
}

void rainToggleTimer(int){
    // Only toggle rain in level 1
    if(currentLevel == 1) {
        rainActive = !rainActive;
    }
    glutPostRedisplay();
    glutTimerFunc(5000, rainToggleTimer, 0);
}

void specialKeyboard(int key, int x, int y) {
    if (!running || showMenu) return;

    if (key == GLUT_KEY_UP) {
        playerSpeed *= 1.05f;
    }
}

/* ================= KEYBOARD INPUT ================= */
void keyboard(unsigned char key,int,int){
    // Handle level selection with number keys 1-5
    if(key >= '1' && key <= '5') {
        int level = key - '0'; // Convert char to int

        // Update both selectedLevel and currentLevel
        selectedLevel = level;
        currentLevel = level;

        // Update menu button selection
        for (int i = 0; i < 5; i++) {
            levelButtons[i].isSelected = (i == (level - 1));
        }

        // Redisplay to show updated selection
        glutPostRedisplay();
        return;
    }

    // M key to toggle menu
    if(key == 'm' || key == 'M') {
        showMenu = !showMenu;
        if (showMenu) {
            running = false;
        }
        glutPostRedisplay();
        return;
    }

    // ESC key to show menu
    if(key == 27) { // ESC key
        showMenu = true;
        running = false;
        gameStarted = false;
        glutPostRedisplay();
        return;
    }

    // S key to start the game
    if(!gameStarted && (key == 's' || key == 'S')) {
        gameStarted = true;
        running = true;
        scoreCounter = 0;

        // Use currentLevel (which was set by pressing 1-5)
        switch(currentLevel) {
            case 1:
                currentOpponents = 3;
                currentOpponentSpeed = 0.007f;
                currentTrainSpeed = 0.004f;
                currentBirdSpeed = 0.0045f;
                currentPlayerSpeed = 0.05f;
                scoreToNextLevel = 100;
                nightMode = false;
                fogActive = false;
                snowActive = false;
                desertWind = false;
                narrowRoad = false;
                rainActive = true;
                break;
            case 2:
                currentOpponents = 4;
                currentOpponentSpeed = 0.009f;
                currentTrainSpeed = 0.005f;
                currentBirdSpeed = 0.006f;
                currentPlayerSpeed = 0.045f;
                scoreToNextLevel = 250;
                nightMode = false;
                fogActive = false;
                snowActive = false;
                desertWind = false;
                narrowRoad = false;
                rainActive = false;
                break;
            case 3:
                currentOpponents = 5;
                currentOpponentSpeed = 0.008f;
                currentTrainSpeed = 0.004f;
                currentBirdSpeed = 0.003f;
                currentPlayerSpeed = 0.04f;
                scoreToNextLevel = 450;
                nightMode = true;
                fogActive = true;
                snowActive = false;
                desertWind = false;
                narrowRoad = false;
                rainActive = false;
                initFog();
                break;
            case 4:
                currentOpponents = 6;
                currentOpponentSpeed = 0.007f;
                currentTrainSpeed = 0.003f;
                currentBirdSpeed = 0.002f;
                currentPlayerSpeed = 0.035f;
                scoreToNextLevel = 700;
                nightMode = false;
                fogActive = false;
                snowActive = true; // Activate snowfall
                desertWind = false;
                narrowRoad = false;
                rainActive = false;
                initSnow();
                break;
            case 5:
                currentOpponents = 7;
                currentOpponentSpeed = 0.012f;
                currentTrainSpeed = 0.006f;
                currentBirdSpeed = 0.009f;
                currentPlayerSpeed = 0.05f;
                scoreToNextLevel = 1000;
                nightMode = false;
                fogActive = false;
                snowActive = false;
                desertWind = true;
                narrowRoad = true;
                rainActive = false;
                initDust();
                break;
        }

        playerY = -0.7f;
        currentLane = 1;
        initOpponents();
        initRain();
        initStars();
        glutPostRedisplay();
        return;
    }

    // R key to restart
    if(key == 'r' || key == 'R') {
        running = true;
        scoreCounter = 0;

        // Restart with current level
        switch(currentLevel) {
            case 1:
                currentOpponents = 3;
                currentOpponentSpeed = 0.007f;
                currentTrainSpeed = 0.004f;
                currentBirdSpeed = 0.0045f;
                currentPlayerSpeed = 0.05f;
                scoreToNextLevel = 100;
                nightMode = false;
                fogActive = false;
                snowActive = false;
                desertWind = false;
                narrowRoad = false;
                rainActive = true;
                break;
            case 2:
                currentOpponents = 4;
                currentOpponentSpeed = 0.009f;
                currentTrainSpeed = 0.005f;
                currentBirdSpeed = 0.006f;
                currentPlayerSpeed = 0.045f;
                scoreToNextLevel = 250;
                nightMode = false;
                fogActive = false;
                snowActive = false;
                desertWind = false;
                narrowRoad = false;
                rainActive = false;
                break;
            case 3:
                currentOpponents = 5;
                currentOpponentSpeed = 0.008f;
                currentTrainSpeed = 0.004f;
                currentBirdSpeed = 0.003f;
                currentPlayerSpeed = 0.04f;
                scoreToNextLevel = 450;
                nightMode = true;
                fogActive = true;
                snowActive = false;
                desertWind = false;
                narrowRoad = false;
                rainActive = false;
                initFog();
                break;
            case 4:
                currentOpponents = 6;
                currentOpponentSpeed = 0.007f;
                currentTrainSpeed = 0.003f;
                currentBirdSpeed = 0.002f;
                currentPlayerSpeed = 0.035f;
                scoreToNextLevel = 700;
                nightMode = false;
                fogActive = false;
                snowActive = true; // Keep snowfall active
                desertWind = false;
                narrowRoad = false;
                rainActive = false;
                initSnow();
                break;
            case 5:
                currentOpponents = 7;
                currentOpponentSpeed = 0.012f;
                currentTrainSpeed = 0.006f;
                currentBirdSpeed = 0.009f;
                currentPlayerSpeed = 0.05f;
                scoreToNextLevel = 1000;
                nightMode = false;
                fogActive = false;
                snowActive = false;
                desertWind = true;
                narrowRoad = true;
                rainActive = false;
                initDust();
                break;
        }

        playerY = -0.7f;
        currentLane = 1;
        nightMode = false;
        heavyRain = false;
        levelTransition = false;
        initOpponents();
        glutPostRedisplay();
        return;
    }

    // Other game controls
    if(key == 'n' || key == 'N') nightMode = !nightMode;
    if(key == 'h' || key == 'H') heavyRain = !heavyRain;

    if(!running) return;

    // Lane changing with level-specific restrictions
    if(key == 'a' || key == 'A') {
        if(currentLane > 0) currentLane--;
        // In desert level, prevent going too far left
        if(currentLevel == 5 && narrowRoad && currentLane == 0) {
            currentLane = 1; // Keep in middle or right
        }
    }
    if(key == 'd' || key == 'D') {
        if(currentLane < 2) currentLane++;
        // In desert level, prevent going too far right
        if(currentLevel == 5 && narrowRoad && currentLane == 2) {
            currentLane = 1; // Keep in middle or left
        }
    }

    // Movement speed varies by level
    float effectiveSpeed = currentPlayerSpeed;
    if(currentLevel == 4) effectiveSpeed *= 0.8f; // Slower in snow
    if(currentLevel == 5) effectiveSpeed *= 1.1f; // Faster in desert

    if(key == 'w' || key == 'W') {
        if(playerY < PLAYER_TOP_LIMIT) playerY += effectiveSpeed;
    }
    if(key == 's' || key == 'S') {
        if(playerY > PLAYER_BOTTOM_LIMIT) playerY -= effectiveSpeed;
    }
}

/* ================= RESHAPE ================= */
void reshape(int w,int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1,1,-1,1);
    glMatrixMode(GL_MODELVIEW);
}

/* ================= MAIN ================= */
int main(int argc,char** argv){
    seedVal = (unsigned int)time(NULL);

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(1000,900);
    glutCreateWindow("5-Level Racing Game: Weather & Terrain Challenge");

    glClearColor(0,0,0,1);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(mouseMotion);

    initMenuButtons();
    initRain();
    initStars();
    initSnow(); // Initialize snow system
    initFog();
    initDust();
    glutTimerFunc(5000, rainToggleTimer, 0);
    glutTimerFunc(30, timerFunc, 0);

    glutMainLoop();
    return 0;
}
