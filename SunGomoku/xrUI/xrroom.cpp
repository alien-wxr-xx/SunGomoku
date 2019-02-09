#include "xrroom.h"
#include "ui_xrroom.h"
#include "Kernel/uiinc.h"
#include "Kernel/macro.h"

#include <QLabel>
#include <QPainter>
#include <QMessageBox>
#include <QProcess>
#include <QToolBar>

xrRoom::xrRoom(QString vtext, int depth, int color, QWidget *parent) :
    QMainWindow(parent), chessboard(),
    ui(new Ui::xrRoom)
{
    ui->setupUi(this);

    this->setWindowModality(Qt::ApplicationModal);

    playerColor = color;
    aiColor = 3-playerColor;

    QLabel * versionNum = new QLabel(this);
    versionNum->setText(vtext);

    ui->statusbar->addPermanentWidget(versionNum);
    ui->statusbar->setSizeGripEnabled(false);
    ui->toolBar->setMovable(false);

    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor("#B1723C"));

    setMouseTracking(true);
    centralWidget()->setMouseTracking(true);

    if (depth == 2)
        ui->actionUndo->setEnabled(false);
    else
        ui->actionUndo->setEnabled(true);

    begin();
}

xrRoom::~xrRoom()
{
    delete ui;
}

void xrRoom::paintEvent(QPaintEvent *)
{
    paintRatio();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen pen=painter.pen();
    pen.setColor(QColor("#8D5822"));
    pen.setWidth(7);
    painter.setPen(pen);
    QBrush brush;
    brush.setColor(QColor("#EEC085"));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(beginX-halfGap, beginY-halfGap, 15*gap, 15*gap);
    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
    for(int i=0;i<15;i++)
    {
        painter.drawLine(beginX+i*gap, beginY, beginX+i*gap, endY);
        painter.drawLine(beginX, beginY+i*gap, endX, beginY+i*gap);
    }
    brush.setColor(Qt::black);
    painter.setBrush(brush);
    painter.drawEllipse(QPoint(beginX+3*gap, beginY+3*gap), eighthGap, eighthGap);
    painter.drawEllipse(QPoint(beginX+11*gap, beginY+3*gap), eighthGap, eighthGap);
    painter.drawEllipse(QPoint(beginX+3*gap, beginY+11*gap), eighthGap, eighthGap);
    painter.drawEllipse(QPoint(beginX+11*gap, beginY+11*gap), eighthGap, eighthGap);
    painter.drawEllipse(QPoint(beginX+7*gap, beginY+7*gap), eighthGap, eighthGap);
    //画棋子
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            if (chessboard.chess[i][j] == 1){
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(QPoint(j*gap+beginX, i*gap+beginY), r, r);
            }
            else if (chessboard.chess[i][j] == 2) {
                brush.setColor(Qt::white);
                painter.setPen(Qt::NoPen);
                painter.setBrush(brush);
                painter.drawEllipse(QPoint(j*gap+beginX, i*gap+beginY), r, r);
            }
        }
    }
    //鼠标当前位置的光标
    pen.setColor(Qt::red);
    pen.setWidth(1);
    painter.setPen(pen);

    if(moveX>=0&&moveX<15&&moveY>=0&&moveY<15)
    {
        painter.drawLine(moveY*gap+beginX-halfGap, moveX*gap+beginY-halfGap, moveY*gap+beginX-quarGap, moveX*gap+beginY-halfGap);
        painter.drawLine(moveY*gap+beginX+halfGap, moveX*gap+beginY-halfGap, moveY*gap+beginX+quarGap, moveX*gap+beginY-halfGap);
        painter.drawLine(moveY*gap+beginX-halfGap, moveX*gap+beginY+halfGap, moveY*gap+beginX-quarGap, moveX*gap+beginY+halfGap);
        painter.drawLine(moveY*gap+beginX+halfGap, moveX*gap+beginY+halfGap, moveY*gap+beginX+quarGap, moveX*gap+beginY+halfGap);
        painter.drawLine(moveY*gap+beginX-halfGap, moveX*gap+beginY-halfGap, moveY*gap+beginX-halfGap, moveX*gap+beginY-quarGap);
        painter.drawLine(moveY*gap+beginX+halfGap, moveX*gap+beginY-halfGap, moveY*gap+beginX+halfGap, moveX*gap+beginY-quarGap);
        painter.drawLine(moveY*gap+beginX-halfGap, moveX*gap+beginY+halfGap, moveY*gap+beginX-halfGap, moveX*gap+beginY+quarGap);
        painter.drawLine(moveY*gap+beginX+halfGap, moveX*gap+beginY+halfGap, moveY*gap+beginX+halfGap, moveX*gap+beginY+quarGap);
    }
    int pen2 = 2;
    pen.setWidth(pen2);
    painter.setPen(pen);
    //最近一个落子的标志
    painter.drawLine(currentY*gap+beginX-pen2, currentX*gap+beginY, currentY*gap+beginX-pen2-eighthGap, currentX*gap+beginY);
    painter.drawLine(currentY*gap+beginX+pen2, currentX*gap+beginY, currentY*gap+beginX+pen2+eighthGap, currentX*gap+beginY);
    painter.drawLine(currentY*gap+beginX, currentX*gap+beginY-pen2, currentY*gap+beginX, currentX*gap+beginY-pen2-eighthGap);
    painter.drawLine(currentY*gap+beginX, currentX*gap+beginY+pen2, currentY*gap+beginX, currentX*gap+beginY+pen2+eighthGap);

    isPaint=true;
}

