#ifndef FIGHTFIELD_H
#define FIGHTFIELD_H

#include <QWidget>
#include <QApplication>
#include <QFont>
#include <string>

class QFontMetrics;
class QPainter;

class FightField: public QWidget
{
    Q_OBJECT

public:
    FightField(int padding=30, int column_size=30, QFont font=QApplication::font(), QWidget *parent = nullptr); // добавить цвета
    ~FightField();
// нужно реализовать функции для изменения внешнего вида кнопки(промах, попадание, деактивация клеток вокруг убитого коробля)

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

    int getCellNumber(int x, int y);
    QRect getRect(int cell_number);

    static const QString FIELD_LETTERS;
    static const std::string CELL_SIZE_ERROR;
    static const int LINE_WIDTH = 1;

    int _padding;
    int _column_size;
    QFont _font;
    QRect *rectum;

    inline int getFieldSize() {
        return _column_size*FIELD_LETTERS.length();
    }

    inline int getOffsetSize(){
        return _padding + _column_size;
    }

private:
    void drawFieldLabels(QPainter &painter);
    void drawFieldButtons(QPainter &painter);
    void checkFontToSizeRatio(const QFontMetrics &font_metric);
    void drawFieldLetters(QPainter &painter, const QFontMetrics &font_metric);
    void drawFieldNumbers(QPainter &painter, const QFontMetrics &font_metric);

};

#endif // FIGHTFIELD_H
