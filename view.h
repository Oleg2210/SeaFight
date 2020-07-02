#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include "communication.h"

class QLabel;
class QLineEdit;
class QPushButton;

class View: public QMainWindow
{
    Q_OBJECT

public:
    View(QWidget *parent=nullptr);
    void setPortNumber(quint16);


public slots:
    void commandFromModel(QJsonObject);

signals:
    void commandToModel(QJsonObject);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void connectButtonClicked();

private:
    void setUpConnectionWidget();
    void setUpBattleWidget();
    void errorNotify(QJsonObject);
    void letUsPlayNotify(QJsonObject);

    QWidget *_start_widget;
    QWidget *_battle_widget;
    QLabel *_start_instrucntion_label;
    QLineEdit *_start_host_edit;
    QLineEdit *_start_port_edit;
    QPushButton *_start_connect_button;
    quint16 _port_number;
};

#endif // VIEW_H
