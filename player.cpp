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
vector<vector<int> > me(4, vector<int>(4));
vector<vector<int> > opponent(4, vector<int>(4));
vector<int> rows(4);
vector<int> cols(4);
vector<int> diag(2);
vector<int> myScoreRows(4);
vector<int> opScoreRows(4);
vector<int> myScoreCols(4);
vector<int> opScoreCols(4);
vector<int> myScoreDi(2);
vector<int> opScoreDi(2);
int lastPlayer =1;
int childCount=0;
int rowChances =4;
int colChances =4;
int diChances =2;


int fill(){
    ticTac[0][0]=3;
    ticTac[0][1]=2;
    ticTac[0][2]=2;
    ticTac[0][3]=3;
    
    ticTac[1][0]=2;
    ticTac[1][1]=4;
    ticTac[1][2]=4;
    ticTac[1][3]=2;
    
    ticTac[2][0]=2;
    ticTac[2][1]=4;
    ticTac[2][2]=4;
    ticTac[2][3]=2;
    
    ticTac[3][0]=3;
    ticTac[3][1]=2;
    ticTac[3][2]=2;
    ticTac[3][3]=3;
    
//    for(int i=0; i<4; ++i){
//        for(int j=0;j<4;++j){
//            me[i][j]=0;
//            opponent[i][j]=0;
//        }
//    }
}

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
int kill(int nr, int id){
    if(id==0){
        //kill row
        //rows.erase(rows.begin()+nr);
        --rowChances;
    }else if(id==1){
        //kill col
        //cols.erase(rows.begin()+nr);
        --colChances;
    }else if(id==2){
        //kill diag
        //diag.erase(rows.begin()+nr);
        --diChances;
    }
    return 0;
}

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
int place(int row, int col, int meOrOpp){
    if(meOrOpp==0){
        //me
        me[row][col]=1;
        if(row==col){
            //Diagonal1
            myScoreDi[0]+=1;        
        }else if(row==0&&col==3||row==1&&col==2||row==2&&col==1||row==3&&col==0){
            //Diagonal2
            myScoreDi[1]+=1;
        }else{
            myScoreRows[row]+=1;
            myScoreCols[col]+=1;
        }
    }else{
        opponent[row][col]=1;
        if(row==col){
            //Diagonal1
            opScoreDi[0]+=1;        
        }else if(row==0&&col==3||row==1&&col==2||row==2&&col==1||row==3&&col==0){
            //Diagonal2
            opScoreDi[1]+=1;
        }else{
            opScoreRows[row]+=1;
            opScoreCols[col]+=1;
        }
        
    }
}
//Kollar om motståndaren är ett drag ifrån att vinna
tuple<int, string, string, string, int> danger(){
    tuple<int, string, string, string, int> final = make_tuple(0,"","","",0);
    for(int i=0;i<4;++i){
        int d = opScoreDi[i];
        int r = opScoreRows[i];
        int c = opScoreCols[i];
        
        if(d==3){            
            if(r==3){               
                if(c==3){
                    tuple<int, string, string, string, int> warning3 = make_tuple(3, "D", "R", "C", i);
                    final=warning3;
                }else{
                   tuple<int, string, string, string, int> warning2 = make_tuple(2, "D", "R", "", i);
                   final=warning2;
                }
            }
            tuple<int, string, string, string, int> warning1 = make_tuple(1, "D", "", "", i);
            final=warning1;
        }else if(r==3){
            if(c==3){
                tuple<int, string, string, string, int> warning5 = make_tuple(2, "R", "C", "", i);
                final=warning5;
            }else{
                tuple<int, string, string, string, int> warning4 = make_tuple(1, "R", "", "", i);
                final=warning4;
            }
        }else if(c==3){
            tuple<int, string, string, string, int> warning6 = make_tuple(1, "C", "", "", i);
            final=warning6;
        }
    }
    return final;
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

    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            
            if(pState.at(i,j)&CELL_X){
                //Score how I am doing
                myScoreRows[i]+=1;
                myScoreCols[j]+=1;
                int di = isDiag(i, j);
                if(di!=2){
                    myScoreDi[di]+=1;
                }
                
            }else if(pState.at(i,j)&CELL_O){
                //Score enemy& Kill chances at winning
                opScoreRows[i]+=1;
                opScoreCols[j]+=1;
                int di = isDiag(i, j);
                if(di!=2){
                    opScoreDi[di]+=1;
                    kill(di,2);
                }
                //kill row
                kill(i,0);
                //kill col
                kill(j,1);                
            }
        }
    }
    //Calculate nr rows&&cols&&diag with more than 2 - dead rows&&cols&&diag
    int good =0;
    int bad =0;
    for(int k=0;k<4;++k){       
        int r = myScoreRows[k];
        int c = myScoreCols[k];
        
        int ro = opScoreRows[k];
        int co = opScoreCols[k];
        
        if(r>2){good+=10;}
        if(c>2){good+=10;}
        if(r>3){good+=INT8_MAX;}
        if(c>3){good+=INT8_MAX;}
        
        if(ro>2){bad+=10;}
        if(co>2){bad+=10;}
        if(ro>3){bad+=INT8_MAX;}
        if(co>3){bad+=INT8_MAX;}
    }
    for(int l=0;l<2;++l){
        int d = myScoreDi[l];
        
        int d_o = opScoreDi[l];
        
        if(d>2){good+=10;}
        if(d>3){good+=INT8_MAX;}
        
        if(d_o>2){bad+=10;}
        if(d_o>3){bad+=INT8_MAX;}
    }
    
//    if(pState.isXWin()){
//        if(player==0){
//            return 100;
//        }else{
//            return 0;
//        }
//    }else if(pState.isOWin()){
//        if(player==0){
//            return 100;
//        }else{
//            return 0;
//        }
//    }
    //else if(pState.isDraw()){
//        return 50;
//    }
    int chancesLeft = 10-(rowChances+colChances+diChances);
    return good-bad+(chancesLeft*100);
}
//Spelare A=1, Spelare B=0
int minMax(const GameState &pState, int player, int depth, int alfa, int beta){
    //cerr<<"\n------minMax----\n";
    
    
    //cerr<<"\n----child:"<<childCount<<"\n";
    double bestPossible;
    int v;
    if(depth==0||mu(pState).size()==0){
        v= (gamma(player, pState));
        cerr<<"gamma: "<<v<<" \n";
        cerr<<"player: "<<player<<" \n";
    }else if(player == 0){
        //X-player
        v = INT8_MIN;//Blev problems med -INFINITY
        vector<GameState> States1 = mu(pState);
        for(int i=0;i<States1.size();++i){
            GameState child = States1[i];

            v = max(v,minMax(child, 1, depth-1, alfa, beta));
            cerr<<"max; "<<v<<" \n";
            alfa = max(alfa, v);
            if(beta<=alfa){
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
            cerr<<"min; "<<v<<" \n";
            beta = min(beta, v);
            if(beta<=alfa){
                break;
            }
        }
    }   
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
        cerr<<"i: "<<i<<" ";
        int newMax = minMax(lNextStates[i], player, INT8_MAX, INT8_MAX, INT8_MIN);
        if(newMax>max){
            max = i;
        }
    }
    return lNextStates[max];
}

/*namespace TICTACTOE*/ }