void xrRoom::mouseMoveEvent(QMouseEvent *event){
    moveX=(event->y()-beginY+halfGap)/gap;
    moveY=(event->x()-beginX+halfGap)/gap;
    update();
}

void xrRoom::mouseReleaseEvent(QMouseEvent *event)
{
    int x, y;
    if(mouseflag)
    {
        x=(event->y()-beginY+halfGap)/gap;
        y=(event->x()-beginX+halfGap)/gap;

        if(x>=0&&x<15&&y>=0&&y<15)
        {
            if(chessboard.judge(x, y))
            {
                currentX=x;
                currentY=y;
                chessboard.go(currentX, currentY);
                repaint();
                playerGo();
            }
        }
    }
}

void xrRoom::on_actionUndo_triggered()
{
    int currentPos, t;
    currentPos=15*currentX+currentY;
    if(chessboard.player >= 2){
        t = (playerColor+chessboard.player)%2+1;
        currentPos=chessboard.undo(t);
        undo_move(t);
    }
    currentX=currentPos/15;
    currentY=currentPos%15;
    update();
    mouseflag=true;
}

void xrRoom::on_actionRestart_triggered()
{
    chessboard.cleanup();
    restart();
    update();
    begin();
}

void xrRoom::on_actionQuit_triggered()
{
    this->close();
}

void xrRoom::begin(){

    chessboard.cleanup();
    restart();
    currentX = 0;
    currentY = 0;
    isOver = 0;
    isPaint = true;

    if (playerColor==2){
        aiPos = ai_do_move(&isOver, aiColor);
        currentX=aiPos/15;
        currentY=aiPos%15;
        chessboard.go(currentX, currentY);
        update();
    }

    mouseflag=true;
}

void xrRoom::playerGo()
{
    isPaint=false;
    while (!isPaint) {
        repaint();
    }

    player_do_move(currentX, currentY, &isOver, playerColor);

    if(isOver==playerColor){
        QMessageBox::about(this, QStringLiteral("Win"), QStringLiteral("You win!"));
        mouseflag=false;
    }
    else if (isOver==225) {
        QMessageBox::about(this, QStringLiteral("Draw"), QStringLiteral("双方战平"));
        mouseflag=false;
    }
    else if (isOver==0) {
        mouseflag=false;
        aiGo();
    }
    else if (isOver==aiColor){
        QMessageBox::about(this, QStringLiteral("Lose"), QStringLiteral("AI wins!"));
        mouseflag=false;
    }
}

void xrRoom::aiGo()
{
    aiPos = ai_do_move(&isOver, aiColor);
    currentX = aiPos/15;
    currentY = aiPos%15;
    chessboard.go(currentX, currentY);
    update();

    if(isOver==playerColor){
        QMessageBox::about(this, QStringLiteral("Win"), QStringLiteral("You win!"));
        mouseflag=false;
    }
    else if (isOver==225) {
        QMessageBox::about(this, QStringLiteral("Draw"), QStringLiteral("双方战平"));
        mouseflag=false;
    }
    else if (isOver==0) {
        mouseflag=true;
    }
    else if (isOver==aiColor){
        QMessageBox::about(this, QStringLiteral("Lose"), QStringLiteral("AI wins!"));
        mouseflag=false;
    }
}

void xrRoom::paintRatio()
{
    gap = qMin(this->centralWidget()->width()/16, this->centralWidget()->height()/16);

    centerX = this->centralWidget()->width()/2;
    centerY = this->centralWidget()->height()/2+this->centralWidget()->y();

    beginX = centerX-7*gap;
    beginY = centerY-7*gap;

    endX = beginX+14*gap;
    endY = beginY+14*gap;

    halfGap = gap/2;
    quarGap = gap/4;
    eighthGap = gap/8;

    r = gap*40/100;
}
