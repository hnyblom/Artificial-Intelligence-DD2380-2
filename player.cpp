#include "player.hpp"
#include <cstdlib>
#include <cmath>
#include <tuple>
#include <vector>
#include <algorithm> 
#include <limits>
#include <cstdint>

namespace TICTACTOE {
using namespace std;
vector<vector<int> > ticTac(4, vector<int>(4));
int pruning=0;

//om raden och kolumnen är död, placera inte där
//int isDead(int row, int col){
//    if(std::find(row.begin(), row.end(), x) != row.end()) {
//    /* row contains x */
//       if(std::find(cols.begin(), cols.end(), x) != cols.end()) {
//        /* cols contains x */
//           //Don't place in an intersection of opponent
//           ticTac[row][col] = 0;
//       }  
//    } 
//    return 0;
//}

//Kollar om motståndaren är ett drag ifrån att vinna
//tuple<int, string, string, string, int> danger(){
//    tuple<int, string, string, string, int> final = make_tuple(0,"","","",0);
//    for(int i=0;i<4;++i){
//        int d = opScoreDi[i];
//        int r = opScoreRows[i];
//        int c = opScoreCols[i];
//        
//        if(d==3){            
//            if(r==3){               
//                if(c==3){
//                    tuple<int, string, string, string, int> warning3 = make_tuple(3, "D", "R", "C", i);
//                    final=warning3;
//                }else{
//                   tuple<int, string, string, string, int> warning2 = make_tuple(2, "D", "R", "", i);
//                   final=warning2;
//                }
//            }
//            tuple<int, string, string, string, int> warning1 = make_tuple(1, "D", "", "", i);
//            final=warning1;
//        }else if(r==3){
//            if(c==3){
//                tuple<int, string, string, string, int> warning5 = make_tuple(2, "R", "C", "", i);
//                final=warning5;
//            }else{
//                tuple<int, string, string, string, int> warning4 = make_tuple(1, "R", "", "", i);
//                final=warning4;
//            }
//        }else if(c==3){
//            tuple<int, string, string, string, int> warning6 = make_tuple(1, "C", "", "", i);
//            final=warning6;
//        }
//    }
//    return final;
//}

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

vector<GameState> mu(const GameState &pState){
    //cerr<<"\n------mu----\n";
    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);
    
    return lNextStates;
    
}
int rowDanger(const GameState &pState, int i, int j, int cell){
    if(j==0){
        if(pState.at(i,1)==cell && pState.at(i,2)==cell && pState.at(i,3)==cell){
            return true;
        }
    }else if(j==1){
        if(pState.at(i,0)==cell && pState.at(i,2)==cell && pState.at(i,3)==cell){
            return true;
        }
    }else if(j==2){
        if(pState.at(i,0)==cell && pState.at(i,1)==cell && pState.at(i,3)==cell){
            return true;
        }      
    }else if(j==3){
        if(pState.at(i,0)==cell && pState.at(i,1)==cell && pState.at(i,2)==cell){
            return true;
        }        
    }
    return false;
}
int colDanger(const GameState &pState, int i, int j, int cell){
    if(j==0){
        if(pState.at(1,j)==cell && pState.at(2,j)==cell && pState.at(3,j)==cell){
            return true;
        }
    }else if(j==1){
        if(pState.at(0,j)==cell && pState.at(2,j)==cell && pState.at(3,j)==cell){
            return true;
        }
    }else if(j==2){
        if(pState.at(0,j)==cell && pState.at(1,j)==cell && pState.at(3,j)==cell){
            return true;
        }
        
    }else if(j==3){
        if(pState.at(0,j)==cell && pState.at(1,j)==cell && pState.at(2,j)==cell){
            return true;
        }        
    }
    return false;
}
int diDanger(const GameState &pState, int i, int j, int cell){
    if(i==j==0){
        if(pState.at(1,1)==cell && pState.at(2,2)==cell && pState.at(3,3)==cell){
            return true;
        }
    }else if(i==j==1){
        if(pState.at(0,0)==cell && pState.at(2,2)==cell && pState.at(3,3)==cell){
            return true;
        }
    }else if(i==j==2){
        if(pState.at(0,0)==cell && pState.at(1,1)==cell && pState.at(3,3)==cell){
            return true;
        }
        
    }else if(i==j==3){
        if(pState.at(0,0)==cell && pState.at(1,1)==cell && pState.at(2,2)==cell){
            return true;
        }        
    }
    else if(i==0&&j==3){
        if(pState.at(1,2)==cell && pState.at(2,1)==cell && pState.at(3,0)==cell){
            return true;
        }
    }else if(i==1&&j==2){
        if(pState.at(0,3)==cell && pState.at(2,1)==cell && pState.at(3,0)==cell){
            return true;
        }
    }else if(i==2&&j==1){
        if(pState.at(0,3)==cell && pState.at(1,2)==cell && pState.at(3,0)==cell){
            return true;
        }
    }else if(i==3&&j==0){
        if(pState.at(0,3)==cell && pState.at(1,2)==cell && pState.at(2,1)==cell){
            return true;
        }
    }
    return false;
}

