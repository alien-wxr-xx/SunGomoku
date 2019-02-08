#ifndef XRROOM_H
#define XRROOM_H

#include <QMainWindow>

namespace Ui {
class xrRoom;
}

class xrRoom : public QMainWindow
{
    Q_OBJECT

public:
    explicit xrRoom(QWidget *parent = nullptr);
    ~xrRoom();

private:
    Ui::xrRoom *ui;
};

#endif // XRROOM_H
