#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>

class QLabel;
class QLineEdit;
class QPushButton;

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
    void setUpConnectionWidget();

    QWidget *startWidget;
    QLabel *startInstrucntionLabel;
    QLineEdit *startHostEdit;
    QLineEdit *startPortEdit;
    QPushButton *startConnectButton;
};

#endif // VIEW_H
