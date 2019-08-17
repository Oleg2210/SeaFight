#ifndef FIGHTFIELD_H
#define FIGHTFIELD_H

#include <QWidget>

class QPainter;

class FightField: public QWidget
{
    Q_OBJECT

public:
    FightField(QWidget *parent = nullptr);
    ~FightField();
// нужно реализовать функции для изменения внешнего вида кнопки(промах, попадание, деактивация клеток вокруг убитого коробля)

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

// реализовать слот, принимающий сигнал нажатия на кнопку, и вызывающий новый сигнал с координатами кнопки.
private:
    int start_left_position = 34;
    int start_top_position = 24;
    int column_width = 30;
    int column_height = 30;

    void drawFieldLabels(QPainter *painter);
    void drawFieldButtons(QPainter *painter);
};

#endif // FIGHTFIELD_H
