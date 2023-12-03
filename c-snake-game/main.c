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
#define WIDTH 20
#define HEIGHT 10

// start position for snake
int y = 5;
int x = 5;

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
int kbhit() {
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
    #ifdef WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main(void) {
    
    char snakeHead = '@';
    char direction = 'd';
    int isRunning = 1;
    
    setNonBlockingMode();
    
    while (isRunning == 1) {
        clearScreen();
        
        printf("%c", direction);
        
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
        
        for (int dY = 0; dY <= HEIGHT; dY++) {
            for (int dX = 0; dX <= WIDTH; dX++) {
                
                if (y == dY && x == dX) {
                    printf("%c", snakeHead);
                } else {
                    printf(" ");
                }
                
                
                if (y == HEIGHT || y == 0) {
                    printf("y died here");
                    isRunning = 0;
                }
                
                if (x == WIDTH || x == 0) {
                    printf("x died here");
                    isRunning = 0;
                }
            }
            printf("\n");
        }
        
        usleep(500000);
        
        
        if (kbhit()) {
            int input = getchar();
            switch(input) {
                case 'w':
                case 'a':
                case 's':
                case 'd':
                    direction = input;
                    break;
            }
        }
        
    
        if (isRunning == 0) {
            printf("\n");
            printf("YOU DIED!");
        }
    }
    
    return 0;
}
