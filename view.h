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

public slots:
    void commandFromModel(QJsonDocument);

signals:
    void commandToModel(QJsonDocument);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void connectButtonClicked();

private:
    void setUpConnectionWidget();
    void errorNotify(QJsonObject);

    QWidget *_start_widget;
    QLabel *_start_instrucntion_label;
    QLineEdit *_start_host_edit;
    QLineEdit *_start_port_edit;
    QPushButton *_start_connect_button;
};

#endif // VIEW_H
