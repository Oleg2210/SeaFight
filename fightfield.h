#ifndef FIGHTFIELD_H
#define FIGHTFIELD_H

#include <QWidget>

class QGridLayout;

class FightField: public QWidget
{
    Q_OBJECT

public:
    FightField(QWidget *parent = nullptr);
    ~FightField();
// нужно реализовать функции для изменения внешнего вида кнопки(промах, попадание, деактивация клеток вокруг убитого коробля)

// реализовать слот, принимающий сигнал нажатия на кнопку, и вызывающий новый сигнал с координатами кнопки.
private:
    void drawFieldLabels(QGridLayout *main_layout);
    void drawFieldButtons(QGridLayout *main_layout);
};

#endif // FIGHTFIELD_H
