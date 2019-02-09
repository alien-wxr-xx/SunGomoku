#ifndef XRHALL_H
#define XRHALL_H

#include <QWidget>
#include <QButtonGroup>
#include <QString>
#include "xrroom.h"

namespace Ui {
class xrHall;
}

class xrHall : public QWidget
{
    Q_OBJECT

public:
    explicit xrHall(QWidget *parent = nullptr);
    ~xrHall();

    void game();

private slots:

    void on_startButton_clicked();

private:
    Ui::xrHall *ui;
    QButtonGroup * chesscolor;
    QButtonGroup * edition;
    xrRoom * room;
    QString vtext;
    int depth;
};

#endif // XRHALL_H
