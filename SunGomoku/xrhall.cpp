#include "xrhall.h"
#include "ui_xrhall.h"

xrHall::xrHall(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::xrHall)
{
    ui->setupUi(this);
}

xrHall::~xrHall()
{
    delete ui;
}
