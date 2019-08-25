#include "fightfield.h"
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QFont>
#include <QFontMetrics>
#include <stdexcept>
#include <QDebug>

FightField::FightField(int padding, int column_size, QWidget *parent):
    _padding(padding), _column_size(column_size), QWidget(parent)
{
    int size = _padding*2 + getFieldSize();
    this->setFixedSize(size, size);
}

FightField::~FightField(){

}

void FightField::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPen pen(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);

    drawFieldLabels(painter);
    drawFieldButtons(painter);

    QWidget::paintEvent(event);
}

void FightField::mousePressEvent(QMouseEvent *event){

    QWidget::mousePressEvent(event);
}

void FightField::drawFieldLabels(QPainter &painter){
    QFontMetrics font_metric(painter.font());
    drawFieldChars(painter, font_metric);
    drawFieldNumbers(painter, font_metric);
}

void FightField::drawFieldButtons(QPainter &painter){
    QPainterPath rect_path;
    rect_path.addRect(this->_padding, this->_padding, getFieldSize(), getFieldSize());
    painter.fillPath(rect_path, Qt::white);
    painter.drawPath(rect_path);

    int edge = getFieldSize() + this->_padding;

    for(int i=0; i <= FIELD_DIMENSION; i++){
        int line_size = i*this->_column_size + this->_padding;
        painter.drawLine(line_size, this->_padding, line_size, edge);
        painter.drawLine(this->_padding, line_size, edge, line_size);
    }
}

void FightField::drawFieldChars(QPainter &painter, const QFontMetrics &font_metric){
    QString characters = "АБВГДЕЁЖЗИ";
    auto iter = characters.begin();
    int i = 0;

    for(iter, i; iter != characters.end(); iter++, i++){
        int top = this->_padding - font_metric.ascent();
        int left = this->_column_size*i + this->_padding;
        int left_offset = (this->_column_size - font_metric.horizontalAdvance(*iter))/2;

        if((left_offset <= 0) || (top <= 0))
            throw std::range_error(CELL_SIZE_ERROR);

        painter.drawText(left + left_offset, top, *iter);
    }
}

void FightField::drawFieldNumbers(QPainter &painter, const QFontMetrics &font_metric){
    for(int i = 1; i <= FIELD_DIMENSION; i++){
        int left = (this->_padding - font_metric.horizontalAdvance(QString::number(i)))/2;
        int top = i*this->_column_size + this->_padding;
        int top_offset = (this->_column_size - font_metric.ascent())/2;

        if((top_offset <= 0) || (left <= 0))
            throw std::range_error(CELL_SIZE_ERROR);

        painter.drawText(left, top - top_offset, QString::number(i));
    }
}

std::string FightField::CELL_SIZE_ERROR = "cell size is too small"; //прогуглить, где лучше располагать
