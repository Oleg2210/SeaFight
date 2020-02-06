#include "view.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QRegularExpression>
#include <QHostAddress>
#include <QMessageBox>
#include <QDebug>

View::View(QWidget *parent):
    QMainWindow(parent)
{
    setUpConnectionWidget();
    connect(_start_connect_button, SIGNAL(clicked()), this, SLOT(connectButtonClicked()));
    setCentralWidget(_start_widget);
}

void View::setUpConnectionWidget(){
    _start_instrucntion_label = new QLabel(tr("To play input host adress and port of your opponent(your port is 55433)."));
    QLabel *start_host_label = new QLabel(tr("Host adress"));
    QLabel *start_port_label = new QLabel(tr("Port number"));
    _start_host_edit = new QLineEdit;
    _start_port_edit = new QLineEdit;
    _start_connect_button = new QPushButton(tr("Connect"));
    QGridLayout *start_layout = new QGridLayout;

    start_layout->setVerticalSpacing(14);
    _start_instrucntion_label->setMinimumHeight(60);
    _start_host_edit->setFixedWidth(140);
    _start_port_edit->setFixedWidth(100);

    start_layout->addWidget(_start_instrucntion_label, 0, 0, 1, 4);
    start_layout->addWidget(start_host_label, 2, 0);
    start_layout->addWidget(_start_host_edit, 2, 1);
    start_layout->addWidget(start_port_label, 2, 2);
    start_layout->addWidget(_start_port_edit, 2, 3);
    start_layout->addWidget(_start_connect_button, 3, 3, 1, 1);

    _start_widget=new QWidget(this);
    _start_widget->setLayout(start_layout);
    _start_widget->setFixedSize(start_layout->sizeHint());
}

void View::resizeEvent(QResizeEvent *event){
    QSize widgetSize = centralWidget()->size();
    int widgetX = (width() - widgetSize.width()) / 2;
    int widgetY = (height() - widgetSize.height()) / 2;
    centralWidget()->setGeometry(widgetX, widgetY, widgetSize.width(), widgetSize.height());
    QWidget::resizeEvent(event);
}

void View::connectButtonClicked(){
    QString ip_input = _start_host_edit->text();
    QString port_input = _start_port_edit->text();

    QRegularExpression port_reg("^[0-9]{1,5}$");
    bool port_state = port_reg.match(port_input).hasMatch();
    port_state = port_state && (port_input.toInt() <= 65535) && (port_input.toInt() > 0);
    bool ip_state = !QHostAddress(ip_input).isNull();

    if(ip_state && port_state){
        qDebug()<<"ok";
    }else{
        QMessageBox::information(_start_widget, "", tr("IP adress or port number are wrong!"));
    }
}

