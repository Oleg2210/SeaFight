#include "fightfield.h"
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QFont>
#include <QFontMetrics>
#include <QRect>
#include <QMouseEvent>
#include <stdexcept>
#include <QDebug>

const QString FightField::FIELD_LETTERS = "АБВГДЕЁЖЗИ";
const int FightField::CELLS_PER_SIDE = FightField::FIELD_LETTERS.length();
const std::string FightField::CELL_SIZE_ERROR = "cell size is too small";

FightField::FightField(int padding, int column_size, QFont font, QWidget *parent):
     QWidget(parent), _padding(padding), _column_size(column_size), _font(font)
{
    setMouseTracking(true);
    int offset_numbers = 2;
    int size = getOffsetSize()*offset_numbers + getFieldSize();
    this->setFixedSize(size, size);
}

FightField::~FightField(){

}

void FightField::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPen pen(Qt::black);
    pen.setWidth(LINE_WIDTH);
    painter.setPen(pen);
    painter.setFont(_font);

    drawFieldLabels(painter);
    drawFieldButtons(painter);

    QWidget::paintEvent(event);
}

void FightField::drawFieldLabels(QPainter &painter){
    QFontMetrics font_metric(painter.font());
    checkFontToSizeRatio(font_metric);
    drawFieldLetters(painter);
    drawFieldNumbers(painter);
}

void FightField::drawFieldButtons(QPainter &painter){
    QRect rect(getOffsetSize(), getOffsetSize(), getFieldSize(), getFieldSize());
    drawRect(painter, rect, Qt::white);

    int edge = getOffsetSize() + getFieldSize();
    for(int i=1; i < CELLS_PER_SIDE; i++){
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

void FightField::drawFieldLetters(QPainter &painter){
    auto iter = FIELD_LETTERS.begin();
    int i = 0;

    for(iter, i; iter != FIELD_LETTERS.end(); iter++, i++){
        int left = getOffsetSize() + this->_column_size*i;
        QRect rect(left, this->_padding, this->_column_size, this->_column_size);
        painter.drawText(rect, Qt::AlignCenter, *iter);
    }
}

void FightField::drawFieldNumbers(QPainter &painter){
    for(int i=0; i < CELLS_PER_SIDE; i++){
        int top = getOffsetSize() + this->_column_size*i;
        QRect rect(this->_padding, top, this->_column_size, this->_column_size);
        painter.drawText(rect, Qt::AlignCenter, QString::number(i + 1));
    }
}

int FightField::getCellNumber(int x, int y){
    int start = getOffsetSize();
    int edge = getOffsetSize() + getFieldSize();
    if((x <= start || y <= start) || (x > edge || y > edge))
        return OUT_OF_FIELD;

    x -= getOffsetSize() + LINE_WIDTH;
    y -= getOffsetSize() + LINE_WIDTH;

    int number_of_cell = (y / this->_column_size) * CELLS_PER_SIDE;
    number_of_cell += (x / this->_column_size) + 1;
    return number_of_cell;
}

QRect FightField::getRect(int cell_number){
    if((cell_number <= OUT_OF_FIELD) || cell_number > CELLS_PER_SIDE*CELLS_PER_SIDE){
        throw std::runtime_error("cell is out");
    }
    cell_number -= 1;
    int y = getOffsetSize() + (cell_number / CELLS_PER_SIDE) * this->_column_size;
    int x = getOffsetSize() + (cell_number % CELLS_PER_SIDE) * this->_column_size;
    QRect rect(x, y, this->_column_size, this->_column_size);
    return rect;
}

void FightField::drawRect(QPainter &painter, QRect rect, QColor color){
    QPainterPath field_path;
    field_path.addRect(rect);
    painter.fillPath(field_path, color);
    painter.drawPath(field_path);
}
