#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

void setBufferedInput(bool);
void signal_callback_handler(int);
void generate_grid();
void show_grid();
void show_point_moves();
void rotate_grid();
int move_grid();
void move_right();
void move_down();
void move_up();
void move_left();
void add_random();
int check_loose();
int check_win();
int grid[4][4];
// int grid[4][4]= {{2,4,16,32},
//                  {4,16,1024,2},
//                  {16,32,5121,4},
//                  {2,4,2,32}};
int points = 0;
int total_moves = 0;
int moved = 0;

int main(){
  signal(SIGINT, signal_callback_handler);
  setBufferedInput(false);
  printf("\033[2J\033[H" );

  generate_grid();
  show_point_moves();
  printf("\n");
  show_grid();
  printf("'w, a, s, d' to control and 'q' to quit\n");

  char c;
  while (true) {
    if(check_win()){
      printf("\n");
      printf("      Congratulations You Won!\n");
      printf("       Points: %d | Moves: %d\n", points, total_moves);
      break;
    }

    if(check_loose()){
      printf("\n");
      printf("    You Lost, Try Again Next Time.\n");
      printf("       Points: %d | Moves: %d\n", points, total_moves);
      break;
    }
    c=getchar();
    if(c == 'w'){
      move_up();
    }
    if(c == 'a'){
      move_left();
    }
    if(c == 's'){
      move_down();
    }
    if (c == 'd'){
      move_right();
    }
    if(c == 'q'){
      printf("ending the game\n");
      break;
    }
    printf("\033[2J\033[H" );
    show_point_moves();
    printf("\n");
    show_grid();
    printf("'w, a, s, d' to control and 'q' to quit\n");
  }
    setBufferedInput(true);
}
void move_right(){
  printf("\033[2J\033[H" );
  
  if(move_grid()){
    add_random();
    add_random();
  }
  show_point_moves();
  printf("\n");
  show_grid();
}

void move_left(){
  printf("\033[2J\033[H" );

  rotate_grid();
  rotate_grid();
  if(move_grid()){
    add_random();
    add_random();
  }
  rotate_grid();
  rotate_grid();
  show_point_moves();
  printf("\n");
  show_grid();
}

void move_down(){
  printf("\033[2J\033[H" );

  rotate_grid();
  if(move_grid()){
    add_random();
    add_random();
  }
  rotate_grid();
  rotate_grid();
  rotate_grid();
  show_point_moves();
  printf("\n");
  show_grid();
}

void move_up(){
  printf("\033[2J\033[H" );

  rotate_grid();
  rotate_grid();
  rotate_grid();
  if(move_grid()){
    add_random();
    add_random();
  }  
  rotate_grid();
  show_point_moves();
  printf("\n");
  show_grid();
}

void generate_grid(){
  // generate empty grid
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      grid[i][j] = 0;
    }
  }
  add_random();
  add_random();
  
}

void add_random(){
    srand((unsigned)time(NULL));

    int cnt_empty = 0;
    int empty_place[16][2];
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(grid[i][j] == 0){
              empty_place[cnt_empty][0] = i;
              empty_place[cnt_empty][1] = j;
              cnt_empty++;
            }
        }
    }

    int r1, c1, r2, c2, value1, value2;
    // assign value
    if(rand() % 2){
        value1 = 4;
    }else{
        value1 = 2;
    }

    if(cnt_empty > 0){
      int temp = rand() % cnt_empty;
      r1 = empty_place[temp][0];
      c1 = empty_place[temp][1];
      grid[r1][c1] = value1;
    }
}

void show_grid(){
  for(int i = 0; i < 4; i++){
    printf("   |------|------|------|------|\n");
    printf("\n");
    for(int j = 0; j < 4; j++){
      printf("%*d", 7, grid[i][j]);
    }
    printf("\n");
    printf("\n");
  }
  printf("   |------|------|------|------|\n");
  printf("\n");
}

void show_point_moves(){
  printf("   Points: %d %*s%d\n", points, 18,"Moves: ", total_moves);
}

/*
idea from https://www.geeksforgeeks.org/inplace-rotate-square-matrix-by-90-degrees/
*/
void rotate_grid(){
  int temp = 0;
  int k = 4; //size of grid
  for (int i=0; i<k; i++) {
      for (int j=i; j<k-i-1; j++) {
          temp = grid[i][j];
          grid[i][j] = grid[j][k-i-1];
          grid[j][k-i-1] = grid[k-i-1][k-j-1];
          grid[k-i-1][k-j-1] = grid[k-j-1][i];
          grid[k-j-1][i] = temp;
      }
  }
}

int move_grid(){ //considering moving right is default
  int current, next;
  bool moved = false; //used to check if the val has moved
  for(int r = 0; r < 4; r++){
    for(int c = 3; c > -1; c--){
      if(grid[r][c] != 0){
        current = c;
        next = c + 1;

        // move until end or next val is not 0
        while(next < 4 && grid[r][next] == 0){
          grid[r][next] = grid[r][current];
          grid[r][current] = 0;
          current += 1;
          next += 1;
          moved = true;
        }

        // add when next val is same as current val
        if(next < 4 && (grid[r][current] == grid[r][next])){
          grid[r][next] *= 2;
          grid[r][current] = 0;
          moved = true;
          points += grid[r][next];
        }
      }
    }
  }
  total_moves++;
  return moved;
}

// check if the player lost the game
int check_loose(){
  bool condi = true;
  // check if no same number across the row
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 3; j++){
      if(grid[i][j] == grid[i][j+1]){
        condi = false;
      }
    }
  }
  // check if no same number across the column
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 4; j++){
      if(grid[i][j] == grid[i+1][j]){
        condi = false;
      }
    }
  }
  // check if there is zero
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      if(grid[i][j] ==0){
        condi = false;
      }
    }
  }
  return condi;
}

//check if the player won the game
int check_win(){
  bool condi = false;
      for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(grid[i][j] == 2048){
                condi = true;
                return condi;
            }
        }
    }
    return condi;
}

// code from Maurits van der Schee: 2048.c
void signal_callback_handler(int signum) {
    printf("         TERMINATED         \n");
    setBufferedInput(true);
    printf("\033[?25h\033[m");
    exit(signum);
}

void setBufferedInput(bool enable) {
    static bool enabled = true;
    static struct termios old;
    struct termios new;

    if (enable && !enabled) {
        // restore the former settings
        tcsetattr(STDIN_FILENO,TCSANOW,&old);
        // set the new state
        enabled = true;
    } else if (!enable && enabled) {
        // get the terminal settings for standard input
        tcgetattr(STDIN_FILENO,&new);
        // we want to keep the old setting to restore them at the end
        old = new;
        // disable canonical mode (buffered i/o) and local echo
        new.c_lflag &=(~ICANON & ~ECHO);
        // set the new settings immediately
        tcsetattr(STDIN_FILENO,TCSANOW,&new);
        // set the new state
        enabled = false;
    }
}
