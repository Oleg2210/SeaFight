#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include "communication.h"
#include "seafightfield.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QGridLayout;

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
    void shuffleButtonClicked();

private:
    void setUpConnectionWidget();
    void setUpConnectionLayout(QGridLayout*);
    void setUpBattleWidget();
    void setUpBattleLayout(QGridLayout*);
    void resizeMainWindow(const QSize);
    void errorNotify(QJsonObject);
    void letUsPlayNotify(QJsonObject);

    quint16 _port_number;
    QWidget *_start_widget;
    QWidget *_battle_widget;
    const QSize _start_widget_size = QSize(508, 156);

    QLabel *_start_instrucntion_label;
    QLabel *_battle_state_label;
    QLineEdit *_start_host_edit;
    QLineEdit *_start_port_edit;
    QPushButton *_start_connect_button;
    QPushButton *_battle_shuffle_button;
    QPushButton *_battle_ready_button;

    SeaFightField *_your_fight_field;
    SeaFightField *_enemies_fight_field;
};

#endif // VIEW_H
