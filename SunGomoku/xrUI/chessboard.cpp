#include <memory.h>
#include "chessboard.h"

chessboard::chessboard(){
    memset(chess,0,sizeof(chess));
    player=0;
}

bool chessboard::judge(int x,int y){
    if(!chess[x][y])
        return true;
    else
        return false;
}

void chessboard::go(int x,int y){
    player++;
    chess[x][y]=2-player%2;
    chessPoint.push_back(QPoint(x,y));
}

int chessboard::undo(int j){
    int x,y;
    for(int i=0;i<j;i++)
    {
        x=chessPoint.last().x();
        y=chessPoint.last().y();
        chess[x][y]=0;
        player--;
        chessPoint.pop_back();
    }
    return 15*chessPoint.last().x()+chessPoint.last().y();
}

void chessboard::cleanup(){
    memset(chess,0,sizeof(chess));
    player=0;
}
