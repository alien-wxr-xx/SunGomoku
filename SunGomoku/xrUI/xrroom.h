#ifndef XRROOM_H
#define XRROOM_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QString>
#include <QTime>
#include <QtGlobal>
#include "chessboard.h"

namespace Ui {
class xrRoom;
}

class xrRoom : public QMainWindow
{
    Q_OBJECT

public:
    explicit xrRoom(QString vtext, int depth, int isFirst, QWidget *parent = nullptr);
    ~xrRoom();
    virtual void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

    void paintRatio();

    void begin();
    void playerGo();
    void aiGo();

    chessboard chessboard;
    bool mouseflag;
    QString version;

private slots:
    void on_actionUndo_triggered();

    void on_actionRestart_triggered();

    void on_actionQuit_triggered();

private:
    Ui::xrRoom *ui;

    int moveX, moveY;
    int currentX, currentY;
    int playerColor;
    int aiColor;
    int aiPos;
    int isOver;
    bool isPaint;

    int beginX, beginY, endX, endY, centerX, centerY;
    int gap, halfGap, quarGap, eighthGap;
    int r;
};

#endif // XRROOM_H
