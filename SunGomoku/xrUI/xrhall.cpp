#include "xrhall.h"
#include "ui_xrhall.h"
#include "Kernel/uiinc.h"
#include "Kernel/macro.h"

xrHall::xrHall(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::xrHall)
{
    ui->setupUi(this);

    initialize();

    this->setWindowTitle("Welcome to SunGomoku!");

    vtext = "AI v1.1.3, UI v1.4-beta2";
    ui->versionLabel->setText(vtext);
    ui->versionLabel->setAlignment(Qt::AlignCenter);

    chesscolor = new QButtonGroup(this);
    chesscolor->addButton(ui->black,1);
    chesscolor->addButton(ui->white,2);
    ui->black->setChecked(true);

    edition = new QButtonGroup(this);
    edition->addButton(ui->proEdition,1);
    edition->addButton(ui->pubEdition,0);
    ui->proEdition->setChecked(true);
    set_forbidden(edition->checkedId());
}

xrHall::~xrHall()
{
    uninitialize();
    delete ui;
}

void xrHall::game()
{
    depth = ui->modelBox->currentIndex();
    set_difficulty(depth);
    room = new xrRoom(vtext, depth, chesscolor->checkedId());
    room->show();
}

void xrHall::on_startButton_clicked()
{
    game();
}
