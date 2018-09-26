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

vector<GameState> mu(const GameState &pState){
    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);
    return lNextStates;
}

int gamma(bool player, const GameState &pState){
    if(pState.isXWin()){
        return 2;
    }
    else if(pState.isOWin()){
        return 0;
    }
    else{
        return 1;
    }

    
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
