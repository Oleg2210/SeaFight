#include "fightfield.h"
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QFont>
#include <QFontMetrics>
#include <QRect>
#include <stdexcept>
#include <QDebug>

const QString FightField::FIELD_LETTERS = "АБВГДЕЁЖЗИ";
const std::string FightField::CELL_SIZE_ERROR = "cell size is too small";

FightField::FightField(int padding, int column_size, QFont font, QWidget *parent):
     QWidget(parent), _padding(padding), _column_size(column_size), _font(font)
{
    int size = getOffsetSize()*2 + getFieldSize();
    this->setFixedSize(size, size);
}

FightField::~FightField(){

}

void FightField::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPen pen(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setFont(_font);

    drawFieldLabels(painter);
    drawFieldButtons(painter);

    QWidget::paintEvent(event);
}

void FightField::mousePressEvent(QMouseEvent *event){

    QWidget::mousePressEvent(event);
}

void FightField::drawFieldLabels(QPainter &painter){
    QFontMetrics font_metric(painter.font());
    checkFontToSizeRatio(font_metric);
    drawFieldLetters(painter, font_metric);
    drawFieldNumbers(painter, font_metric);
}

void FightField::drawFieldButtons(QPainter &painter){
    QPainterPath field_path;
    field_path.addRect(getOffsetSize(), getOffsetSize(), getFieldSize(), getFieldSize());
    painter.fillPath(field_path, Qt::white);
    painter.drawPath(field_path);

    int edge = getOffsetSize() + getFieldSize();
    for(int i=1; i < FIELD_LETTERS.length(); i++){
        int indent = getOffsetSize() + i*this->_column_size;
        painter.drawLine(indent, getOffsetSize(), indent, edge);
        painter.drawLine(getOffsetSize(), indent, edge, indent);
    }
}

void FightField::checkFontToSizeRatio(const QFontMetrics &font_metric){
    int top_residual = (this->_column_size - font_metric.height())/2;
    int width_residual = (this->_column_size - font_metric.maxWidth())/2;
    if(top_residual <= 0 || width_residual <= 0)
        throw std::range_error(CELL_SIZE_ERROR);
}

void FightField::drawFieldLetters(QPainter &painter, const QFontMetrics &font_metric){
    auto iter = FIELD_LETTERS.begin();
    int i = 0;

    for(iter, i; iter != FIELD_LETTERS.end(); iter++, i++){
        int left = getOffsetSize() + this->_column_size*i;
        QRect rect(left, this->_padding, this->_column_size, this->_column_size);
        painter.drawText(rect, Qt::AlignCenter, *iter);
    }
}

void FightField::drawFieldNumbers(QPainter &painter, const QFontMetrics &font_metric){
    for(int i=0; i < FIELD_LETTERS.length(); i++){
        int top = getOffsetSize() + this->_column_size*i;
        QRect rect(this->_padding, top, this->_column_size, this->_column_size);
        painter.drawText(rect, Qt::AlignCenter, QString::number(i + 1));
    }
}
