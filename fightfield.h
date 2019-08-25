#ifndef FIGHTFIELD_H
#define FIGHTFIELD_H

#include <QWidget>

class QPainter;

class FightField: public QWidget
{
    Q_OBJECT

public:
    FightField(int field_height=340, int field_width=340, int start_left=24,
               int start_top=24, int column_width=30, int column_height=30, QWidget *parent = nullptr);
    ~FightField();
// нужно реализовать функции для изменения внешнего вида кнопки(промах, попадание, деактивация клеток вокруг убитого коробля)

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

// реализовать слот, принимающий сигнал нажатия на кнопку, и вызывающий новый сигнал с координатами кнопки.
private:    static const int field_dimension = 10;
    int _field_height;
    int _field_width;
    int _start_left;
    int _start_top;
    int _column_width;
    int _column_height;

    void drawFieldLabels(QPainter *painter);
    void drawFieldButtons(QPainter *painter);
};

#endif // FIGHTFIELD_H