int gamma(bool player, const GameState &pState){
    //Calculate value of state
    //Possible winning opportunities
    //cerr<<"\n------gamma----\n";
    vector<int> xRows(4);
    vector<int> xCols(4);
    vector<int> xDi(2);
    vector<int> yRows(4);
    vector<int> yCols(4);
    vector<int> yDi(2);
    vector<int> xScoreRows(4);
    vector<int> yScoreRows(4);
    vector<int> xScoreCols(4);
    vector<int> yScoreCols(4);
    vector<int> xScoreDi(2);
    vector<int> yScoreDi(2);
    vector<int> xStopRowDanger(4);
    vector<int> xStopColDanger(4);
    vector<int> yStopRowDanger(4);
    vector<int> yStopColDanger(4);
    vector<int> xStopDiDanger(2);
    vector<int> yStopDiDanger(2);

    int xRowChances =4;
    int xColChances =4;
    int xDiChances =2;
    int yRowChances =4;
    int yColChances =4;
    int yDiChances =2;
    
    for(int i=0; i<4; ++i){
        xRows[i]=1;
        xCols[i]=1;  

        yRows[i]=1;
        yCols[i]=1;
    }
    xDi[0]=1;
    xDi[1]=1;
    
    yDi[0]=1;
    yDi[1]=1;
    //cerr<<"yRowChances: "<<yRowChances<<"\n";
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){            
            if(pState.at(i,j)==CELL_X){
                if(rowDanger(pState, i, j, CELL_O)){
                    xStopRowDanger[i]=1;
                }
                if(colDanger(pState, i, j, CELL_O)){
                    xStopColDanger[i]=1;
                }
                if(yRows[i]!=0){
                   yRows[i]=0;
                   --yRowChances;
                   //cerr<<"yRowChances: "<<yRowChances<<"\n";
                }
                if(yCols[j]!=0){
                  yCols[j]=0;
                  --yColChances;
                  //cerr<<"yColChances: "<<yColChances<<"\n";
                }
                             
                int diT = isDiag(i, j);
                if(diDanger(pState,i,j,CELL_O)){
                    xStopDiDanger[diT]=1;
                }
                if(diT!=2){
                    if(yDi[diT]!=0){
                        yDi[diT]=0;
                        --yDiChances;
                        //cerr<<"yDiChances: "<<yDiChances<<"\n";
                    }
               }
            }
            if(pState.at(i,j)==CELL_O){
                if(rowDanger(pState, i, j, CELL_X)){
                    yStopRowDanger[i]=1;
                }
                if(colDanger(pState, i, j, CELL_X)){
                    yStopColDanger[i]=1;
                }
                if(xRows[i]!=0){
                    xRows[i]=0;
                    --xRowChances;
                }
                if(xCols[j]!=0){
                   xCols[j]=0;
                    --xColChances; 
                }
                
                int diT = isDiag(i, j);
                if(diDanger(pState,i,j,CELL_X)){
                    yStopDiDanger[diT]=1;
                }
                if(diT!=2){
                    if(xDi[diT]!=0){
                        xDi[diT]=0;
                        --xDiChances;
                    }
                }
            }
        }
    }
    //Score how players are doing
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){          
            if(pState.at(i,j)==CELL_X){     
                //X ensam på rad/column
                if(xRows[i]!=0){xScoreRows[i]+=1;}
                if(xCols[j]!=0){xScoreCols[j]+=1;}
                //Stoppa motståndare med 3 poäng
                
                //X Ensam på diagonal
                int diT = isDiag(i, j);
                if(diT!=2){
                    if(xDi[diT]!=0){xScoreDi[diT]+=1;}                   
                }                            
            }
            if(pState.at(i,j)==CELL_O){
                //Y ensam på rad/column
                if(yRows[i]!=0){yScoreRows[i]+=1;}
                if(yCols[j]!=0){yScoreCols[j]+=1;}
                //Y ensam på rad/column
                int diT = isDiag(i, j);
                if(diT!=2){
                    if(yDi[diT]!=0){yScoreDi[diT]+=1;}           
                }                 
            }
        }
    }
