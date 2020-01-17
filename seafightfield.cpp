#include "seafightfield.h"
#include <QRect>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QFont>
#include <QDebug>
#include <QRandomGenerator>
#include <algorithm>

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

//    _states_of_cells[25] = CELL_SHIP;
//    _states_of_cells[26] = CELL_SHIP;
//    _states_of_cells[27] = CELL_SHIP;
//    _states_of_cells[28] = CELL_SHIP;

//    _states_of_cells[47] = CELL_SHIP;
//    _states_of_cells[48] = CELL_SHIP;

//    _states_of_cells[67] = CELL_SHIP;
//    _states_of_cells[77] = CELL_SHIP;
//    _states_of_cells[87] = CELL_SHIP;
//    _states_of_cells[97] = CELL_SHIP;
//    randomArrangement();
    randomArrangement();
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
    if(event->button() == Qt::LeftButton){
        int cell_number = getCellNumber(event->x(), event->y());

        if(cell_number != OUT_OF_FIELD){
            if(_ship_dragging){
                auto states_iter = _states_of_cells.find(cell_number);
                if((states_iter != _states_of_cells.end()) && (*states_iter == CELL_SHIP)){
                    _cell_dragged = cell_number;
                }
            }else{
                emit cellPressed(cell_number);
            }
        }
    }
    QWidget::mousePressEvent(event);
}

