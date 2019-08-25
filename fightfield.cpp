#include "fightfield.h"
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QFont>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QApplication>

FightField::FightField(int field_height, int field_width, int start_left,
                       int start_top, int column_width, int column_height, QWidget *parent):
    QWidget(parent)
{
//    this->_field_height = field_height;
//    this->_field_width = field_width;
//    this->_start_left = start_left;
//    this->_start_top = start_top;
//    this->_column_width = column_width;
//    this->_column_height = column_height;

    qDebug()<<field_width;
    //this->setFixedSize(field_width, field_height);
}

FightField::~FightField(){

}

void FightField::paintEvent(QPaintEvent *event){
    //QPainter painter(this);
    //QPen pen(Qt::black);
    //pen.setWidth(1);
    //painter.setPen(pen);

    //drawFieldLabels(&painter);
    //drawFieldButtons(&painter);

    QWidget::paintEvent(event);
}

void FightField::mousePressEvent(QMouseEvent *event){

    QWidget::mousePressEvent(event);
}

void FightField::drawFieldLabels(QPainter *painter){
    QString characters = "АБВГДЕЁЖЗИ";
    auto iter = characters.begin();

    int i = 0;
    for(iter, i; iter != characters.end(); iter++, i++){
        int left_position = i*this->_column_width + this->_start_left;
        painter->drawText(left_position, this->_start_top, *iter);
    }

    for(i = 1; i <= field_dimension; i++){
        int top_position = i*this->_column_height + this->_start_top;
        painter->drawText(this->_start_left - this->_column_width, top_position, QString::number(i));
    }
}

void FightField::drawFieldButtons(QPainter *painter){
    int down_position = this->_column_height*field_dimension + this->_start_top;
    int right_position = this->_column_width*field_dimension + this->_start_left;
    int field_width = right_position - this->_start_left;
    int field_height = down_position - this->_start_top;

    QPainterPath rect_path;
    rect_path.addRect(this->_start_left, this->_start_top, field_width, field_height);
    painter->fillPath(rect_path, Qt::white);
    painter->drawPath(rect_path);

    int i=0;
    for(i; i <= field_dimension; i++){
        int left_position = i*this->_column_width + this->_start_left;
        painter->drawLine(left_position, this->_start_top, left_position, down_position);
    }

    for(i = 0; i <= field_dimension; i++){
        int top_position = i*this->_column_height + this->_start_top;
        painter->drawLine(this->_start_left, top_position, right_position, top_position);
    }

}