//    cerr<<"\n xRows: \n";
//    for(int i=0;i<4;++i){               
//        cerr<<xRows[i]<<" ";        
//    }
//    cerr<<"\n xCols: \n";
//    for(int i=0;i<4;++i){
//        cerr<<xCols[i]<<" ";
//    }
//    cerr<<"\n yRows: \n";
//    for(int i=0;i<4;++i){
//        cerr<<yRows[i]<<" ";
//    }
//    cerr<<"\n yCols: \n";
//    for(int i=0;i<4;++i){
//        cerr<<yCols[i]<<" ";
//    }
//    
//    for(int i=0;i<2;++i){
//        cerr<<"\n xDi: \n";
//        cerr<<xDi[i]<<" ";     
//    }
//    cerr<<"\n yDi: \n";
//    for(int i=0;i<2;++i){
//        cerr<<yDi[i]<<" ";  
//    }
    
    //Calculate nr rows&&cols&&diag with more than 2 - dead rows&&cols&&diag
    int good =0;
    int bad =0;
    //cerr<<"\nrx, cx, ry, cy \n";
    for(int k=0;k<4;++k){
        //Score +1 bara om ensam på rad
        int rx = xScoreRows[k];
        int cx = xScoreCols[k];              
        int ry = yScoreRows[k];
        int cy = yScoreCols[k];
        int rxd = xStopRowDanger[k];
        int cxd = xStopColDanger[k];
        int ryd = yStopRowDanger[k];
        int cyd = yStopColDanger[k];
        
        
        //cerr<<" "<<rx<<"  "<<cx<<"  "<<"  "<<ry<<"  "<<cy<<"\n";
        if(!player){
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
        }else if(player){
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
    for(int l=0;l<2;++l){
        int dx = xScoreDi[l];       
        int dy = yScoreDi[l];
        int dxd = xStopDiDanger[l];
        int dyd = yStopDiDanger[l];
        //Player X
        if(!player){
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
        }else if(player){
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
    int chancesLeft=0;
    if(!player){
        chancesLeft = xRowChances+xColChances+xDiChances;
    }else if(player){
        chancesLeft = yRowChances+yColChances+yDiChances;
    }

    //cerr<<"\n good \n";
    //cerr<<good;
    //cerr<<"\n chances \n";
    //cerr<<chancesLeft<<"\n";
    return good-bad+(chancesLeft*50);
    }

//Spelare A=1, Spelare B=0
int minMax(const GameState &pState, bool playerBool, int depth, int alfa, int beta){
    //cerr<<"\n------minMax----\n";
    
    
    //cerr<<"\n----child:"<<childCount<<"\n";
    double bestPossible;
    int v;
    if(depth==0||mu(pState).size()==0){
        v= (gamma(playerBool, pState));
        //cerr<<"gamma: "<<v<<" \n";
        //cerr<<"player: "<<player<<" \n";
    }else if(playerBool){
        //X-player
        v = INT8_MIN;//Blev problems med -INFINITY
        vector<GameState> States1 = mu(pState);
        for(int i=0;i<States1.size();++i){
            GameState child = States1[i];

            v = max(v,minMax(child, false, depth-1, alfa, beta));
            //cerr<<"max; "<<v<<" \n";
            alfa = max(alfa, v);
            if(beta<=alfa){
               ++pruning;
                break;
            }
        }
    //O-player
    }else{
        bestPossible = INT8_MAX;
        vector<GameState> States2 = mu(pState);
        for(int j=0;j<States2.size();++j){
            GameState child = States2[j];
            v = min(v, minMax(child, true, depth-1, alfa, beta));
            //cerr<<"min; "<<v<<" \n";
            beta = min(beta, v);
            if(beta<=alfa){
                ++pruning;
                break;
            }
        }
    }
    //Prunar 668 noder totalt
    //cerr<<"pruning: "<<pruning<<" \n";
    return v;
}

GameState Player::play(const GameState &pState,const Deadline &pDue)
{
    //std::cerr << "Processing " << pState.toMessage() << std::endl;
    //int player;
//    cerr<<"\n------play----\n";
//    cerr<<"\n player before: "<<player<<"\n";
//    
//    if(lastPlayer==1){
//        player=0;
//        lastPlayer =0;
//        cerr<<"\n last player was 1 \n";
//    }else if(lastPlayer==0){
//        player=1;
//        lastPlayer=1;
//        cerr<<"\n last player was 0 \n";
//    }
//    cerr<<"\n player after: "<<player<<"\n";
    vector<GameState> lNextStates = mu(pState);
    if (lNextStates.size() == 0) return GameState(pState, Move());

    /*
     * Here you should write your clever algorithms to get the best next move, ie the best
     * next state. This skeleton returns a random move instead.
     */
    //return lNextStates[rand() % lNextStates.size()];
    int max = 0;
    for(int i=0;i<lNextStates.size();++i){
        
        int newMax = minMax(lNextStates[i], true, INT8_MAX, INT8_MAX, INT8_MIN);
        if(newMax>max){
            max = i;
        }
    }
    return lNextStates[max];
}

/*namespace TICTACTOE*/ }
