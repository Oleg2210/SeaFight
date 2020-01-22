#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>

class View: public QMainWindow
{
    Q_OBJECT

public:
    View(QWidget *parent=nullptr);
    ~View();
};

#endif // VIEW_H
