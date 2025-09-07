#include <stdio.h>
#include <stdlib.h>

#define MAX_A 4
#define RESET   "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define BLUE    "\x1B[34m"


typedef struct {
    int x, y;
} Grid;

void deleteCoord(int arr[][2], int index, int *count) {
    for (int i = index; i < *count - 1; i++) {
        arr[i][0] = arr[i + 1][0];
        arr[i][1] = arr[i + 1][1];
    }
    (*count)--;
}

void removeCoords(int x, int y, int arr[][2], int *count) {
    for (int i = 0; i < *count; i++) {
        if (arr[i][0] == x && arr[i][1] == y) {
            deleteCoord(arr, i, count);
            break;
        }
    }
}

int checkWin(int moves[][2], int count) {
    int winPatterns[4][4][2] = {
        {{1,1}, {1,2}, {1,3}, {1,4}},
        {{1,4}, {2,3}, {3,2}, {4,1}},
        {{4,1}, {4,2}, {4,3}, {4,4}},
        {{1,1}, {2,2}, {3,3}, {4,4}}
    };
    
    for (int i = 0; i < 4; i++) {
        int match = 0;
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < count; k++) {
                if (moves[k][0] == winPatterns[i][j][0] && moves[k][1] == winPatterns[i][j][1]) {
                    match++;
                    break;
                }
            }
        }
        if (match == 4) return 1;
    }
    return 0;
}

void displayBoard(Grid P[MAX_A][MAX_A], int Uno[][2], int Tres[][2], int UnoCount, int TresCount) {
    system("cls");
    printf("     1     2     3     4\n");
    printf("  -------------------------\n");
    for (int x = 0; x < MAX_A; x++) {
        printf("%d |", x + 1);
        for (int y = 0; y < MAX_A; y++) {
            int occupied = 0;
            for (int i = 0; i < UnoCount; i++) {
                if (Uno[i][0] == x + 1 && Uno[i][1] == y + 1) {
                    printf(" " RED " U " RESET " |");
                    occupied = 1;
                    break;
                }
            }
            for (int i = 0; i < TresCount && !occupied; i++) {
                if (Tres[i][0] == x + 1 && Tres[i][1] == y + 1) {
                    printf(" " BLUE " T " RESET " |");
                    occupied = 1;
                    break;
                }
            }
            if (!occupied) printf("(%d,%d)|", P[x][y].x, P[x][y].y);
        }
        printf("\n  -------------------------\n");
    }
}

void playerMove(const char *player, int arr[][2], int *count, int Uno[][2], int UnoCount, int Tres[][2], int TresCount) {
    int px, py, valid;
    do {
        valid = 1;
        printf("%s's Turn! Enter row and column (x,y): ", player);
        scanf("%d,%d", &px, &py);
        
        if (px < 1 || px > 4 || py < 1 || py > 4) {
            printf("Invalid move! Choose numbers between 1 and 4.\n");
            valid = 0;
        } else {
            for (int i = 0; i < UnoCount; i++) {
                if (Uno[i][0] == px && Uno[i][1] == py) {
                    printf("Spot taken! Choose another position.\n");
                    valid = 0;
                    break;
                }
            }
            for (int i = 0; i < TresCount && valid; i++) {
                if (Tres[i][0] == px && Tres[i][1] == py) {
                    printf("Spot taken! Choose another position.\n");
                    valid = 0;
                    break;
                }
            }
        }
    } while (!valid);
    
    arr[*count][0] = px;
    arr[*count][1] = py;
    (*count)++;
}

void dosRemove(int Uno[][2], int Tres[][2], int *UnoCount, int *TresCount) {
    int px, py;
    printf("Dos's Turn! Enter row and column (x,y) to remove: ");
    scanf("%d,%d", &px, &py);
    
    removeCoords(px, py, Uno, UnoCount);
    removeCoords(px, py, Tres, TresCount);
}

int main() {
    int over = 0;
    Grid P[MAX_A][MAX_A];
    int Uno[MAX_A * MAX_A][2], Tres[MAX_A * MAX_A][2];
    int UnoCount = 0, TresCount = 0;

    for (int x = 0; x < MAX_A; x++) {
        for (int y = 0; y < MAX_A; y++) {
            P[x][y].x = x + 1;
            P[x][y].y = y + 1;
        }
    }

    while (!over) {
        displayBoard(P, Uno, Tres, UnoCount, TresCount);
        
        playerMove("Uno", Uno, &UnoCount, Uno, UnoCount, Tres, TresCount);
        displayBoard(P, Uno, Tres, UnoCount, TresCount);
        if (checkWin(Uno, UnoCount)) {
            printf("Uno Wins!\n");
            break;
        }
        
        playerMove("Tres", Tres, &TresCount, Uno, UnoCount, Tres, TresCount);
        displayBoard(P, Uno, Tres, UnoCount, TresCount);
        if (checkWin(Tres, TresCount)) {
            printf("Tres Wins!\n");
            break;
        }
        
        dosRemove(Uno, Tres, &UnoCount, &TresCount);
        displayBoard(P, Uno, Tres, UnoCount, TresCount);
        
        if (UnoCount + TresCount == MAX_A * MAX_A) {
            printf("Dos Wins (Board Full)!\n");
            break;
        }
    }
    return 0;
}

