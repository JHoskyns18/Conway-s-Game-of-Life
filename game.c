/*
* Project for PHY2027 module
* Conway's Game of Life
* Author: James Hoskyns
*/

/*
 * Program description: This program must follow conways rules and run the game of life. This means there must be a way of visualising the game board. The user should be able to pick a starting grid from a set of predefined boards, or create their own board of variable size.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


//This is a structure that contains all the variables to do with the board, and the running of the game.
typedef struct grid_info {
    int len;
    int wid;
    int **grid;
    int **next_grid;
} Grid_info ;

//==================== Function Definitions ==============

int input(int min, int max);
int alive_or_dead(int n, int alive_neighbours);
void next(Grid_info *g);
void custom(Grid_info *g);
void print_board(Grid_info *g);
void equal_grids(Grid_info *g, int array1[g->len][g->wid]);
void run(int iterations, Grid_info *g);
void preset(Grid_info *g);


//======================= Main Program =========================

int main(){
    Grid_info g;
    g.len = 40;
    g.wid = 40;
    
    //Memory allocation for grid and next grid in the structure. Max dimensions for the game board in this program is 40x40.
    g.grid = (int **)malloc(40*sizeof(int*));
    if (g.grid == NULL) {
        printf("Out of memory!\n");
        return -1;
    }
    for(int l = 0; l<g.len; l++){
        g.grid[l] = (int *)malloc(40*sizeof(int));
        if (g.grid[l] == NULL) {
            printf("Out of memory!\n");
            return -1;
        }
    }
    g.next_grid = (int **)malloc(40*sizeof(int*));
    if (g.next_grid == NULL) {
        printf("Out of memory!\n");
        return -1;
    }
    for(int l = 0; l<g.len; l++){
        g.next_grid[l] = (int *)malloc(40*sizeof(int));
        if (g.next_grid[l] == NULL) {
            printf("Out of memory!\n");
            return -1;
        }
    }
    
//Start of menu
    int repeat=1,choice, iterations;
    while(repeat != 0){
        printf("Menu:\n");
        printf("Press 1 to start the game from a set of pre-defined configurations\n");
        printf("Press 2 to set a custom board\n");
        printf("Press 3 to exit\n");
        choice = input(1,3);
        switch (choice) {
            case 1: {
                preset(&g);
                printf("How many repeats do you want the game to do? (max 200)\n");
                iterations = input(1,200);
                run(iterations, &g);
                break;
            }
            case 2: {
                printf("Please enter the length of your custom board:\n");
                g.len = input(1,40);
                printf("Please enter the width of your custom board:\n");
                g.wid = input(1,40);
                printf("The custom board size is %d by %d\n",g.len,g.wid);
                custom(&g);
                printf("How many repeats do you want the game to do? (max 200)\n");
                iterations = input(1,200);
                run(iterations, &g);
                break;
            }
            case 3: {
                printf("Thank you for using this code.\n");
                repeat=0;
                break;
            }
            default:
                printf("You were meant to enter an integer between 1 and 4. Please try again.\n");
        }
    }
    return 0;
}


//========================= Functions =======================

//An input function with a check on the user input is useful in this code. All the inputs needed from the user in this program are integers, but vary with the limits depending on the situation. For example, the input for a custom grid is either 0 or 1, but the input for the grid length can go from 1 to 40.
int input(int min, int max){
    int x, repeat=1;
    scanf("%i", &x);
    while(repeat!=0){
        if( x < min || x > max ){
            printf("You were meant to enter an integer between %d and %d.\nPlease try again:\n",min,max);
            scanf("%d", &x);
        }
        else{
            repeat = 0;
        }
    }
    return x;
}

//This function prints a 2D array as a grid to the terminal screen. It goes through the whole 2D array in order printing a * where there are ones (alive) and a . where there are zeros (dead).
void print_board(Grid_info *g){
    printf("\n");
    for (int l=0 ; l < g->len ; l++){
        printf("(");
        for (int w=0 ; w < g->wid ; w++){
            if(g->grid[l][w]==1){
                printf(" * ");
            }else{
                printf(" . ");
            }
        }
        printf(")\n");
    }
    printf("\n");
}

//This is a function so then this for loop isn't repeated all throughout the code specifically the preset function. The function takes all the values from a 2D array and puts them into the structure where the array can be run in the game.
void equal_grids(Grid_info *g, int array[g->len][g->wid]){
    for(int l=0; l<g->len; l++){
        for(int w=0; w<g->wid; w++){
            g->grid[l][w] = array[l][w];
        }
    }
}

//This function determines the future of any cell (being 0 or 1) depending on how many alive neighbours it has. The rules for Conways Game of Life are that for a dead cell to become alive, it needs exactly 3 alive neighbours. An alive cell will die to overpopulation if there are 4 or more alive neighbours. An alive cell will die to solitude if it has one or zero alive neighbours. This if else statement implements these rules into the code. The input of n in this function can only be 0 or 1.
int alive_or_dead(int n, int alive_neighbours){
    //Cell dies
    if ((n == 1 && alive_neighbours < 2) || (n == 1 && alive_neighbours > 3)){
        return 0;
    }
    //Cell is born
    else if (n == 0 && alive_neighbours == 3){
        return 1;
    }
    //No change to cell
    else{
        return n;
    }
}

//This function calculates the next grid based off conways rules and also the starting grid. Each cell has eight specific neighbours. It is complicated to code the neighbours of the cells at the boundaries of the grid, as the grid needs to loop around so the board is continuous.
void next(Grid_info *g){
    int alive_neighbours;
    
    //MAIN INSIDE BLOCK
    for(int l=1; l+1<g->len; l++){
        for(int w=1; w+1<g->wid; w++){
            alive_neighbours = g->grid[l-1][w-1] + g->grid[l-1][w] + g->grid[l-1][w+1] + g->grid[l][w-1] + g->grid[l][w+1] + g->grid[l+1][w-1] + g->grid[l+1][w] + g->grid[l+1][w+1];
            g->next_grid[l][w] = alive_or_dead(g->grid[l][w], alive_neighbours);
        }
    }
    //EDGES - Loops round one side
    //left
    for(int l=1; l+1<g->len; l++){
        alive_neighbours = g->grid[l+1][1] + g->grid[l][1] + g->grid[l-1][1] + g->grid[l+1][0] + g->grid[l-1][0] + g->grid[l+1][g->wid-1] + g->grid[l][g->wid-1] + g->grid[l-1][g->wid-1];
        g->next_grid[l][0] = alive_or_dead(g->grid[l][0], alive_neighbours);
    }
    //right
    for(int l=1; l+1<g->len; l++){
        alive_neighbours = g->grid[l+1][0] + g->grid[l][0] + g->grid[l-1][0] + g->grid[l+1][g->wid-1] + g->grid[l-1][g->wid-1] + g->grid[l+1][g->wid-2] + g->grid[l][g->wid-2] + g->grid[l-1][g->wid-2];
        g->next_grid[l][g->wid-1] = alive_or_dead(g->grid[l][g->wid-1], alive_neighbours);
    }
    //top
    for(int w=1; w+1<g->wid; w++){
        alive_neighbours = g->grid[1][w+1] + g->grid[1][w] + g->grid[1][w-1] + g->grid[0][w+1] + g->grid[0][w-1] + g->grid[g->len-1][w+1] + g->grid[g->len-1][w] + g->grid[g->len-1][w-1];
        g->next_grid[0][w] = alive_or_dead(g->grid[0][w], alive_neighbours);
    }
    //bottom
    for(int w=1; w+1<g->wid; w++){
        alive_neighbours = g->grid[0][w+1] + g->grid[0][w] + g->grid[0][w-1] + g->grid[g->len-1][w+1] + g->grid[g->len-1][w-1] + g->grid[g->len-2][w+1] + g->grid[g->len-2][w] + g->grid[g->len-2][w-1];
        g->next_grid[g->len-1][w] = alive_or_dead(g->grid[g->len-1][w], alive_neighbours);
    }
    //CORNERS - Loops round up to 3 sides
    //top left
    alive_neighbours = g->grid[1][1] + g->grid[1][0] + g->grid[1][g->wid-1] + g->grid[0][1] + g->grid[0][g->wid-1] + g->grid[g->len-1][1] + g->grid[g->len-1][0] + g->grid[g->len-1][g->wid-1] ;
    g->next_grid[0][0] = alive_or_dead(g->grid[0][0], alive_neighbours);
    
    //top right
    alive_neighbours = g->grid[g->len-1][g->wid-2] + g->grid[g->len-1][g->wid-1] + g->grid[g->len-1][0] + g->grid[0][g->wid-2] + g->grid[0][0] + g->grid[1][g->wid-2] + g->grid[1][g->wid-1] + g->grid[1][0] ;
    g->next_grid[0][g->wid-1] = alive_or_dead(g->grid[0][g->wid-1], alive_neighbours);
    
    //bottom left
    alive_neighbours = g->grid[g->len-2][g->wid-1] + g->grid[g->len-2][0] + g->grid[g->len-2][1] + g->grid[g->len-1][g->wid-1] + g->grid[g->len-1][1] + g->grid[0][g->wid-1] + g->grid[0][0] + g->grid[0][1] ;
    g->next_grid[g->len-1][0] = alive_or_dead(g->grid[g->len-1][0], alive_neighbours);
    
    //bottom right
    alive_neighbours = g->grid[g->len-2][g->wid-2] + g->grid[g->len-2][g->wid-1] + g->grid[g->len-2][0] + g->grid[g->len-1][g->wid-2] + g->grid[g->len-1][0] + g->grid[0][g->wid-2] + g->grid[0][g->wid-1] + g->grid[0][0] ;
    g->next_grid[g->len-1][g->wid-1] = alive_or_dead(g->grid[g->len-1][g->wid-1], alive_neighbours);
}


//This function lets the user create their own starting grid, each position at a time. This is a slow process but works decently for grids smaller than 10x10.
void custom(Grid_info *g){
    printf("\nPlease enter your values in for your custom board - 0 is dead, 1 is alive a:\n");
    for (int l=0 ; l < g->len ; l++){
        for (int w=0 ; w < g->wid ; w++){
            printf("Please enter your input for the [%d][%d] position:\n",l,w);
            g->grid[l][w] = input(0,1);
        }
    }
}

//This function weaves all the other functions together and takes the correct steps for each iteration of the game. The game will stop if there is no change between iterations.
void run(int iterations, Grid_info *g){
    int j=0, stop=0, same;
    while (j<iterations && stop == 0){
        next(g);
        print_board(g);
        sleep(1);
        printf("\n\n\n\n\n\n\n\n\n\n\n\n");
        
        //For loop and if statement to check if boards are identitcal between iterations.
        same = 0;
        for(int l=0; l<g->len; l++){
            for(int w=0; w<g->wid; w++){
                if (g->grid[l][w] == g->next_grid[l][w]){
                    same += 1;
                }
            }
        }
        if (same == (g->len * g->wid)){
            printf("No change in grid so game will stop.\n");
            stop = 1;
        }
        
        // Readying grid for next interation
        for(int l=0; l<g->len; l++){
            for(int w=0; w<g->wid; w++){
                g->grid[l][w] = g->next_grid[l][w];
            }
        }
        j += 1;
    }
}

// This function lets the user choose 1 of 5 preconfigured grids to be but into the structure and therefore be run in the game. The grids are displayed to the user so they can choose.
void preset(Grid_info *g){
    int grid1[10][10], grid2[10][10] = {{0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,1,0,0}, {0,0,0,0,0,1,0,1,0,0}, {0,0,0,0,0,0,1,1,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}}, grid3[40][40], grid4[40][40], grid5[40][40];
    
    //Grid 1 has cells that are randomised alive or dead (0 or 1).
    for(int l=0; l<10; l++){
        for(int w=0; w<10; w++){
            grid1[l][w] = rand() % 2;
        }
    }
    
    //For the 3 larger grids, the contents of the arrays are appended from plain text files.
    FILE *grid3_file;
    grid3_file = fopen("/Users/jamiehoskyns/Documents/Coding/Project - Conway's Game of Life/Project - Conway's Game of Life/grid3.txt","r");
    for(int l=0; l<40; l++){
        fscanf(grid3_file, "%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i", &grid3[l][0],&grid3[l][1],&grid3[l][2],&grid3[l][3],&grid3[l][4],&grid3[l][5],&grid3[l][6],&grid3[l][7],&grid3[l][8],&grid3[l][9],&grid3[l][10],&grid3[l][11],&grid3[l][12],&grid3[l][13],&grid3[l][14],&grid3[l][15],&grid3[l][16],&grid3[l][17],&grid3[l][18],&grid3[l][19],&grid3[l][20],&grid3[l][21],&grid3[l][22],&grid3[l][23],&grid3[l][24],&grid3[l][25],&grid3[l][26],&grid3[l][27],&grid3[l][28],&grid3[l][29],&grid3[l][30],&grid3[l][31],&grid3[l][32],&grid3[l][33],&grid3[l][34],&grid3[l][35],&grid3[l][36],&grid3[l][37],&grid3[l][38],&grid3[l][39]);
    }
    fclose(grid3_file);
    
    FILE *grid4_file;
    grid4_file = fopen("/Users/jamiehoskyns/Documents/Coding/Project - Conway's Game of Life/Project - Conway's Game of Life/grid4.txt","r");
    for(int l=0; l<40; l++){
        fscanf(grid4_file, "%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i", &grid4[l][0],&grid4[l][1],&grid4[l][2],&grid4[l][3],&grid4[l][4],&grid4[l][5],&grid4[l][6],&grid4[l][7],&grid4[l][8],&grid4[l][9],&grid4[l][10],&grid4[l][11],&grid4[l][12],&grid4[l][13],&grid4[l][14],&grid4[l][15],&grid4[l][16],&grid4[l][17],&grid4[l][18],&grid4[l][19],&grid4[l][20],&grid4[l][21],&grid4[l][22],&grid4[l][23],&grid4[l][24],&grid4[l][25],&grid4[l][26],&grid4[l][27],&grid4[l][28],&grid4[l][29],&grid4[l][30],&grid4[l][31],&grid4[l][32],&grid4[l][33],&grid4[l][34],&grid4[l][35],&grid4[l][36],&grid4[l][37],&grid4[l][38],&grid4[l][39]);
    }
    fclose(grid4_file);
    
    FILE *grid5_file;
    grid5_file = fopen("/Users/jamiehoskyns/Documents/Coding/Project - Conway's Game of Life/Project - Conway's Game of Life/grid5.txt","r");
    for(int l=0; l<40; l++){
        fscanf(grid5_file, "%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i", &grid5[l][0],&grid5[l][1],&grid5[l][2],&grid5[l][3],&grid5[l][4],&grid5[l][5],&grid5[l][6],&grid5[l][7],&grid5[l][8],&grid5[l][9],&grid5[l][10],&grid5[l][11],&grid5[l][12],&grid5[l][13],&grid5[l][14],&grid5[l][15],&grid5[l][16],&grid5[l][17],&grid5[l][18],&grid5[l][19],&grid5[l][20],&grid5[l][21],&grid5[l][22],&grid5[l][23],&grid5[l][24],&grid5[l][25],&grid5[l][26],&grid5[l][27],&grid5[l][28],&grid5[l][29],&grid5[l][30],&grid5[l][31],&grid5[l][32],&grid5[l][33],&grid5[l][34],&grid5[l][35],&grid5[l][36],&grid5[l][37],&grid5[l][38],&grid5[l][39]);
    }
    fclose(grid5_file);

    //Shows the grids to the user so they can choose one.
    printf("Please choose one of the five following starting grids to run.\n");
    printf("\nGrid 1 - Picked randomly (10x10):\n");
    g->len = 10;
    g->wid = 10;
    equal_grids(g, grid1);
    print_board(g);
    sleep(2);
    
    printf("Grid 2 - Glider (10x10):\n");
    equal_grids(g, grid2);
    print_board(g);
    sleep(2);
    
    printf("Grid 3 - Pattern (40x40):\n");
    g->len = 40;
    g->wid = 40;
    equal_grids(g, grid3);
    print_board(g);
    sleep(2);
    
    printf("Grid 4 - Oscillator (40x40):\n");
    equal_grids(g, grid4);
    print_board(g);
    sleep(2);
    
    printf("Grid 5 - Gun (40x40):\n");
    equal_grids(g, grid5);
    print_board(g);
    
    printf("Please enter 1 for grid 1, 2 for grid 2 etc... :\n");
    int x = input(1,5);
    
    //After the user chooses a grid, the correct numbers from an array are entered into the structure.
    if(x == 1){
        g->len = 10;
        g->wid = 10;
        equal_grids(g, grid1);
    }
    if(x == 2){
        g->len = 10;
        g->wid = 10;
        equal_grids(g, grid2);
    }
    if(x == 3){
        g->len = 40;
        g->wid = 40;
        equal_grids(g, grid3);
    }
    if(x == 4){
        g->len = 40;
        g->wid = 40;
        equal_grids(g, grid4);
    }
    if(x == 5){
        g->len = 40;
        g->wid = 40;
        equal_grids(g, grid5);
    }
}

//========================= Results of the Program ===============================================

// ==== First part of this will be the predefined configurations: (I will keep the iterations of the game low so there isn't too much to copy and paste for the results) ====
 /*
 Menu:
 Press 1 to start the game from a set of pre-defined configurations
 Press 2 to set a custom board
 Press 3 to exit
 1
 Please choose one of the five following starting grids to run.

 Grid 1 - Picked randomly (10x10):

 ( *  *  *  .  .  .  .  .  *  * )
 ( .  *  .  .  *  *  *  *  .  . )
 ( *  *  *  *  .  *  *  .  .  * )
 ( *  .  .  *  .  *  *  *  *  * )
 ( *  .  *  .  *  *  .  *  .  * )
 ( *  *  .  .  .  .  .  .  .  * )
 ( .  .  *  .  *  *  *  .  .  * )
 ( .  *  *  *  *  .  *  .  *  * )
 ( *  .  *  *  *  *  *  .  *  . )
 ( .  *  *  .  .  .  .  *  .  . )

 Grid 2 - Glider (10x10):

 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  *  .  . )
 ( .  .  .  .  .  *  .  *  .  . )
 ( .  .  .  .  .  .  *  *  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )

 Grid 3 - Pattern (40x40):

 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  *  *  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )

 Grid 4 - Oscillator (40x40):

 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  *  .  .  .  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  .  .  .  .  .  .  *  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  *  .  *  .  .  .  .  .  .  .  .  .  *  .  *  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  *  .  .  .  .  .  .  .  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  .  .  *  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )

 Grid 5 - Gun (40x40):

 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  *  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  *  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  *  *  .  .  .  *  *  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  *  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  *  *  .  .  .  .  *  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  .  .  .  *  *  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  *  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  *  .  .  *  .  .  *  .  *  * )
 ( .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .  .  .  .  .  .  *  .  .  .  .  .  .  *  *  .  .  *  * )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  *  .  .  .  .  .  .  .  *  .  *  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  .  .  .  .  .  .  .  .  *  *  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )

 Please enter 1 for grid 1, 2 for grid 2 etc... :
 3
 How many repeats do you want the game to do? (max 200)
 10







 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  *  *  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )














 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )














 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )














 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  *  *  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )














 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  *  *  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )














 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  *  .  .  *  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  *  *  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  *  *  *  *  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )














 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  *  *  *  *  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  .  .  .  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  *  .  .  *  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  *  *  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )














 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  *  *  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  *  *  *  *  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  .  .  .  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  .  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  *  *  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )














 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  .  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  .  .  .  .  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  *  *  *  *  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  .  .  .  .  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  .  .  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )














 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  .  *  *  .  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  .  .  *  *  .  .  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  .  .  .  .  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  *  .  .  *  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  *  *  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  . )

 Menu:
 Press 1 to start the game from a set of pre-defined configurations
 Press 2 to set a custom board
 Press 3 to exit
 3
 Thank you for using this code.
 Program ended with exit code: 0
 
 */

