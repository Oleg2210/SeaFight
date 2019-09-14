#include "seafightfield.h"
#include <QRect>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QFont>
#include <QDebug>

SeaFightField::SeaFightField(int padding, int column_size, QFont font, QWidget *parent):
    FightField (padding, column_size, font, parent), _crossed_cell_highlighting(false), _cell_crossed(0), _states_of_cells()
{
    _states_of_cells[1] = CELL_WOUND;
}

SeaFightField::~SeaFightField(){
}

void SeaFightField::paintEvent(QPaintEvent *event){
    FightField::paintEvent(event);
    QPainter painter(this);
    draw_cells(painter);

    if(_crossed_cell_highlighting && _cell_crossed){
        QRect highlight_rect = getRect(_cell_crossed);
        highlight_cell(painter, highlight_rect);
        _cell_crossed = 0;
    }
}

void SeaFightField::mouseMoveEvent(QMouseEvent *event){
    if(_crossed_cell_highlighting){
        int cell_number = getCellNumber(event->x(), event->y());
        _cell_crossed = cell_number;
        update();
    }
    QWidget::mouseMoveEvent(event);
}

void SeaFightField::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        int cell_number = getCellNumber(event->x(), event->y());
        emit cellPressed(cell_number);
    }
    QWidget::mousePressEvent(event);
}

void SeaFightField::highlight_cell(QPainter &painter, QRect rect){
    QPainterPath hl_path;
    hl_path.addRect(rect);
    painter.fillPath(hl_path, Qt::red);
    painter.drawPath(hl_path);
}

void SeaFightField::draw_cells(QPainter &painter){
    for(auto iter = _states_of_cells.begin(); iter != _states_of_cells.end(); iter++){
        int cell_number = iter.key();
        QRect rect = getRect(cell_number);
        int cell_state = iter.value();

        if(cell_state == CELL_WOUND){
            draw_wound(painter, rect);
        }
    }
}

void SeaFightField::draw_wound(QPainter &painter, QRect rect){
    int left_top_x = rect.x();
    int left_top_y = rect.y();
    int right_top_x = left_top_x + _column_size;
    int right_top_y = left_top_y;

    int left_bottom_x = left_top_x;
    int left_bottom_y = left_top_y + _column_size;
    int right_bottom_x = right_top_x;
    int right_bottom_y = right_top_y + _column_size;

    painter.save();
    painter.setPen(QPen(Qt::red, 1, Qt::SolidLine));
    painter.drawLine(left_top_x, left_top_y, right_bottom_x, right_bottom_y);
    painter.drawLine(right_top_x, right_top_y, left_bottom_x, left_bottom_y);
    painter.restore();

}
