#include "player.hpp"
#include <cstdlib>
#include <cmath>

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
    
    for(int i=0; i<4; ++i){
        for(int j=0;j<4;++j){
            me[i][j]=0;
            opponent[i][j]=0;
        }
    }
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
        rows.erase(rows.begin()+nr);
    }else if(id==1){
        //kill col
        cols.erase(rows.begin()+nr);
    }else if(id==2){
        //kill diag
        diag.erase(rows.begin()+nr);
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
//tuple<int, string, string, string, int> danger(){    
//    for(int i=0;i<4;++i){
//        int d = opScoreDi(i);
//        int r = opScoreRows(i);
//        int c = opScoreCols(i);
//        tuple<int, string, string, string, int> final = make_tuple(0,"","","",0);
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
vector<GameState> mu(const GameState &pState){
    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);

    return lNextStates;
    
}
int gamma(int player, const GameState &pState){
    
}
//Spelare A=1, Spelare B=0
int minMax(const GameState &pState, int player){
    if(mu(pState).size()==0){
        return(gamma(player, pState));
    }else{
        if(player==0){
            double bestPossible = -INFINITY;
            vector<GameState> States = mu(pState);
            for(int i=0;i<States.size();++i){
                GameState child = States[i];
                double v = minMax(child, 1);
                bestPossible = max(bestPossible, v);
            }
            return bestPossible;
        }else{
            double bestPossible = INFINITY;
            vector<GameState> States2 = mu(pState);
            for(int j=0;j<States2.size();++j){
                GameState child = States2[j];
                double v = minMax(child, 0);
                bestPossible = min(bestPossible, v);
            }
            return bestPossible;
        }
    }
}

GameState Player::play(const GameState &pState,const Deadline &pDue)
{
    //std::cerr << "Processing " << pState.toMessage() << std::endl;

    vector<GameState> lNextStates = mu(pState);
    if (lNextStates.size() == 0) return GameState(pState, Move());

    /*
     * Here you should write your clever algorithms to get the best next move, ie the best
     * next state. This skeleton returns a random move instead.
     */
    return lNextStates[rand() % lNextStates.size()];
}

/*namespace TICTACTOE*/ }
