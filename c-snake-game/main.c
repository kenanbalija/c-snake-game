//
//  main.c
//  c-snake-game
//
//  Created by Kenan Balija on 3. 12. 2023..
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
// screen size
#define SIZE 10

char buffer[SIZE][SIZE];

void setNonBlockingMode(void) {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag &= ~(ICANON | ECHO);
    ttystate.c_cc[VMIN] = 0;
    ttystate.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

    // Set file descriptor to non-blocking mode
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
}

// Function to check if there's input ready to be read
int kbhit(void) {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}


void clearScreen(void) {
    for (int i = 0; i < 20; i++) {
        printf("\n");
    }
}

void initializeBuffer (void) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            buffer[i][j] = ' ';
        }
    }
}

void drawScreen (void) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c", buffer[i][j]);
        }
        printf("\n");
    }
}


int randomPlace (void) {
    int numbero = rand() % (9 - 1 + 1) + 1;
    printf("%d", numbero);
    return numbero;
};


int main(void) {
    
    setenv("TERM", "xterm", 1);
    
    int isRunning = 1;
    
    // start position for snake
    int y = 5;
    int x = 5;
    char snakeHead = '@';
    char direction = 'd';
    char oppositeDirection = 'a';
    
    randomPlace();
    
    char apple = '*';
    
    setNonBlockingMode();
    initializeBuffer();
    
    while (isRunning == 1) {
        
        clearScreen();
        
        switch (direction) {
            case 'w':
                y--;
                break;
            case 's':
                y++;
                break;
            case 'd':
                x++;
                break;
            case 'a':
                x--;
                break;
        }
        
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (i == 0 || i == SIZE - 1) {
                    buffer[i][j] = '-';
                } else if (j == 0 || j == SIZE - 1) {
                    buffer[i][j] = '|';
                } else if (y == i && x == j) {
                    buffer[i][j] = snakeHead;
                } else {
                    buffer[i][j] = ' ';
                }
                
                
                buffer[7][8] = apple;

                if (y == SIZE || y == 0 || x == SIZE || x == 0) {
                    isRunning = 0;
                }
            }
        }
        
        drawScreen();
        
        usleep(500000);
        
        
        if (kbhit()) {
            int input = getchar();
            // can't move in opposite direction
            if (input != oppositeDirection) {
                switch(input) {
                    case 'w':
                        oppositeDirection = 's';
                        direction = input;
                        break;
                    case 'a':
                        oppositeDirection = 'd';
                        direction = input;
                        break;
                    case 's':
                        oppositeDirection = 'w';
                        direction = input;
                        break;
                    case 'd':
                        oppositeDirection = 'a';
                        direction = input;
                        break;
                }
            }
        }
        
    
        if (isRunning == 0) {
            printf("\n");
            printf("YOU DIED!");
        }
    }
    
    return 0;
}
