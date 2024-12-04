#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Game settings
int width = 20, height = 17;
int gameOver, score, speed, boundaryMode;
int x, y, fruitX, fruitY, flag;
int tailX[100], tailY[100];
int nTail;

// Function to set console text color (Windows/Linux)
void setColor(int color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
#else
    printf("\033[0;%dm", color);
#endif
}

// Function to reset console text color
void resetColor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7); // Default color
#else
    printf("\033[0m");
#endif
}

// Function to clear screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Function to draw the board
void draw() {
    clearScreen();
    int i, j;
    
    // Draw top border
    setColor(11); // Cyan color
    for (i = 0; i < width + 2; i++) printf("¦");
    printf("\n");
    
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if (j == 0) printf("¦"); // Left wall
            
            if (i == y && j == x) {
                setColor(10); // Green for snake head
                printf("O");
                resetColor();
            } else if (i == fruitY && j == fruitX) {
                setColor(12); // Red for fruit
                printf("F");
                resetColor();
            } else {
                int isTail = 0;
                int k;
                for (k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        setColor(14); // Yellow for snake body
                        printf("o");
                        resetColor();
                        isTail = 1;
                    }
                }
                if (!isTail) printf(" ");
            }
            
            if (j == width - 1) printf("¦"); // Right wall
        }
        printf("\n");
    }

    // Draw bottom border
    setColor(11); // Cyan color
    for (i = 0; i < width + 2; i++) printf("¦");
    printf("\n");
    resetColor();

    // Display game stats
    setColor(13); // Purple for stats
    printf("Score: %d | Speed: %d | Mode: %s\n", score, speed, boundaryMode ? "Wrap" : "Wall");
    resetColor();
}

// Function to set up the game
void setup() {
    gameOver = 0;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
    speed = 100; // Initial speed
    boundaryMode = 0; // Wall collision by default
}

// Function to place the fruit at a valid position
void placeFruit() {
    int valid = 0;
    while (!valid) {
        valid = 1;
        fruitX = rand() % width;
        fruitY = rand() % height;
        int i;
        for (i = 0; i < nTail; i++) {
            if (fruitX == tailX[i] && fruitY == tailY[i]) {
                valid = 0; // Fruit overlaps with the snake
                break;
            }
        }
    }
}

// Function to take input
void input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a': if (flag != 2) flag = 1; break; // Move left
            case 'd': if (flag != 1) flag = 2; break; // Move right
            case 'w': if (flag != 4) flag = 3; break; // Move up
            case 's': if (flag != 3) flag = 4; break; // Move down
            case 'x': gameOver = 1; break;           // Exit game
            case 'p':                                 // Pause game
                printf("Game Paused. Press any key to resume...\n");
                _getch();
                break;
            case 'b': boundaryMode = !boundaryMode; break; // Toggle boundary mode
        }
    }
}

// Function for the game logic
void logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    int i;
    for (i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (flag) {
        case 1: x--; break; // Left
        case 2: x++; break; // Right
        case 3: y--; break; // Up
        case 4: y++; break; // Down
        default: break;
    }

    // Handle boundary behavior
    if (boundaryMode) {
        if (x < 0) x = width - 1;
        if (x >= width) x = 0;
        if (y < 0) y = height - 1;
        if (y >= height) y = 0;
    } else {
        if (x < 0 || x >= width || y < 0 || y >= height)
            gameOver = 1;
    }

    // Check for self-collision
    for (i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameOver = 1;
            break;
        }
    }

    // If snake eats the fruit
    if (x == fruitX && y == fruitY) {
        score += 10;
        nTail++;
        placeFruit();
        // Increase speed every 50 points
        if (score % 50 == 0 && speed > 20) speed -= 10;
    }
}

// Main function
int main() {
    srand(time(0)); // Seed for random number generation
    setup();
    while (!gameOver) {
        draw();
        input();
        logic();
#ifdef _WIN32
        Sleep(speed); // Sleep in Windows
#else
        usleep(speed * 1000); // Sleep in Linux
#endif
    }
    setColor(12); // Red for Game Over
    printf("Game Over! Final Score: %d\n", score);
    resetColor();
    return 0;
}