#include "fightfield.h"
#include <QPainter>
#include <QFont>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>

FightField::FightField(QWidget *parent):
    QWidget (parent)
{
    this->setFixedSize(340, 340);
    QGridLayout *main_layout = new QGridLayout;
    main_layout->setSpacing(0);
    this->setLayout(main_layout);

    this->drawFieldLabels(main_layout);
    this->drawFieldButtons(main_layout);
}

FightField::~FightField(){

}

void FightField::drawFieldLabels(QGridLayout *main_layout){
    QString characters = "АБВГДЕЁЖЗИ";
    auto iter = characters.begin();
    int column_number = 1;
    int row_number = 0;

    for(iter, column_number; iter != characters.end(); iter++, column_number++){
        QLabel *character_label = new QLabel(*iter);
        main_layout->addWidget(character_label, row_number, column_number, Qt::AlignCenter);
    }

    column_number = 0;
    for(row_number = 1; row_number <= 10; row_number++){
        QLabel *digit_label = new QLabel(QString::number(row_number));
        digit_label->setFixedSize(30, 30);
        main_layout->addWidget(digit_label, row_number, 0);
    }
}

void FightField::drawFieldButtons(QGridLayout *main_layout){
    for(int row_number = 1; row_number <= 10; row_number++){
        for(int column_number = 1; column_number <= 10; column_number++){
            QPushButton *field_button = new QPushButton;
            field_button->setFixedSize(30, 30);
            main_layout->addWidget(field_button, row_number, column_number);
        }
    }
}

