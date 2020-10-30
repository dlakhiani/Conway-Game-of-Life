#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* selecting corner */
#define TOPLEFT x==0 && y==0
#define BOTLEFT x==rows-1 && y==0
#define TOPRIGHT x==0 && y==cols-1
#define BOTRIGHT x==rows-1 && y==cols-1

/* scrolling through edges */
#define TOPEDGE x==0 && (y>0 && y<cols-1)
#define LEFTEDGE y==0 && (x>0 && x<rows-1)
#define RIGHTEDGE y==cols-1 && (x>0 && x<rows-1)
#define BOTEDGE x==rows-1 && (y>0 && y<cols-1)

#define CENTER (x>0 && x<rows-1) && (y>0 && y<cols-1)

int main(int argc, char *argv[]) {

  FILE *f;
  int x=0, y=0, i=0, j=0;
  int rows=0, cols=0;
  int countTicks=0; /* program counter to ticks initialized */
  int ticks=0; /* ticks initialized */
  int extraTicks=1; /* when asked to continue by the user, multiplied to initial ticks */
  int printGen=1; /* to print the generation into terminal */
  int changedCells=0; /* if no cells change, terminate program */
  char yORn[2];
  char input[10];

  if (argc != 5) {
    printf("Usage: ./cgol filename.seed #OfTicks rows columns\n");
    return 1;
  } else {
    ticks= atoi(argv[2]);
    rows= atoi(argv[3]);
    cols= atoi(argv[4]);
  }

  /* checking whether the dimensions match the seedFile dimensions */
  int checkRowCol[2][2]= { 0 }; /* to store the dimensions of the seedFile */
  int check[2]= { 0 }; /* [0]=Rows [1]=Cols: if the dimensions are >9 */
  int character;
  int a, b=0;
  for (a = 0; a < strlen(argv[1]); a++) {
    character = argv[1][a];
    if (character >= 48 && character <= 57) {
      checkRowCol[b][0]= character-48; /* to get the ASCII integer back to its originial integer value */
      if (argv[1][a+1] >= 48 && argv[1][a+1] <= 57) { /* to check if the integer is >10 */
        checkRowCol[b][1]= (argv[1][a+1])-48;
        check[b]++;
        a++; /* to now skip this value */
      }
      b++;
    }
  }
  for (b = 0; b < 2; b++) {
    if (check[b]!=0) { /* the dimensions are >9 */
      check[b]= (checkRowCol[b][0]*10) + checkRowCol[b][1];
    } else {
      check[b]= checkRowCol[b][0];
    }
  }
  /* now compare the seedFile dimensions to the user input */
  if (check[0]!=rows || check[1]!=cols) {
    printf("Both of the paramaters inputted must match dimensions of the seedFile.\n");
    return 2;
  }

  /* initialize rows and cols of the grids */
  int alive[rows][cols]; /* store aliveCells around cell in question */
  int currentGrid[rows][cols];
  int futureGrid[rows][cols];

  /* reading in *.seed */
  if ( (f = fopen(argv[1], "r")) != NULL ) {
    /* left to right */
    for (x = 0; x < rows; x++) {
      for (y = 0; y < cols; y++) {
        fscanf(f, "%d", &currentGrid[x][y]);
      }
    }
    fclose(f);

  } else {
    printf ( "Cannot open\n" );
    return 2;
  }

  while (countTicks <= ticks*extraTicks) {
    /* printing the currentGrid */
    printGen=1;
    if (printGen==1) {
      system("sleep 0.25");
      system("clear");
      /* top border */
      printf(" ");
      for (y = 0; y < cols; y++) {
        printf("-");
      }
      printf(" \n");
      /* left to right */
      for (x = 0; x < rows; x++) {
        printf("|");
        for (y = 0; y < cols; y++) {
          if (currentGrid[x][y]==1) { /* converting to X according to 1 ---- "alive" */
            printf("X");
          } else {
            printf(" ");
          }
        }
        printf("|");
        printf("\n");
      }
      /* bottom border */
      printf(" ");
      for (y = 0; y < cols; y++) {
        printf("-");
      }
      printf("%d\n", countTicks);

      if (countTicks==0 && printGen==1) {
        do { /* errorCheck */
          input[0]= '\0';
          yORn[0]= '\0';
          printf("Start? (y or n) ");
          fgets(input, 20, stdin);
          sscanf(input, "%s", yORn);

          if ((strcmp("Y\0", yORn)==0) || (strcmp("y\0", yORn)==0)) {
            printGen=2;
            break;
          } else if ((strcmp("N\0", yORn)==0) || (strcmp("n\0", yORn)==0)) {
            countTicks=ticks+3;
            break;
          } else {
            printf("Only the single characters y or n are acceptable.\n");
            continue; /* not valid */
          }
        } while(printGen==1);
      }
      else {
        printGen=2;
      }

    } /* initial generation (printGen=1) */

    if (printGen==2) {
      changedCells=0;
      /* the tick evaluations to futureGrid -- neighbour checks have been applied from similar methods on www.stackoverflow.com */
      for (x = 0; x < rows; x++) {
        for (y = 0; y < cols; y++) {
          alive[x][y]=0;
          /* corner neigbours */
          if (TOPLEFT) {
            for (i = 0; i < 2; i++) {
              for (j = 0; j < 2; j++) {
                if (currentGrid[x+i][y+j]==1) {
                  alive[x][y]++;
                }
              }
            }
          } else if (BOTLEFT) {
            for (i = -1; i < 1; i++) {
              for (j = 0; j < 2; j++) {
                if (currentGrid[x+i][y+j]==1) {
                  alive[x][y]++;
                }
              }
            }
          } else if (TOPRIGHT) {
            for (i = 0; i < 2; i++) {
              for (j = -1; j < 1; j++) {
                if (currentGrid[x+i][y+j]==1) {
                  alive[x][y]++;
                }
              }
            }
          } else if (BOTRIGHT) {
            for (i = -1; i < 1; i++) {
              for (j = -1; j < 1; j++) {
                if (currentGrid[x+i][y+j]==1) {
                  alive[x][y]++;
                }
              }
            }
          }

          /* edge neighbours */
          if (TOPEDGE) {
            for (i = -1; i < 2; i++) {
              for (j = 0; j < 2; j++) {
                if (currentGrid[x+j][y+i]==1) {
                  alive[x][y]++;
                }
              }
            }
          } else if (LEFTEDGE) {
            for (i = 0; i < 2; i++) {
              for (j = -1; j < 2; j++) {
                if (currentGrid[x+j][y+i]==1) {
                  alive[x][y]++;
                }
              }
            }
          } else if (RIGHTEDGE) {
            for (i = -1; i < 1; i++) {
              for (j = -1; j < 2; j++) {
                if (currentGrid[x+j][y+i]==1) {
                  alive[x][y]++;
                }
              }
            }
          } else if (BOTEDGE) {
            for (i = -1; i < 2; i++) {
              for (j = -1; j < 1; j++) {
                if (currentGrid[x+j][y+i]==1) {
                  alive[x][y]++;
                }
              }
            }
          }

          /* center (1 < x < rows-1) && (1 < y < cols-1) */
          if (CENTER) {
            for (i = -1; i < 2; i++) {
              for (j = -1; j < 2; j++) {
                if (currentGrid[x+i][y+j]==1) {
                  alive[x][y]++;
                }
              }
            }
          }

          /* since the cell selected is not a neighbour of itself, thus remove one neighbour if live cell */
          if (currentGrid[x][y]==1) {
            alive[x][y]--;
          }

          /* RULES!! */
          if (currentGrid[x][y]==1 && alive[x][y]<2) { /* live cell has <2 neighbours -- dead */
            futureGrid[x][y]=0;
            changedCells++;
          } else if (currentGrid[x][y]==1 && alive[x][y]>3) { /* live cell has <3 neighbours -- dead */
            futureGrid[x][y]=0;
            changedCells++;
          } else if (currentGrid[x][y]==0 && alive[x][y]==3) { /* cell has 3 neighbours and is dead -- alive*/
            futureGrid[x][y]=1;
            changedCells++;
          } else { /* cell lives on since enough neigbours -- OR -- is dead but not have 3 neighbours. Thus, nothing changed */
            futureGrid[x][y]=currentGrid[x][y];
          }
        } /* evaluate cols [y] */
      } /* evaluate rows [x] */

      /* copying all of the futureGrid into the currentGrid */
      if (changedCells>0) {
        for (x = 0; x < rows; x++) {
          for (y = 0; y < cols; y++) {
            currentGrid[x][y]=futureGrid[x][y];
          }
        }
      } else { /* the generation did not change, thus terminate */
        countTicks=(ticks*extraTicks)+3;
      }
    } /* evaluate new generation (printGen=2) */

    if (countTicks==ticks*extraTicks) {
      do { /* errorCheck */
        input[0]= '\0';
        yORn[0]= '\0';
        printf("Continue? (y or n) ");
        fgets(input, 20, stdin);
        sscanf(input, "%s", yORn);

        if ((strcmp("Y\0", yORn)==0) || (strcmp("y\0", yORn)==0)) {
          extraTicks++;
          break;
        } else if ((strcmp("N\0", yORn)==0) || (strcmp("n\0", yORn)==0)) {
          countTicks=(ticks*extraTicks)+3;
          break;
        } else {
          printf("Only the single characters y or n are acceptable.\n");
          continue; /* not valid */
        }
      } while(countTicks<= ticks*extraTicks);
    }
    countTicks++;
  } /* terminate cgol */

  return 0;
}