// ========== Now for a custom board (Spaceship): =============

/*
 Menu:
 Press 1 to start the game from a set of pre-defined configurations
 Press 2 to set a custom board
 Press 3 to exit
 2
 Please enter the length of your custom board:
 10
 Please enter the width of your custom board:
 10
 The custom board size is 10 by 10

 Please enter your values in for your custom board - 0 is dead, 1 is alive a:
 Please enter your input for the [0][0] position:
 0
 Please enter your input for the [0][1] position:
 0
 Please enter your input for the [0][2] position:
 0
 Please enter your input for the [0][3] position:
 0
 Please enter your input for the [0][4] position:
 0
 Please enter your input for the [0][5] position:
 0
 Please enter your input for the [0][6] position:
 0
 Please enter your input for the [0][7] position:
 0
 Please enter your input for the [0][8] position:
 0
 Please enter your input for the [0][9] position:
 0
 Please enter your input for the [1][0] position:
 0
 Please enter your input for the [1][1] position:
 0
 Please enter your input for the [1][2] position:
 0
 Please enter your input for the [1][3] position:
 0
 Please enter your input for the [1][4] position:
 0
 Please enter your input for the [1][5] position:
 0
 Please enter your input for the [1][6] position:
 0
 Please enter your input for the [1][7] position:
 0
 Please enter your input for the [1][8] position:
 0
 Please enter your input for the [1][9] position:
 0
 Please enter your input for the [2][0] position:
 0
 Please enter your input for the [2][1] position:
 0
 Please enter your input for the [2][2] position:
 0
 Please enter your input for the [2][3] position:
 0
 Please enter your input for the [2][4] position:
 0
 Please enter your input for the [2][5] position:
 0
 Please enter your input for the [2][6] position:
 0
 Please enter your input for the [2][7] position:
 0
 Please enter your input for the [2][8] position:
 0
 Please enter your input for the [2][9] position:
 0
 Please enter your input for the [3][0] position:
 0
 Please enter your input for the [3][1] position:
 0
 Please enter your input for the [3][2] position:
 0
 Please enter your input for the [3][3] position:
 0
 Please enter your input for the [3][4] position:
 0
 Please enter your input for the [3][5] position:
 0
 Please enter your input for the [3][6] position:
 0
 Please enter your input for the [3][7] position:
 0
 Please enter your input for the [3][8] position:
 0
 Please enter your input for the [3][9] position:
 0
 Please enter your input for the [4][0] position:
 0
 Please enter your input for the [4][1] position:
 0
 Please enter your input for the [4][2] position:
 0
 Please enter your input for the [4][3] position:
 0
 Please enter your input for the [4][4] position:
 0
 Please enter your input for the [4][5] position:
 1
 Please enter your input for the [4][6] position:
 0
 Please enter your input for the [4][7] position:
 0
 Please enter your input for the [4][8] position:
 0
 Please enter your input for the [4][9] position:
 0
 Please enter your input for the [5][0] position:
 0
 Please enter your input for the [5][1] position:
 0
 Please enter your input for the [5][2] position:
 0
 Please enter your input for the [5][3] position:
 1
 Please enter your input for the [5][4] position:
 0
 Please enter your input for the [5][5] position:
 0
 Please enter your input for the [5][6] position:
 0
 Please enter your input for the [5][7] position:
 1
 Please enter your input for the [5][8] position:
 0
 Please enter your input for the [5][9] position:
 0
 Please enter your input for the [6][0] position:
 0
 Please enter your input for the [6][1] position:
 0
 Please enter your input for the [6][2] position:
 1
 Please enter your input for the [6][3] position:
 0
 Please enter your input for the [6][4] position:
 0
 Please enter your input for the [6][5] position:
 0
 Please enter your input for the [6][6] position:
 0
 Please enter your input for the [6][7] position:
 0
 Please enter your input for the [6][8] position:
 0
 Please enter your input for the [6][9] position:
 0
 Please enter your input for the [7][0] position:
 0
 Please enter your input for the [7][1] position:
 0
 Please enter your input for the [7][2] position:
 1
 Please enter your input for the [7][3] position:
 0
 Please enter your input for the [7][4] position:
 0
 Please enter your input for the [7][5] position:
 0
 Please enter your input for the [7][6] position:
 0
 Please enter your input for the [7][7] position:
 1
 Please enter your input for the [7][8] position:
 0
 Please enter your input for the [7][9] position:
 0
 Please enter your input for the [8][0] position:
 0
 Please enter your input for the [8][1] position:
 0
 Please enter your input for the [8][2] position:
 1
 Please enter your input for the [8][3] position:
 1
 Please enter your input for the [8][4] position:
 1
 Please enter your input for the [8][5] position:
 1
 Please enter your input for the [8][6] position:
 1
 Please enter your input for the [8][7] position:
 0
 Please enter your input for the [8][8] position:
 0
 Please enter your input for the [8][9] position:
 0
 Please enter your input for the [9][0] position:
 0
 Please enter your input for the [9][1] position:
 0
 Please enter your input for the [9][2] position:
 0
 Please enter your input for the [9][3] position:
 0
 Please enter your input for the [9][4] position:
 0
 Please enter your input for the [9][5] position:
 0
 Please enter your input for the [9][6] position:
 0
 Please enter your input for the [9][7] position:
 0
 Please enter your input for the [9][8] position:
 0
 Please enter your input for the [9][9] position:
 0
 How many repeats do you want the game to do? (max 200)
 12







 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  *  .  .  .  . )
 ( .  .  .  *  .  .  .  *  .  . )
 ( .  .  *  .  .  .  .  .  .  . )
 ( .  .  *  .  .  .  .  *  .  . )
 ( .  .  *  *  *  *  *  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )














 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  *  *  .  .  .  .  .  . )
 ( .  *  *  .  *  *  *  .  .  . )
 ( .  .  *  *  *  *  *  .  .  . )
 ( .  .  .  *  *  *  .  .  .  . )














 ( .  .  .  .  *  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  *  *  *  *  *  .  .  .  . )
 ( .  *  .  .  .  .  *  .  .  . )
 ( .  *  .  .  .  .  .  .  .  . )
 ( .  .  *  .  .  .  *  .  .  . )














 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  *  *  *  .  .  .  .  . )
 ( .  *  *  *  *  *  .  .  .  . )
 ( *  *  .  *  *  *  .  .  .  . )
 ( .  *  *  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )














 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  *  .  .  .  .  .  . )
 ( .  *  .  .  .  *  .  .  .  . )
 ( *  .  .  .  .  .  .  .  .  . )
 ( *  .  .  .  .  *  .  .  .  . )
 ( *  *  *  *  *  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )














 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( *  *  .  .  .  .  .  .  .  . )
 ( *  .  *  *  *  .  .  .  .  * )
 ( *  *  *  *  *  .  .  .  .  . )
 ( .  *  *  *  .  .  .  .  .  . )














 ( .  .  *  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( *  *  *  *  .  .  .  .  .  * )
 ( .  .  .  .  *  .  .  .  .  * )
 ( .  .  .  .  .  .  .  .  .  * )
 ( *  .  .  .  *  .  .  .  .  . )














 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( *  *  *  .  .  .  .  .  .  . )
 ( *  *  *  *  .  .  .  .  .  * )
 ( .  *  *  *  .  .  .  .  *  * )
 ( *  .  .  .  .  .  .  .  .  * )
 ( .  .  .  .  .  .  .  .  .  . )














 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  *  .  .  .  .  .  .  .  . )
 ( .  .  .  *  .  .  .  .  .  * )
 ( .  .  .  .  .  .  .  .  *  . )
 ( .  .  .  *  .  .  .  .  *  . )
 ( *  *  *  .  .  .  .  .  *  * )
 ( .  .  .  .  .  .  .  .  .  . )














 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  *  * )
 ( *  *  *  .  .  .  .  *  *  . )
 ( *  *  *  .  .  .  .  .  *  * )
 ( *  *  .  .  .  .  .  .  .  * )














 ( *  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( *  *  .  .  .  .  .  *  *  * )
 ( .  .  *  .  .  .  .  *  .  . )
 ( .  .  .  .  .  .  .  *  .  . )
 ( .  .  *  .  .  .  .  .  *  . )














 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( .  .  .  .  .  .  .  .  .  . )
 ( *  .  .  .  .  .  .  .  *  * )
 ( *  *  .  .  .  .  .  *  *  * )
 ( *  *  .  .  .  .  *  *  .  * )
 ( .  .  .  .  .  .  .  *  *  . )
 ( .  .  .  .  .  .  .  .  .  . )

 Menu:
 Press 1 to start the game from a set of pre-defined configurations
 Press 2 to set a custom board
 Press 3 to exit
 4
 You were meant to enter an interger between 1 and 3.
 Please try again.
 3
 Thank you for using this code.
 Program ended with exit code: 0
 
 */
