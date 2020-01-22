#ifndef SEAFIGHTFIED_H
#define SEAFIGHTFIED_H

#include "fightfield.h"
#include <QVector>


class SeaFightField: public FightField
{
    Q_OBJECT

public:    
    enum {CELL_WOUND, CELL_MISS, CELL_SHIP};
    enum {UP, DOWN, LEFT, RIGHT}; //UP must stands first, RIGHT - last

    static const int PEN_WIDTH = 2;
    static const int PEN_IMPERCISION = 1;

    SeaFightField(int padding=30, int column_size=30, QFont font=QApplication::font(), QWidget *parent = nullptr);
    ~SeaFightField();

    QVector<int> getNeighbourShips(int cell_number);
    QVector<int> getNeighbourShipsByDirection(int cell_number, int direction);
    void dragShips(int from_cell, int to_cell, QVector<int> neighbour_ships);
    bool dragValid(QVector<int> to_positions);
    QSet<int> getNeighbourCells(QVector<int> cells);
    void turnShip(QVector<int> neighbour_ships);
    void replaceShips(QVector<int> start_positions, QVector<int> replace_positions);

    void randomArrangement();
    void createShip(int deck_number, QSet<int> &engaged_cells);

    inline void highlight_cell(bool hl_status){
        _crossed_cell_highlighting = hl_status;
    }

    inline void drag_ship(bool drag_status){
        _ship_dragging = drag_status;
    }

signals:
    void cellPressed(int cell_number); // rename to hit, drag i.e.

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool _crossed_cell_highlighting;
    bool _ship_dragging;
    int _cell_crossed;
    int _cell_dragged;
    int _cell_ghost;
    QHash<int, int> _states_of_cells;
    void highlightCell(QPainter &painter, QRect rect);
    void drawCells(QPainter &painter);
    void drawWound(QPainter &painter, QRect rect);
    void drawMiss(QPainter &painter, QRect rect);
    void drawShip(QPainter &painter, QRect rect);
};

#endif // SEAFIGHTFIED_H
