#ifndef FIGHTFIELD_H
#define FIGHTFIELD_H

#include <QWidget>
#include <string>

class QFontMetrics;
class QPainter;

class FightField: public QWidget
{
    Q_OBJECT

public:
    FightField(int padding=30, int column_size=30, QWidget *parent = nullptr); //передавать QFont
    ~FightField();
// нужно реализовать функции для изменения внешнего вида кнопки(промах, попадание, деактивация клеток вокруг убитого коробля)

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

// реализовать слот, принимающий сигнал нажатия на кнопку, и вызывающий новый сигнал с координатами кнопки.
private:
    static const int FIELD_DIMENSION = 10;
    static std::string CELL_SIZE_ERROR;

    int _padding;
    int _column_size;

    inline int getFieldSize() {
        return _column_size*FIELD_DIMENSION;
    }

    void drawFieldLabels(QPainter &painter);
    void drawFieldChars(QPainter &painter, const QFontMetrics &font_metric);
    void drawFieldNumbers(QPainter &painter, const QFontMetrics &font_metric);
    void drawFieldButtons(QPainter &painter);
};

#endif // FIGHTFIELD_H
