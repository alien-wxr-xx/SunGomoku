#include "xrroom.h"
#include "ui_xrroom.h"

xrRoom::xrRoom(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::xrRoom)
{
    ui->setupUi(this);
}

xrRoom::~xrRoom()
{
    delete ui;
}
