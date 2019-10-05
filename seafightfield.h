#ifndef SEAFIGHTFIED_H
#define SEAFIGHTFIED_H

#include "fightfield.h"

class SeaFightField: public FightField
{
    Q_OBJECT

public:
    static const int CELL_WOUND = 1;
    static const int CELL_MISS = 2;
    static const int CELL_SHIP = 3;
    static const int CELL_GHOST = 4;

    static const int PEN_WIDTH = 2;
    static const int PEN_IMPERCISION = 1;

    SeaFightField(int padding=30, int column_size=30, QFont font=QApplication::font(), QWidget *parent = nullptr);
    ~SeaFightField();

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

    void clearGhostShips();
    void highlightCell(QPainter &painter, QRect rect);
    void drawCells(QPainter &painter);
    void drawWound(QPainter &painter, QRect rect);
    void drawMiss(QPainter &painter, QRect rect);
    void drawShip(QPainter &painter, QRect rect);
};

#endif // SEAFIGHTFIED_H
