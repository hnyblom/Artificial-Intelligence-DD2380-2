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
    vector<vector<int>> xRows(6, vector<int>(4));
    vector<vector<int>> xNARows(6, vector<int>(4));
    vector<vector<int>> xCols(6, vector<int>(4));
    vector<vector<int>> xNACols(6, vector<int>(4));
    vector<vector<int>> xDi(6, vector<int>(2));
    vector<vector<int>> xNADi(6, vector<int>(2));
    vector<vector<int>> yRows(6, vector<int>(4));
    vector<vector<int>> yNARows(6, vector<int>(4));
    vector<vector<int>> yCols(6, vector<int>(4));
    vector<vector<int>> yNACols(6, vector<int>(4));
    vector<vector<int>> yDi(6, vector<int>(2));
    vector<vector<int>> yNADi(6, vector<int>(2));
    vector<vector<int>> xScoreRows(6, vector<int>(4));
    vector<vector<int>> yScoreRows(6, vector<int>(4));
    vector<vector<int>> xScoreCols(6, vector<int>(4));
    vector<vector<int>> yScoreCols(6, vector<int>(4));
    vector<vector<int>> xScoreDi(6, vector<int>(2));
    vector<vector<int>> yScoreDi(6, vector<int>(2));
    vector<vector<int>> xStopRowDanger(6, vector<int>(4));
    vector<vector<int>> xStopColDanger(6, vector<int>(4));
    vector<vector<int>> yStopRowDanger(6, vector<int>(4));
    vector<vector<int>> yStopColDanger(6, vector<int>(4));
    vector<vector<int>> xStopDiDanger(6, vector<int>(2));
    vector<vector<int>> yStopDiDanger(6, vector<int>(2));

    int xRowChances =24;
    int xColChances =24;
    int xDiChances =12;
    int yRowChances =24;
    int yColChances =24;
    int yDiChances =12;
    
    for(int l=0; l<6; ++l){
        for(int j=0;j<4;++j){
            xRows[l][j]=1;
            xCols[l][j]=1;  

            yRows[l][j]=1;
            yCols[l][j]=1; 
        }       
    }
    for (int l=0;l<6;++l){
        for(int j=0;j<2;++j){
            xDi[l][j]=1;
            yDi[l][j]=1;
        }
    }
    
    //cerr<<"yRowChances: "<<yRowChances<<"\n";
    for(int l=0;l<6;++l){
        for(int i=0;i<4;++i){
            for(int j=0;j<4;++j){ 
                cerr<<"\nChecking chances \n";
                if(pState.at(i,j,l)==CELL_X){
                    cerr<<"\n chances X \n";
                    if(rowDanger(pState, i, j, l, CELL_O)){
                        xStopRowDanger[l][i]=1;
                    }
                    if(colDanger(pState, i, j, l, CELL_O)){
                        xStopColDanger[l][i]=1;
                    }
                    if(yRows[l][i]!=0){
                       yRows[l][i]=0;
                       --yRowChances;
                       //cerr<<"yRowChances: "<<yRowChances<<"\n";
                    }
                    if(yCols[l][j]!=0){
                      yCols[l][j]=0;
                      --yColChances;
                      //cerr<<"yColChances: "<<yColChances<<"\n";
                    }
                    xNARows[l][i]+=1;
                    xNACols[l][j]+=1;

                    int diT = isDiag(i, j);
                    if(diDanger(pState,i,j,l,CELL_O)){
                        xStopDiDanger[l][diT]=1;
                    }
                    if(diT!=2){
                        xNADi[l][diT]+=1;
                        if(yDi[l][diT]!=0){
                            yDi[l][diT]=0;
                            --yDiChances;
                            //cerr<<"yDiChances: "<<yDiChances<<"\n";
                        }
                   }
                }
                if(pState.at(i,j,l)==CELL_O){
                    cerr<<"\n chances O \n";
                    if(rowDanger(pState, i, j, l, CELL_X)){
                        yStopRowDanger[l][i]=1;
                    }
                    if(colDanger(pState, i, j, l, CELL_X)){
                        yStopColDanger[l][i]=1;
                    }
                    if(xRows[l][i]!=0){
                        xRows[l][i]=0;
                        --xRowChances;
                    }
                    if(xCols[l][j]!=0){
                       xCols[l][j]=0;
                        --xColChances; 
                    }
                    yNARows[l][i]+=1;
                    yNACols[l][j]+=1;
                    int diT = isDiag(i, j);
                    if(diDanger(pState,i,j,l,CELL_X)){
                        yStopDiDanger[l][diT]=1;
                    }
                    if(diT!=2){
                        yNADi[l][diT]+=1;
                        if(xDi[l][diT]!=0){
                            xDi[l][diT]=0;
                            --xDiChances;
                        }
                    }
                }
            }
        }
    }
    //Score how players are doing
    for(int l=0;l<6;++l){
        for(int i=0;i<4;++i){
            for(int j=0;j<4;++j){
                cerr<<"\n scoring \n";
                if(pState.at(i,j,l)==CELL_X){     
                    //X ensam på rad/column
                    if(xRows[i][l]!=0){xScoreRows[l][i]+=1;}
                    if(xCols[j][l]!=0){xScoreCols[l][j]+=1;}
                    //Stoppa motståndare med 3 poäng

                    //X Ensam på diagonal
                    int diT = isDiag(i, j);
                    if(diT!=2){
                        if(xDi[l][diT]!=0){xScoreDi[l][diT]+=1;}                   
                    }                            
                }
                if(pState.at(i,j,l)==CELL_O){
                    //Y ensam på rad/column
                    if(yRows[l][i]!=0){yScoreRows[l][i]+=1;}
                    if(yCols[l][j]!=0){yScoreCols[l][j]+=1;}
                    //Y ensam på rad/column
                    int diT = isDiag(i, j);
                    if(diT!=2){
                        if(yDi[l][diT]!=0){yScoreDi[l][diT]+=1;}           
                    }                 
                }
            }
        }
    }
    
    //Calculate nr rows&&cols&&diag with more than 2 - dead rows&&cols&&diag
    int good =0;
    int bad =0;
    //cerr<<"\nrx, cx, ry, cy \n";
    for(int l=0;l<6;++l){
    for(int k=0;k<4;++k){
        //Score +1 bara om ensam på rad
        int rx = xScoreRows[l][k];
        int cx = xScoreCols[l][k];              
        int ry = yScoreRows[l][k];
        int cy = yScoreCols[l][k];
        int rxd = xStopRowDanger[l][k];
        int cxd = xStopColDanger[l][k];
        int ryd = yStopRowDanger[l][k];
        int cyd = yStopColDanger[l][k];
        
        
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
            int dx = xScoreDi[l][k];       
            int dy = yScoreDi[l][k];
            int dxd = xStopDiDanger[l][k];
            int dyd = yStopDiDanger[l][k];
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

    cerr<<"\n good - bad \n";
    cerr<<good-bad<<"\n";
    cerr<<"\n chances \n";
    cerr<<chancesLeft<<"\n";
    
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