void SeaFightField::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        int cell_number = getCellNumber(event->x(), event->y());

        if((cell_number != OUT_OF_FIELD) && (_cell_dragged != OUT_OF_FIELD) && _ship_dragging){
            auto neighbour_ships = getNeighbourShips(_cell_dragged);
            if(cell_number != _cell_dragged){
                dragShips(_cell_dragged, cell_number, neighbour_ships);
                qDebug()<<"drag";
            }else if(neighbour_ships.length() > 1){
                qDebug()<<"turn";
                turnShip(neighbour_ships);
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

QVector<int> SeaFightField::getNeighbourShips(int cell_number){
    QVector<int> neighbour_ships;

    for(int direction=UP;direction<=RIGHT;direction++){
        QVector<int> neigbours = getNeighbourShipsByDirection(cell_number, direction);
        if(direction == UP || direction == LEFT){
            std::reverse(neigbours.begin(), neigbours.end());
        }
        neighbour_ships += neigbours;
    }

    neighbour_ships.append(cell_number);
    std::sort(neighbour_ships.begin(), neighbour_ships.end());

    return neighbour_ships;
}

QVector<int> SeaFightField::getNeighbourShipsByDirection(int cell_number, int direction){ //слишком длинное название?
    QVector<int> neighbour_ships;
    int next_cell_number=cell_number;

    while(_states_of_cells.contains(next_cell_number) && (_states_of_cells[next_cell_number] == CELL_SHIP)){
        neighbour_ships.append(next_cell_number);
        switch (direction) {
            case UP: next_cell_number -= CELLS_PER_SIDE; break;
            case DOWN: next_cell_number += CELLS_PER_SIDE; break;
            case LEFT: next_cell_number -= 1; break;
            case RIGHT: next_cell_number += 1; break;
        }
    }

    neighbour_ships.erase(neighbour_ships.begin());
    return neighbour_ships;
}

void SeaFightField::dragShips(int from_cell, int to_cell, QVector<int> neighbour_ships){ //optimize
    int offset = to_cell - from_cell;
    QVector<int> drag_positions;
    for(int position: neighbour_ships)
        drag_positions.append(position + offset);

    if(dragValid(drag_positions)){
        for(int position: neighbour_ships)
            _states_of_cells.remove(position);

        QSet<int> drag_neighbours = getNeighbourCells(drag_positions);
        for(int cell_number: drag_neighbours){
            if(_states_of_cells.find(cell_number) != _states_of_cells.end()){
                for(int position: neighbour_ships)
                    _states_of_cells[position] = CELL_SHIP;

                return;
            }

        }

        for(int position: neighbour_ships)
            _states_of_cells[position + offset] = CELL_SHIP;
    }
}

QSet<int> SeaFightField::getNeighbourCells(QVector<int> cells){
    QSet<int> neighbour_cells;
    const int ROWS_NUMBER = 3;
    for(int cell_number: cells){
        int position = cell_number - CELLS_PER_SIDE;
        bool has_left = (position % CELLS_PER_SIDE != 1)?true: false;
        bool has_right = (position % CELLS_PER_SIDE)?true: false;

        for(int i=0; i<ROWS_NUMBER; i++){
            int next_cell_num = position + CELLS_PER_SIDE*i;

            if((next_cell_num > 0) && (next_cell_num < TOTAL_CELLS_NUMBER)){
                neighbour_cells.insert(next_cell_num);
                if(has_left)
                    neighbour_cells.insert(next_cell_num - 1);
                if(has_right)
                    neighbour_cells.insert(next_cell_num + 1);
            }
        }
    }

    for(int cell_number: cells)
        neighbour_cells.remove(cell_number);

    return neighbour_cells;
}

bool SeaFightField::dragValid(QVector<int> to_positions){
    if(to_positions.length() > 1){
        bool vertical_dir = ((to_positions[1]-to_positions[0]) < CELLS_PER_SIDE)?false: true;
        if(vertical_dir){
            for(int position: to_positions){
                if((position < 0) || (position > TOTAL_CELLS_NUMBER))
                    return false;
            }
        }else{
            if(!(to_positions[0] % CELLS_PER_SIDE))
                return false;

            int row = to_positions[0] / CELLS_PER_SIDE;
            for(int position: to_positions){
                if((position/CELLS_PER_SIDE != row) && (position % CELLS_PER_SIDE))
                    return false;
            }
        }
    }

    return true;
}

void SeaFightField::turnShip(QVector<int> neighbour_ships){
    QVector<int> turn_positions;
    bool vertical_dir = ((neighbour_ships[1]-neighbour_ships[0]) < CELLS_PER_SIDE)?false: true;

    for(int ship_number=0; ship_number<neighbour_ships.length(); ship_number++){
        if(vertical_dir){
            turn_positions.append(neighbour_ships[0] + ship_number);
        }else{
            turn_positions.append(neighbour_ships[0] + CELLS_PER_SIDE*ship_number);
        }
    }

    replaceShips(neighbour_ships, turn_positions);
}

void SeaFightField::replaceShips(QVector<int> start_positions, QVector<int> replace_positions){
    if(dragValid(replace_positions)){
        for(int position: start_positions)
            _states_of_cells.remove(position);

        QSet<int> replace_cells = getNeighbourCells(replace_positions);
        for(int cell_number: replace_positions)
            replace_cells.insert(cell_number);

        for(int cell_number: replace_cells){
            if(_states_of_cells.find(cell_number) != _states_of_cells.end()){
                for(int position: start_positions)
                    _states_of_cells[position] = CELL_SHIP;

                return;
            }
        }

        for(int position: replace_positions)
            _states_of_cells[position] = CELL_SHIP;
    }
}

void SeaFightField::randomArrangement(){
    const int DECK_MAX = 4;
    QSet<int> engaged_cells;

    for(int decks_number=1; decks_number<=DECK_MAX; decks_number++){
        int ship_numbers = DECK_MAX - decks_number + 1;

        for(int ship_number=0; ship_number<ship_numbers; ship_number++){
            createShip(decks_number, engaged_cells);
        }
    }
}

void SeaFightField::createShip(int decks_number, QSet<int> &engaged_cells){
    while(true){
        QVector<int> ship;
        int direction = QRandomGenerator::global()->generate() % 2;
        int random_cell = QRandomGenerator::global()->generate() % 101;
        bool is_valid = true;

        for(int cell_number,deck=0; deck<decks_number; deck++){
            cell_number = random_cell + ((direction)?deck: CELLS_PER_SIDE*deck);
            qDebug()<<cell_number;
            if(engaged_cells.find(cell_number) == engaged_cells.end()){
                ship.append(cell_number);
            }else{
                is_valid = false;
                break;
            }
        }

        if(is_valid && dragValid(ship)){
            engaged_cells += getNeighbourCells(ship);
            for(int cell_number: ship){
                _states_of_cells[cell_number] = CELL_SHIP;
                engaged_cells.insert(cell_number);
            }
            break;
        }
    }
}
