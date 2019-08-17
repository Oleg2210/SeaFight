#include "fightfield.h"
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QFont>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include <QApplication>

FightField::FightField(QWidget *parent):
    QWidget (parent)
{
    this->setFixedSize(340, 340);
}

FightField::~FightField(){

}

void FightField::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPen pen(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);

    drawFieldLabels(&painter);
    drawFieldButtons(&painter);

    QWidget::paintEvent(event);
}

void FightField::mousePressEvent(QMouseEvent *event){

    mousePressEvent(event);
}

void FightField::drawFieldLabels(QPainter *painter){
    QString characters = "АБВГДЕЁЖЗИ";
    auto iter = characters.begin();

    int i = 0;
    for(iter, i; iter != characters.end(); iter++, i++){
        int left_position = i*this->column_width + this->start_left_position;
        painter->drawText(left_position, this->start_top_position, *iter);
    }

    for(i = 1; i <= 10; i++){
        int top_position = i*this->column_height + this->start_top_position;
        painter->drawText(this->start_left_position - this->column_width, top_position, QString::number(i));
    }
}

void FightField::drawFieldButtons(QPainter *painter){
    int down_position = this->column_height*10 + this->start_top_position;
    int right_position = this->column_width*10 + this->start_left_position;
    int field_width = right_position - this->start_left_position;
    int field_height = down_position - this->start_top_position;

    QPainterPath rect_path;
    rect_path.addRect(this->start_left_position, this->start_top_position, field_width, field_height);
    painter->fillPath(rect_path, Qt::white);
    painter->drawPath(rect_path);

    int i=0;
    for(i; i <= 10; i++){
        int left_position = i*this->column_width + this->start_left_position;
        painter->drawLine(left_position, this->start_top_position, left_position, down_position);
    }

    for(i = 0; i <= 10; i++){
        int top_position = i*this->column_height + this->start_top_position;
        painter->drawLine(this->start_left_position, top_position, right_position, top_position);
    }

}

