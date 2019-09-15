#ifndef SEAFIGHTFIED_H
#define SEAFIGHTFIED_H

#include "fightfield.h"

class SeaFightField: public FightField
{
    Q_OBJECT

public:
    static const int CELL_WOUND = 0;
    static const int CELL_MISS = 1;
    static const int CELL_SHIP = 2;

    SeaFightField(int padding=30, int column_size=30, QFont font=QApplication::font(), QWidget *parent = nullptr);
    ~SeaFightField();

    inline void highlight_cell(bool hl_status){
        _crossed_cell_highlighting = hl_status;
    }

signals:
    void cellPressed(int cell_number);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    bool _crossed_cell_highlighting;
    int _cell_crossed;
    QHash<int, int> _states_of_cells;

    void highlightCell(QPainter &painter, QRect rect);
    void drawCells(QPainter &painter);
    void drawWound(QPainter &painter, QRect rect);
    void drawMiss(QPainter &painter, QRect rect);
    void drawShip(QPainter &painter, QRect rect);
};

#endif // SEAFIGHTFIED_H
