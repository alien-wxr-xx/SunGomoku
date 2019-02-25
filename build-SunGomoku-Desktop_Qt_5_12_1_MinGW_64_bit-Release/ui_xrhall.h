/********************************************************************************
** Form generated from reading UI file 'xrhall.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XRHALL_H
#define UI_XRHALL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_xrHall
{
public:
    QGridLayout *gridLayout_2;
    QLabel *logoLabel;
    QLabel *versionLabel;
    QVBoxLayout *whoFirstLayout;
    QRadioButton *black;
    QRadioButton *white;
    QComboBox *modelBox;
    QPushButton *startButton;

    void setupUi(QWidget *xrHall)
    {
        if (xrHall->objectName().isEmpty())
            xrHall->setObjectName(QString::fromUtf8("xrHall"));
        xrHall->resize(502, 340);
        gridLayout_2 = new QGridLayout(xrHall);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        logoLabel = new QLabel(xrHall);
        logoLabel->setObjectName(QString::fromUtf8("logoLabel"));
        logoLabel->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(logoLabel, 0, 0, 1, 4);

        versionLabel = new QLabel(xrHall);
        versionLabel->setObjectName(QString::fromUtf8("versionLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(versionLabel->sizePolicy().hasHeightForWidth());
        versionLabel->setSizePolicy(sizePolicy);
        versionLabel->setTextFormat(Qt::PlainText);

        gridLayout_2->addWidget(versionLabel, 4, 2, 1, 2);

        whoFirstLayout = new QVBoxLayout();
        whoFirstLayout->setObjectName(QString::fromUtf8("whoFirstLayout"));
        black = new QRadioButton(xrHall);
        black->setObjectName(QString::fromUtf8("black"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(black->sizePolicy().hasHeightForWidth());
        black->setSizePolicy(sizePolicy1);

        whoFirstLayout->addWidget(black);

        white = new QRadioButton(xrHall);
        white->setObjectName(QString::fromUtf8("white"));
        sizePolicy1.setHeightForWidth(white->sizePolicy().hasHeightForWidth());
        white->setSizePolicy(sizePolicy1);

        whoFirstLayout->addWidget(white);


        gridLayout_2->addLayout(whoFirstLayout, 4, 1, 2, 1);

        modelBox = new QComboBox(xrHall);
        modelBox->addItem(QString());
        modelBox->addItem(QString());
        modelBox->addItem(QString());
        modelBox->setObjectName(QString::fromUtf8("modelBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(modelBox->sizePolicy().hasHeightForWidth());
        modelBox->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(modelBox, 5, 2, 1, 1);

        startButton = new QPushButton(xrHall);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(startButton->sizePolicy().hasHeightForWidth());
        startButton->setSizePolicy(sizePolicy3);

        gridLayout_2->addWidget(startButton, 5, 3, 1, 1);


        retranslateUi(xrHall);

        QMetaObject::connectSlotsByName(xrHall);
    } // setupUi

    void retranslateUi(QWidget *xrHall)
    {
        xrHall->setWindowTitle(QApplication::translate("xrHall", "Form", nullptr));
        logoLabel->setText(QApplication::translate("xrHall", "<html><head/><body><p align=\"center\"><img src=\":/myimages/bigLogo.png\"/></p></body></html>", nullptr));
        versionLabel->setText(QString());
        black->setText(QApplication::translate("xrHall", "\347\216\251\345\256\266\346\211\247\351\273\221", nullptr));
        white->setText(QApplication::translate("xrHall", "\347\216\251\345\256\266\346\211\247\347\231\275", nullptr));
        modelBox->setItemText(0, QApplication::translate("xrHall", "\346\227\240\347\246\201\346\211\213", nullptr));
        modelBox->setItemText(1, QApplication::translate("xrHall", "\346\234\211\347\246\201\346\211\213", nullptr));
        modelBox->setItemText(2, QApplication::translate("xrHall", "\346\234\211\347\246\201\346\211\213+\345\274\200\345\261\200\345\272\223", nullptr));

        startButton->setText(QApplication::translate("xrHall", "\345\274\200\345\247\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class xrHall: public Ui_xrHall {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XRHALL_H
