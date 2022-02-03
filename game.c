#include "stdio.h"                // Mateus Rico
#include "stdlib.h"               // Fabrício Lourran
#include <math.h>                 // André Renato Villela
#include <unistd.h>

#define MAX_NIVEL 6

struct play{
    int line,column;
};

struct position{
    int board[8][8];
    int inturnplayer;
};

struct element{
  struct play ply;
  struct element *next,*prev;
};

#include "node_reversi.h"


void openn(){
  printf("               =------------------------------------------------------=\n");
  printf("               |--------------|||/WELCOME TO REVERSI/|||--------------|\n");
  printf("               =------------------------------------------------------=\n");

  printf("\n");
  printf("THE GAME:\n\nThe game is divided between black(B) and white(W) players. Each play consists of placing a piece\non any empty square on the board, in order to make a 'sandwich' (in any direction)\nwith at least one opponent's piece.\n\nAll pieces that are in these sandwiches belong to the opponent. If, at any time, the current player cannot make a play, he passes his turn to the opponent without placing any pieces. If both players can no longer place pieces, the game ends.\n\nThe game also ends when all the squares on the board have some pieces. At the end,\nThe player who ends up with the most pieces wins.\n");
  printf("\n\n                        Enter the positions like the example below\n");
  printf("                       --> 'Enter the line and the column:5 6'   <--                       \n");

}

void Thinking(){
    printf("\nAI thinking");
    sleep(1);
    printf(".");
    sleep(1);
    printf(".");
    sleep(1);
    printf(".\n");
}

struct position InitializeBoard(){
    struct position aux;
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            aux.board[i][j]=0;
        }
    }
    aux.board[3][3] = aux.board[4][4] = 1;
    aux.board[3][4] = aux.board[4][3] = -1;

    aux.inturnplayer = -1;

    return aux;
}

void DrawBoard(struct position pos, int mode){
  printf("  0   1   2   3   4   5   6   7\n");
  for(int i=0;i<8;i++)
  {
    printf("%d ",i);
    for(int j=0;j<8;j++)
    {

      if(pos.board[i][j]==0)
        printf("  | ",pos.board[i][j]);
      else
      {
        if(pos.board[i][j]==1)
          printf("W | ",pos.board[i][j]);
        else
          printf("B | ",pos.board[i][j]);
      }
    }
    printf("\n");
    printf(" ---------------------------------\n");
  }
  if ((pos.inturnplayer==1) && (mode==1)){
      printf("Jogador da vez: brancas\n");
  }else{
    if(mode==1){
      printf("Jogador da vez: pretas\n");
    }
  }
}

int DirectionTest(struct position pos, int line, int column, int deltaL, int deltaC){
    int i=line+deltaL;
    int j=column+deltaC;
    int cont=0;

    while (i>=0 && i<8 && j>=0 && j<8 && pos.board[i][j]==-pos.inturnplayer){
        cont++;
        i += deltaL;
        j += deltaC;
    }

    if (i>=8||i<0||j>=8||j<0){
        cont = 0;
    }else if (pos.board[i][j]==0)
        cont = 0;

    return cont;
}

struct element *CalculateValidPlays(struct position pos){
    struct element *List = Cratenode();
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            int answer = 0;

            for (int deltaL=-1;deltaL<=1;deltaL++){
                for (int deltaC=-1;deltaC<=1;deltaC++){
                    if (deltaL!=0||deltaC!=0){
                        if (DirectionTest(pos,i,j,deltaL,deltaC)){
                            answer=1;
                        }
                    }
                }
            }
            if(pos.board[i][j]==0){
              if (answer)
                List = Insert(List,i,j);
            }
        }
    }

    return List;
}

struct play ChooseMove(struct element *List){
    int line,column;
  //  if (List==NULL) return NULL;

    struct play *aux = NULL;

    do{
        Printlist(List);

        printf("\nEnter the line and the column:");
        scanf("%d %d",&line,&column);
        aux = Search(List,line,column);
    }while(aux==NULL);

    return *aux;
}

void FlipsDisk(struct position *pos, int line, int column, int deltaL, int deltaC){
    int i=line+deltaL;
    int j=column+deltaC;

    while (pos->board[i][j]==-pos->inturnplayer){
        pos->board[i][j] = -pos->board[i][j];
        i += deltaL;
        j += deltaC;
    }
}

void RunPlay(struct position *pos, struct play *ply){

    if (ply!=NULL){
        pos->board[ply->line][ply->column] = pos->inturnplayer;
        for (int deltaL=-1;deltaL<=1;deltaL++){
            for (int deltaC=-1;deltaC<=1;deltaC++){
                if (deltaL!=0||deltaC!=0){
                    if (DirectionTest(*pos,ply->line,ply->column,deltaL,deltaC))
                        FlipsDisk(pos,ply->line,ply->column,deltaL,deltaC);
                }
            }
        }
    }

    pos->inturnplayer = -pos->inturnplayer;
}

