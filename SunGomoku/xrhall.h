#ifndef XRHALL_H
#define XRHALL_H

#include <QWidget>

namespace Ui {
class xrHall;
}

class xrHall : public QWidget
{
    Q_OBJECT

public:
    explicit xrHall(QWidget *parent = nullptr);
    ~xrHall();

private:
    Ui::xrHall *ui;
};

#endif // XRHALL_H
