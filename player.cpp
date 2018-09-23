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
//vector<vector<int> > me(4, vector<int>(4));
//vector<vector<int> > opponent(4, vector<int>(4));


int lastPlayer =1;
int pruning=0;


//int fill(){
//    ticTac[0][0]=3;
//    ticTac[0][1]=2;
//    ticTac[0][2]=2;
//    ticTac[0][3]=3;
//    
//    ticTac[1][0]=2;
//    ticTac[1][1]=4;
//    ticTac[1][2]=4;
//    ticTac[1][3]=2;
//    
//    ticTac[2][0]=2;
//    ticTac[2][1]=4;
//    ticTac[2][2]=4;
//    ticTac[2][3]=2;
//    
//    ticTac[3][0]=3;
//    ticTac[3][1]=2;
//    ticTac[3][2]=2;
//    ticTac[3][3]=3;
//    
//}

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
//Döda chans på rad/kolumn/diagonal om motståndarre placerat där
//int kill(int nr, int id){
//    if(id==0){
//        //kill row
//        rows[id]=0;
//        --rowChances;
//    }else if(id==1){
//        //kill col
//        cols[id]=0;
//        --colChances;
//    }else if(id==2){
//        //kill diag
//        diag[id]=0;
//        --diChances;
//    }
//    return 0;
//}