void WinnerV(int tab[8][8],int mode){
    int White=0;
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            if (tab[i][j]==1)
                White++;
        }
    }

    if (White==32){
        printf("/////////////////// || TIE || ///////////////////\n");
    }else{
      if(mode==1){
        if (White>32){
           printf("///////////////////|| CONGRATS: WHITE WON ||///////////////////\n");
       }else printf("///////////////////|| CONGRATS: BLACK WON ||///////////////////");
     }else if(mode==2){
       if (White>32){
          printf("/////////////////// || CONGRATS: YOU WON || ///////////////////\n");
      }else printf("/////////////////// || DON'T CRY BABY, THE AI WON || ///////////////////");
     }
    }
}


double EvaluatePosition(struct position pos){
  double w = 0;
  double b = 0;
  double r;
  for(int i = 0; i<8; i++){
    for(int j = 0; j<8; j++){
      if(pos.board[i][j] == 1){
        w++;
        if(i==0 || i==7 || j==0 || j==7){
          w = w + 1;
        }
      }else if(pos.board[i][j] == -1){
        b = b - 1;
        if(i==0 || i==7 || j==0 || j==7){
          b = b - 1;
        }
      }
    }
  }
  r = b + w;
  return r;
}


struct play RunAI(struct position currentlyPos, int level, double alfa, double beta){

    int prune = 0;
    double bestValue,bestMove;
    struct position copyPos;
    struct play bestPly,aiPly;
    struct element *list,*AuxMove;

    if((level%2)==0){
      bestValue  = -INFINITY;
    }else{
      bestValue  = INFINITY;
    }

    bestPly.line = -1;
    bestPly.column = -1;

    list = CalculateValidPlays(currentlyPos);


    if(list==NULL) return bestPly;

    AuxMove = list->next;

    while (AuxMove!=list && prune==0){
        copyPos = currentlyPos;

        if (level<MAX_NIVEL)
        {

            if ((level%2)==0){

                aiPly = RunAI(copyPos,level+1,bestValue,beta);
             }

            else{

                aiPly = RunAI(copyPos,level+1,alfa,bestValue);
            }

            RunPlay(&copyPos,&aiPly);

        }

        bestMove = EvaluatePosition(copyPos);

        if((bestMove<alfa)||(bestMove>beta)){
          prune = 1;
        }


        if(((level%2)==0) && (bestMove>=bestValue)){
          bestValue = bestMove;
          bestPly = list->ply;
        }else if(((level%2)!=0) && (bestMove<=bestValue)){
          bestValue = bestMove;
          bestPly = list->ply;
        }

        AuxMove = AuxMove->next;
    }


    Deletelist(list);

    return bestPly;
}


int main(){
    int aux = 1;
    int game_mode;
    openn();

    while(aux){
      printf("\n\n\n");
      printf("Enter 1 to play with a friend\n");
      printf("Enter 2 to play against the machine\n:");
      scanf("%d",&game_mode);

      int EmptySquares = 60;
      int Winner = 0;

      struct position Realpos = InitializeBoard();
      struct element *Possible_moves=NULL;
      struct play Chosen_move;

      while ((EmptySquares>0) && (Winner!=2)){
          system("cls"); //for windows
          //system("clear"); //for linux
          DrawBoard(Realpos,game_mode);
          if (Realpos.inturnplayer==-1){
              Possible_moves = CalculateValidPlays(Realpos);
              if (Possible_moves!=NULL){
                  Chosen_move = ChooseMove(Possible_moves);

                  RunPlay(&Realpos,&Chosen_move);
                  Deletelist(Possible_moves);
                  EmptySquares--;
                  Winner = 0;
              }else{
                 RunPlay(&Realpos,NULL);
                 Winner++;
              }

          }else{
              if(game_mode==2){
                Thinking();
                Chosen_move = RunAI(Realpos,0,-INFINITY,+INFINITY);
                if (Chosen_move.line==-1){
                  RunPlay(&Realpos,NULL);
                  Winner++;
                }else{
                  RunPlay(&Realpos,&Chosen_move);
                  EmptySquares--;
                  Winner = 0;
                }
              }else if(game_mode==1){
                Possible_moves = CalculateValidPlays(Realpos);
                if (Possible_moves!=NULL){
                    Chosen_move = ChooseMove(Possible_moves);
                    RunPlay(&Realpos,&Chosen_move);
                    Deletelist(Possible_moves);
                    EmptySquares--;
                    Winner = 0;
                }else{
                   RunPlay(&Realpos,NULL);
                   Winner++;
                }
              }
          }

      }
      DrawBoard(Realpos,game_mode);
      WinnerV(Realpos.board,game_mode);

      printf("\n\nWould you like to play again?\n");
      printf("Enter 1 for yes\n");
      printf("Enter 0 for no\n:");
      scanf("%d",&aux);
    }
}
