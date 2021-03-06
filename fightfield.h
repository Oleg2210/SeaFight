#ifndef FIGHTFIELD_H
#define FIGHTFIELD_H

#include <QWidget>
#include <QApplication>
#include <QFont>
#include <string>

class FightField: public QWidget
{
    Q_OBJECT

public:
    FightField(int padding=30, int column_size=30, QFont font=QApplication::font(), QWidget *parent = nullptr);
    ~FightField();

protected:
    void paintEvent(QPaintEvent *event);

    int getCellNumber(int x, int y);
    QRect getRect(int cell_number);
    void drawRect(QPainter &painter, QRect rect, QColor color);

    static const QString FIELD_LETTERS;
    static const std::string CELL_SIZE_ERROR;
    static const int CELLS_PER_SIDE = 10;
    static const int TOTAL_CELLS_NUMBER = CELLS_PER_SIDE * CELLS_PER_SIDE;
    static const int LINE_WIDTH = 1;
    static const int OUT_OF_FIELD = 0;

    int _padding;
    int _column_size;
    QFont _font;

    inline int getFieldSize() {
        return _column_size * CELLS_PER_SIDE;
    }

    inline int getOffsetSize(){
        return _padding + _column_size;
    }

private:
    void drawFieldLabels(QPainter &painter);
    void drawFieldButtons(QPainter &painter);
    void checkFontToSizeRatio(const QFontMetrics &font_metric);
    void drawFieldLetters(QPainter &painter);
    void drawFieldNumbers(QPainter &painter);

};

#endif // FIGHTFIELD_H