//Kollar om grannar och ger riktning
string isNeighbours(int row1, int col1,  int row2, int col2){
    if(abs(row1-row2)==1){
        //Vertical
        return "V";
    }else if(abs(col1-col2)==1){
        //Horisontal
        return "H";
    }else if(abs(row1-row2)==1&&abs(col1-col2)==1){
        //Diagonal
        return "D";
    }else{
        return "N";
    }
}
//int place(int row, int col, int meOrOpp){
//    if(meOrOpp==0){
//        //me
//        me[row][col]=1;
//        if(row==col){
//            //Diagonal1
//            myScoreDi[0]+=1;        
//        }else if(row==0&&col==3||row==1&&col==2||row==2&&col==1||row==3&&col==0){
//            //Diagonal2
//            myScoreDi[1]+=1;
//        }else{
//            myScoreRows[row]+=1;
//            myScoreCols[col]+=1;
//        }
//    }else{
//        opponent[row][col]=1;
//        if(row==col){
//            //Diagonal1
//            opScoreDi[0]+=1;        
//        }else if(row==0&&col==3||row==1&&col==2||row==2&&col==1||row==3&&col==0){
//            //Diagonal2
//            opScoreDi[1]+=1;
//        }else{
//            opScoreRows[row]+=1;
//            opScoreCols[col]+=1;
//        }
//        
//    }
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
int gamma(int player, const GameState &pState){
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
    cerr<<"yRowChances: "<<yRowChances<<"\n";
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            
            if(pState.at(i,j)==CELL_X){
                
                if(yRows[i]!=0){
                   yRows[i]=0;
                   --yRowChances;
                   cerr<<"yRowChances: "<<yRowChances<<"\n";
                }
                if(yCols[j]!=0){
                  yCols[j]=0;
                  --yColChances;
                  cerr<<"yColChances: "<<yColChances<<"\n";
                }
                             
                int diT = isDiag(i, j);
                if(diT!=2){
                    if(yDi[diT]!=0){
                        yDi[diT]=0;
                        --yDiChances;
                        cerr<<"yDiChances: "<<yDiChances<<"\n";
                    }
               }
            }
            if(pState.at(i,j)==CELL_O){
                if(xRows[i]!=0){
                    xRows[i]=0;
                    --xRowChances;
                }
                if(xCols[j]!=0){
                   xCols[j]=0;
                    --xColChances; 
                }
                
                int diT = isDiag(i, j);
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
    cerr<<"\nrx, cx, ry, cy \n";
    for(int k=0;k<4;++k){
        //Score +1 bara om ensam på rad
        int rx = xScoreRows[k];
        int cx = xScoreCols[k];              
        int ry = yScoreRows[k];
        int cy = yScoreCols[k];
        
        cerr<<" "<<rx<<"  "<<cx<<"  "<<"  "<<ry<<"  "<<cy<<"\n";
        if(player==0){
            if(rx==1){good+=1;}
            if(cx==1){good+=1;}
            if(rx==2){good+=5;}
            if(cx==2){good+=5;}
            if(rx==3){good+=10;}
            if(cx==3){good+=10;}
            if(rx==4){good+=INT8_MAX;}
            if(cx==4){good+=INT8_MAX;}
            
//            if(ry==1){bad+=10;}
//            if(cy==1){bad+=10;}
//            if(ry==2){bad+=50;}
//            if(cy==2){bad+=50;}
//            if(ry>2){bad+=100;}
//            if(cy>2){bad+=100;}
//            if(ry>3){bad+=INT8_MAX;}
//            if(cy>3){bad+=INT8_MAX;}
        }else if(player==1){
//            if(rx==1){bad+=10;}
//            if(cx==1){bad+=10;}
//            if(rx==2){bad+=50;}
//            if(cx==2){bad+=50;}
//            if(rx==3){bad+=100;}
//            if(cx==3){bad+=100;}
//            if(rx==4){bad+=INT8_MAX;}
//            if(cx==4){bad+=INT8_MAX;}
            
            if(ry==1){good+=1;}
            if(cy==1){good+=1;}
            if(ry==2){good+=5;}
            if(cy==2){good+=5;}
            if(ry==3){good+=10;}
            if(cy==3){good+=10;}
            if(ry==4){good+=INT8_MAX;}
            if(cy==4){good+=INT8_MAX;}
        }          
    }
    for(int l=0;l<2;++l){
        int dx = xScoreDi[l];
        
        int dy = yScoreDi[l];
        //Player X
        if(player==0){
            if(dx==1){good+=1;}
            if(dx==2){good+=5;}
            if(dx==3){good+=10;}
            if(dx==4){good+=INT8_MAX;}
//            
//            if(dy==1){bad+=1;}
//            if(dy==2){bad+=5;}
//            if(dy==3){bad+=10;}
//            if(dy==4){bad+=INT8_MAX;}
        //Player O
        }else if(player==1){
//            if(dx==1){bad+=1;}
//            if(dx==2){bad+=5;}
//            if(dx==3){bad+=10;}
//            if(dx==4){bad+=INT8_MAX;}

            if(dy==1){good+=1;}
            if(dy==2){good+=5;}
            if(dy==3){good+=10;}
            if(dy==4){good+=INT8_MAX;}
        }       
    }
    int chancesLeft=0;
    if(player==0){
        chancesLeft = xRowChances+xColChances+xDiChances;
    }else if(player==1){
        chancesLeft = yRowChances+yColChances+yDiChances;
    }

    cerr<<"\n good \n";
    cerr<<good;
    cerr<<"\n chances \n";
    cerr<<chancesLeft<<"\n";
    return good+(chancesLeft*100);
    }

//Spelare A=1, Spelare B=0
int minMax(const GameState &pState, int player, int depth, int alfa, int beta){
    //cerr<<"\n------minMax----\n";
    
    
    //cerr<<"\n----child:"<<childCount<<"\n";
    double bestPossible;
    int v;
    if(depth==0||mu(pState).size()==0){
        v= (gamma(player, pState));
        //cerr<<"gamma: "<<v<<" \n";
        //cerr<<"player: "<<player<<" \n";
    }else if(player == 0){
        //X-player
        v = INT8_MIN;//Blev problems med -INFINITY
        vector<GameState> States1 = mu(pState);
        for(int i=0;i<States1.size();++i){
            GameState child = States1[i];

            v = max(v,minMax(child, 1, depth-1, alfa, beta));
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
            v = min(v, minMax(child, 0, depth-1, alfa, beta));
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
    //cerr<<"\n------play----\n";
    int player;
    if(lastPlayer==1){
        player=0;
        lastPlayer =0;
    }else{
        player=1;
        lastPlayer=1;
    }
    
    vector<GameState> lNextStates = mu(pState);
    if (lNextStates.size() == 0) return GameState(pState, Move());

    /*
     * Here you should write your clever algorithms to get the best next move, ie the best
     * next state. This skeleton returns a random move instead.
     */
    //return lNextStates[rand() % lNextStates.size()];
    int max = 0;
    for(int i=0;i<lNextStates.size();++i){
       // cerr<<"i: "<<i<<" ";
        int newMax = minMax(lNextStates[i], player, INT8_MAX, INT8_MAX, INT8_MIN);
        if(newMax>max){
            max = i;
        }
    }
    return lNextStates[max];
}

/*namespace TICTACTOE*/ }
