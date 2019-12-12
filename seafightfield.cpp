#include "seafightfield.h"
#include <QRect>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QFont>
#include <QDebug>


SeaFightField::SeaFightField(int padding, int column_size, QFont font, QWidget *parent):
    FightField (padding, column_size, font, parent), _crossed_cell_highlighting(false), _ship_dragging(false),
    _cell_crossed(OUT_OF_FIELD), _cell_dragged(OUT_OF_FIELD), _cell_ghost(OUT_OF_FIELD), _states_of_cells()
{
//    _states_of_cells[1] = CELL_WOUND;
//    _states_of_cells[2] = CELL_WOUND;
//    _states_of_cells[3] = CELL_MISS;
//    _states_of_cells[11] = CELL_MISS;
//    _states_of_cells[12] = CELL_MISS;
//    _states_of_cells[13] = CELL_MISS;

    _states_of_cells[77] = CELL_SHIP;
}

SeaFightField::~SeaFightField(){
}

void SeaFightField::paintEvent(QPaintEvent *event){
    FightField::paintEvent(event);
    QPainter painter(this);
    drawCells(painter);
    //_states_of_cells.remove(_cell_ghost);
    //_cell_ghost = OUT_OF_FIELD;
//    clearGhostShips();

    if(_crossed_cell_highlighting && _cell_crossed){
        if(_states_of_cells.find(_cell_crossed) == _states_of_cells.end()){
            QRect highlight_rect = getRect(_cell_crossed);
            highlightCell(painter, highlight_rect);
            _cell_crossed = OUT_OF_FIELD;
        }
    }
}

void SeaFightField::mouseMoveEvent(QMouseEvent *event){
    if(_crossed_cell_highlighting){
        int cell_number = getCellNumber(event->x(), event->y());
        _cell_crossed = cell_number;
        if(_cell_crossed != OUT_OF_FIELD){
            update();
        }
    }
//    else if(_ship_dragging){
//        if(event->buttons() == Qt::LeftButton){
//            int cell_number = getCellNumber(event->x(), event->y());
//            if(cell_number != OUT_OF_FIELD && _cell_dragged != OUT_OF_FIELD){
//                _states_of_cells[cell_number] = CELL_GHOST;
//                _cell_ghost = cell_number;
//                update();
//            }
//        }
//    }
//    else if(_ship_dragging){
//        if(event->buttons() == Qt::LeftButton){
//            int cell_number = getCellNumber(event->x(), event->y());
//            if(cell_number != OUT_OF_FIELD && _cell_dragged != OUT_OF_FIELD){
//                _states_of_cells[cell_number] = CELL_GHOST;
//                update();
//            }
//        }
//    }
    QWidget::mouseMoveEvent(event);
}

void SeaFightField::mousePressEvent(QMouseEvent *event){
    qDebug()<<_states_of_cells;
    if(event->button() == Qt::LeftButton){
        int cell_number = getCellNumber(event->x(), event->y());
        if(cell_number != OUT_OF_FIELD){
            if(_ship_dragging){
                auto states_iter = _states_of_cells.find(cell_number);
                if(states_iter != _states_of_cells.end() && (states_iter.value() == CELL_SHIP)){
                    _cell_dragged = cell_number;
                }
            }else{
                emit cellPressed(cell_number);
            }

        }
    }
    qDebug()<<_states_of_cells;
    QWidget::mousePressEvent(event);
}

void SeaFightField::mouseReleaseEvent(QMouseEvent *event){
    qDebug()<<"release";
    if(event->button() == Qt::LeftButton){
        int cell_number = getCellNumber(event->x(), event->y());
        if(cell_number != OUT_OF_FIELD && _ship_dragging){
            if(_cell_dragged != OUT_OF_FIELD){
                _states_of_cells.remove(_cell_dragged);
                _states_of_cells[cell_number] = CELL_SHIP;
            }
            _cell_dragged = OUT_OF_FIELD;
            update();
        }
    }
    QWidget::mouseReleaseEvent(event);
}

void SeaFightField::drawCells(QPainter &painter){
    for(auto iter = _states_of_cells.begin(); iter != _states_of_cells.end(); iter++){
        int cell_number = iter.key();
        QRect rect = getRect(cell_number);
        int cell_state = iter.value();

        switch(cell_state){
            case CELL_WOUND: drawWound(painter, rect); break;
            case CELL_MISS: drawMiss(painter, rect); break;
            case CELL_SHIP: drawShip(painter, rect); break;
            case CELL_GHOST: drawShip(painter, rect); break;
        }
    }
}

void SeaFightField::highlightCell(QPainter &painter, QRect rect){
    int x = rect.x() + PEN_IMPERCISION;
    int y = rect.y() + PEN_IMPERCISION;
    int size = rect.width() - PEN_IMPERCISION;
    rect = QRect(x, y, size, size);

    painter.save();
    QPen pen = QPen(QColor("#FF4500"), PEN_WIDTH);
    painter.setPen(pen);
    painter.drawRect(rect);
    painter.restore();
}

void SeaFightField::drawWound(QPainter &painter, QRect rect){
    const int p_imp = PEN_IMPERCISION;
    int left_top_x = rect.x();
    int left_top_y = rect.y();
    int right_top_x = left_top_x + _column_size;
    int right_top_y = left_top_y;

    int left_bottom_x = left_top_x;
    int left_bottom_y = left_top_y + _column_size;
    int right_bottom_x = right_top_x;
    int right_bottom_y = right_top_y + _column_size;

    painter.save();
    painter.setPen(QPen(QColor("#FF4500"), PEN_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::SvgMiterJoin));
    painter.drawLine(left_top_x + p_imp, left_top_y + p_imp, right_bottom_x - p_imp, right_bottom_y - p_imp);
    painter.drawLine(right_top_x - p_imp, right_top_y + p_imp, left_bottom_x + p_imp, left_bottom_y - p_imp);
    painter.restore();
}

void SeaFightField::drawMiss(QPainter &painter, QRect rect){
    int center_x = rect.x() + _column_size / 2;
    int center_y = rect.y() + _column_size / 2;
    int small_size = _column_size / 4;
    int small_rect_x = center_x - small_size / 2;
    int small_rect_y = center_y - small_size / 2;
    QRect small_rect = QRect(small_rect_x, small_rect_y, small_size, small_size);

    drawRect(painter, rect, QColor("#B8B8B8"));
    painter.save();
    painter.setBrush(QColor("#000000"));
    painter.drawEllipse(small_rect);
    painter.restore();
}

void SeaFightField::drawShip(QPainter &painter, QRect rect){
    drawRect(painter, rect, QColor("#20B2AA"));
}

void SeaFightField::clearGhostShips(){
    auto it = _states_of_cells.begin();
    while(it != _states_of_cells.end()){
        if(*it == CELL_GHOST){
            it = _states_of_cells.erase(it);
        }else{
            ++it;
        }
    }
}
