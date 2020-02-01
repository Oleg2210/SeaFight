#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>

class QLabel;
class QLineEdit;

class View: public QMainWindow
{
    Q_OBJECT

public:
    View(QWidget *parent=nullptr);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void connectButtonClicked();

private:
    QWidget *startWidget;
    QLabel *startInstrucntionLabel;
    QLineEdit *startHostEdit;
    QLineEdit *startPortEdit;
};

#endif // VIEW_H
