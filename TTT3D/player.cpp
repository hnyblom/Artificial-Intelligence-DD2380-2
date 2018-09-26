#include "player.hpp"
#include <cstdlib>
#include <cmath>
#include <tuple>
#include <vector>
#include <algorithm> 
#include <limits>
#include <climits>
#include <cstdint>

/*
By: Hanna & Nicole
*/

namespace TICTACTOE3D{
using namespace std;
int rowDanger(const GameState &pState, int i, int j, int l, int cell){
    if(j==0){
        if(pState.at(i,1, l)==cell && pState.at(i,2,l)==cell && pState.at(i,3,l)==cell){
            return true;
        }
    }else if(j==1){
        if(pState.at(i,0,l)==cell && pState.at(i,2,l)==cell && pState.at(i,3,l)==cell){
            return true;
        }
    }else if(j==2){
        if(pState.at(i,0,l)==cell && pState.at(i,1,l)==cell && pState.at(i,3,l)==cell){
            return true;
        }      
    }else if(j==3){
        if(pState.at(i,0,l)==cell && pState.at(i,1,l)==cell && pState.at(i,2,l)==cell){
            return true;
        }        
    }
    return false;
}
int colDanger(const GameState &pState, int i, int j, int l, int cell){
    if(j==0){
        if(pState.at(1,j,l)==cell && pState.at(2,j,l)==cell && pState.at(3,j,l)==cell){
            return true;
        }
    }else if(j==1){
        if(pState.at(0,j,l)==cell && pState.at(2,j,l)==cell && pState.at(3,j,l)==cell){
            return true;
        }
    }else if(j==2){
        if(pState.at(0,j,l)==cell && pState.at(1,j,l)==cell && pState.at(3,j,l)==cell){
            return true;
        }
        
    }else if(j==3){
        if(pState.at(0,j,l)==cell && pState.at(1,j,l)==cell && pState.at(2,j,l)==cell){
            return true;
        }        
    }
    return false;
}
int diDanger(const GameState &pState, int i, int j, int l, int cell){
    if(i==j==0){
        if(pState.at(1,1,l)==cell && pState.at(2,2,l)==cell && pState.at(3,3,l)==cell){
            return true;
        }
    }else if(i==j==1){
        if(pState.at(0,0,l)==cell && pState.at(2,2,l)==cell && pState.at(3,3,l)==cell){
            return true;
        }
    }else if(i==j==2){
        if(pState.at(0,0,l)==cell && pState.at(1,1,l)==cell && pState.at(3,3,l)==cell){
            return true;
        }
        
    }else if(i==j==3){
        if(pState.at(0,0,l)==cell && pState.at(1,1,l)==cell && pState.at(2,2,l)==cell){
            return true;
        }        
    }
    else if(i==0&&j==3){
        if(pState.at(1,2,l)==cell && pState.at(2,1,l)==cell && pState.at(3,0,l)==cell){
            return true;
        }
    }else if(i==1&&j==2){
        if(pState.at(0,3,l)==cell && pState.at(2,1,l)==cell && pState.at(3,0,l)==cell){
            return true;
        }
    }else if(i==2&&j==1){
        if(pState.at(0,3,l)==cell && pState.at(1,2,l)==cell && pState.at(3,0,l)==cell){
            return true;
        }
    }else if(i==3&&j==0){
        if(pState.at(0,3,l)==cell && pState.at(1,2,l)==cell && pState.at(2,1,l)==cell){
            return true;
        }
    }
    return false;
}

int isDiag(int i, int j){
    //No diagonal returns 2
    int res=2;
    if(i==j){
        //Diagonal 1 returns 0
        res = 0;
    }else if(i == 0 && j == 3 || i == 1 && j == 2 || i == 2 && j == 1 || i == 3 && j == 0){
        //Diagonal 2 returns 1
        res=1;           
    }
    return res;
}
int gamma(bool player, const GameState &pState){
    //Calculate value of state
    //Possible winning opportunities
    //cerr<<"\n------gamma----\n";
    vector<vector<int>> xRows(4, vector<int>(4));
    vector<vector<int>> xNARows(4, vector<int>(4));
    vector<vector<int>> xCols(4, vector<int>(4));
    vector<vector<int>> xNACols(4, vector<int>(4));
    vector<vector<int>> xDi(2, vector<int>(2));
    vector<vector<int>> xNADi(2, vector<int>(2));
    vector<vector<int>> yRows(4, vector<int>(4));
    vector<vector<int>> yNARows(4, vector<int>(4));
    vector<vector<int>> yCols(4, vector<int>(4));
    vector<vector<int>> yNACols(4, vector<int>(4));
    vector<vector<int>> yDi(2, vector<int>(2));
    vector<vector<int>> yNADi(2, vector<int>(2));
    vector<vector<int>> xScoreRows(4, vector<int>(4));
    vector<vector<int>> yScoreRows(4, vector<int>(4));
    vector<vector<int>> xScoreCols(4, vector<int>(4));
    vector<vector<int>> yScoreCols(4, vector<int>(4));
    vector<vector<int>> xScoreDi(2, vector<int>(2));
    vector<vector<int>> yScoreDi(2, vector<int>(2));
    vector<vector<int>> xStopRowDanger(4, vector<int>(4));
    vector<vector<int>> xStopColDanger(4, vector<int>(4));
    vector<vector<int>> yStopRowDanger(4, vector<int>(4));
    vector<vector<int>> yStopColDanger(4, vector<int>(4));
    vector<vector<int>> xStopDiDanger(2, vector<int>(2));
    vector<vector<int>> yStopDiDanger(2, vector<int>(2));

    int xRowChances =16;
    int xColChances =16;
    int xDiChances =8;
    int yRowChances =16;
    int yColChances =16;
    int yDiChances =8;
    
    for(int i=0; i<4; ++i){
        for(int j=0;j<4;++j){
            xRows[i][j]=1;
            xCols[i][j]=1;  

            yRows[i][j]=1;
            yCols[i][j]=1; 
        }       
    }
    for (int i=0;i<2;++i){
        for(int j=0;j<2;++j){
            xDi[i][j]=1;
            yDi[i][j]=1;
        }
    }
    
    //cerr<<"yRowChances: "<<yRowChances<<"\n";
    for(int l=0;l<4;++l){
        for(int i=0;i<4;++i){
            for(int j=0;j<4;++j){            
                if(pState.at(i,j,l)==CELL_X){
                    if(rowDanger(pState, i, j, l, CELL_O)){
                        xStopRowDanger[i][l]=1;
                    }
                    if(colDanger(pState, i, j, l, CELL_O)){
                        xStopColDanger[i][l]=1;
                    }
                    if(yRows[i][l]!=0){
                       yRows[i][l]=0;
                       --yRowChances;
                       //cerr<<"yRowChances: "<<yRowChances<<"\n";
                    }
                    if(yCols[j][l]!=0){
                      yCols[j][l]=0;
                      --yColChances;
                      //cerr<<"yColChances: "<<yColChances<<"\n";
                    }
                    xNARows[i][l]+=1;
                    xNACols[j][l]+=1;

                    int diT = isDiag(i, j);
                    if(diDanger(pState,i,j,l,CELL_O)){
                        xStopDiDanger[diT][l]=1;
                    }
                    if(diT!=2){
                        xNADi[diT][l]+=1;
                        if(yDi[diT][l]!=0){
                            yDi[diT][l]=0;
                            --yDiChances;
                            //cerr<<"yDiChances: "<<yDiChances<<"\n";
                        }
                   }
                }
                if(pState.at(i,j,l)==CELL_O){
                    if(rowDanger(pState, i, j, l, CELL_X)){
                        yStopRowDanger[i][l]=1;
                    }
                    if(colDanger(pState, i, j, l, CELL_X)){
                        yStopColDanger[i][l]=1;
                    }
                    if(xRows[i][l]!=0){
                        xRows[i][l]=0;
                        --xRowChances;
                    }
                    if(xCols[j][l]!=0){
                       xCols[j][l]=0;
                        --xColChances; 
                    }
                    yNARows[i][l]+=1;
                    yNACols[j][l]+=1;
                    int diT = isDiag(i, j);
                    if(diDanger(pState,i,j,l,CELL_X)){
                        yStopDiDanger[diT][l]=1;
                    }
                    if(diT!=2){
                        yNADi[diT][l]+=1;
                        if(xDi[diT][l]!=0){
                            xDi[diT][l]=0;
                            --xDiChances;
                        }
                    }
                }
            }
        }
    }
    //Score how players are doing
    for(int l=0;l<4;++l){
        for(int i=0;i<4;++i){
            for(int j=0;j<4;++j){          
                if(pState.at(i,j,l)==CELL_X){     
                    //X ensam på rad/column
                    if(xRows[i][l]!=0){xScoreRows[i][l]+=1;}
                    if(xCols[j][l]!=0){xScoreCols[j][l]+=1;}
                    //Stoppa motståndare med 3 poäng

                    //X Ensam på diagonal
                    int diT = isDiag(i, j);
                    if(diT!=2){
                        if(xDi[diT][l]!=0){xScoreDi[diT][l]+=1;}                   
                    }                            
                }
                if(pState.at(i,j,l)==CELL_O){
                    //Y ensam på rad/column
                    if(yRows[i][l]!=0){yScoreRows[i][l]+=1;}
                    if(yCols[j][l]!=0){yScoreCols[j][l]+=1;}
                    //Y ensam på rad/column
                    int diT = isDiag(i, j);
                    if(diT!=2){
                        if(yDi[diT][l]!=0){yScoreDi[diT][l]+=1;}           
                    }                 
                }
            }
        }
    }
    
    //Calculate nr rows&&cols&&diag with more than 2 - dead rows&&cols&&diag
    int good =0;
    int bad =0;
    //cerr<<"\nrx, cx, ry, cy \n";
    for(int l=0;l<4;++l){
    for(int k=0;k<4;++k){
        //Score +1 bara om ensam på rad
        int rx = xScoreRows[k][l];
        int cx = xScoreCols[k][l];              
        int ry = yScoreRows[k][l];
        int cy = yScoreCols[k][l];
        int rxd = xStopRowDanger[k][l];
        int cxd = xStopColDanger[k][l];
        int ryd = yStopRowDanger[k][l];
        int cyd = yStopColDanger[k][l];
        
        
        //cerr<<" "<<rx<<"  "<<cx<<"  "<<"  "<<ry<<"  "<<cy<<"\n";
        if(player){
            if(rx==1){good+=0;}
            if(cx==1){good+=0;}
            if(rx==2){good+=5;}
            if(cx==2){good+=5;}
            if(rx==3){good+=10;}
            if(cx==3){good+=10;}
            if(rx==4){good+=1000;}
            if(cx==4){good+=1000;}
            if(rxd==1){good+=100;}
            if(cxd==1){good+=100;}
            
            if(ry==1){bad+=0;}
            if(cy==1){bad+=0;}
            if(ry==2){bad+=50;}
            if(cy==2){bad+=50;}
            if(ry==3){bad+=100;}
            if(cy==3){bad+=100;}
            if(ry==4){bad+=1000;}
            if(cy==4){bad+=1000;}
        }else if(!player){
            if(rx==1){bad+=0;}
            if(cx==1){bad+=0;}
            if(rx==2){bad+=50;}
            if(cx==2){bad+=50;}
            if(rx==3){bad+=100;}
            if(cx==3){bad+=100;}
            if(rx==4){bad+=1000;}
            if(cx==4){bad+=1000;}
            
            if(ry==1){good+=0;}
            if(cy==1){good+=0;}
            if(ry==2){good+=5;}
            if(cy==2){good+=5;}
            if(ry==3){good+=10;}
            if(cy==3){good+=10;}
            if(ry==4){good+=1000;}
            if(cy==4){good+=1000;}
            if(ryd==1){good+=100;}
            if(cyd==1){good+=100;}
        }          
    }
    for(int l=0;l<4;++l){
        for(int k=0;k<2;++k){
            int dx = xScoreDi[k][l];       
            int dy = yScoreDi[k][l];
            int dxd = xStopDiDanger[k][l];
            int dyd = yStopDiDanger[k][l];
            //Player X
            if(player){
                if(dx==1){good+=1;}
                if(dx==2){good+=5;}
                if(dx==3){good+=10;}
                if(dx==4){good+=1000;}
                if(dxd==1){good+=100;}

    //            
                if(dy==1){bad+=10;}
                if(dy==2){bad+=50;}
                if(dy==3){bad+=100;}
                if(dy==4){bad+=1000;}
            //Player O
            }else if(!player){
                if(dx==1){bad+=10;}
                if(dx==2){bad+=50;}
                if(dx==3){bad+=100;}
                if(dx==4){bad+=1000;}

                if(dy==1){good+=1;}
                if(dy==2){good+=5;}
                if(dy==3){good+=10;}
                if(dy==4){good+=1000;}
                if(dyd==1){good+=100;}
            }       
        }
    }
    int chancesLeft=0;
    if(player){
        chancesLeft = xRowChances+xColChances+xDiChances;
    }else if(!player){
        chancesLeft = yRowChances+yColChances+yDiChances;
    }

   // cerr<<"\n good - bad \n";
    //cerr<<good-bad<<"\n";
    //cerr<<"\n chances \n";
    //cerr<<chancesLeft<<"\n";
    
    return good-bad+(chancesLeft*50);

    }
  }
vector<GameState> mu(const GameState &pState){
    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);
    return lNextStates;
}
//Spelare A=1, Spelare B=0
int minMax(const GameState &pState, bool playerBool, int depth, int alfa, int beta){
    int v;
    if(pState.isEOG() || depth == 0){
        v=(gamma(playerBool, pState));
    }else if(playerBool){
        //X-player
        v = INT_MIN;
        vector<GameState> States1 = mu(pState);
        for(int i=0;i<States1.size();++i){
            GameState child = States1[i];
            v = max(v,minMax(child, false, depth-1, alfa, beta));
            alfa = max(alfa, v);
            if(beta<=alfa){
                break;
            }
        }
    //O-player
    }else{
        v = INT_MAX;
        vector<GameState> States2 = mu(pState);
        for(int j=0;j<States2.size();++j){
            GameState child = States2[j];
            v = min(v, minMax(child, true, depth-1, alfa, beta));
            beta = min(beta, v);
            if(beta<=alfa){
                break;
            }
        }
    }
    return v;
}

GameState Player::play(const GameState &pState,const Deadline &pDue)
{
    //std::cerr << "Processing " << pState.toMessage() << std::endl;

    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);

    
    if (lNextStates.size() == 0) return GameState(pState, Move());

    /*
     * Here you should write your clever algorithms to get the best next move, ie the best
     * next state. This skeleton returns a random move instead.
     */
    
    int max = 0;
    int stateIndex=0;
    for(int i=0;i<lNextStates.size();++i){
        int newMax = minMax(lNextStates[i], false, 2, INT_MIN, INT_MAX);
        if(newMax>max){
            max=newMax;
            stateIndex = i;
        }
    }
    return lNextStates[stateIndex];
}

/*namespace TICTACTOE3D*/ }
