#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QPoint>
#include <QVector>

typedef struct
{
  int x,y;
}value;

class chessboard
{
public:
    chessboard();
    int chess[15][15];
    int player;
    bool judge(int x,int y);
    void go(int x,int y);
    int undo(int j);
    void cleanup();

private:
    QVector<QPoint> chessPoint;
};

#endif // CHESSBOARD_H
