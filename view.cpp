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
    QString instuction = tr("To play input host adress and port of your opponent(your port is %port_numb).");
    instuction.replace("%port_numb", QString::number(SFcom::PORT_NUMBER));

    _start_instrucntion_label = new QLabel(instuction);
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

void View::commandFromModel(QJsonDocument json_doc){
    QJsonObject json_obj = json_doc.object();
    if(json_obj["command"] == SFcom::Commands::ERROR){
        errorNotify(json_obj);
    }
}

void View::errorNotify(QJsonObject json_obj){
    QString error_string = "connection error occurred";
    if(json_obj["status"] == SFcom::Status::LOGICERROR)
        error_string = "unknown error occurred";
    QMessageBox::critical(nullptr, "", error_string);
}

void View::connectButtonClicked(){
    QString ip_input = _start_host_edit->text();
    QString port_input = _start_port_edit->text();
    int port_number = port_input.toInt();

    QRegularExpression port_reg("^[0-9]{1,5}$");
    bool port_state = port_reg.match(port_input).hasMatch();
    port_state = port_state && (port_number <= 65535) && (port_number > 0);
    bool ip_state = !QHostAddress(ip_input).isNull();

    if(ip_state && port_state){
        QJsonObject payload;
        payload["IP"] = ip_input;
        payload["port"] = port_number;
        QJsonDocument json_doc = SFcom::createJsonCommand(SFcom::Commands::LETUSSPLAY, SFcom::Status::REQUEST, payload);
        emit commandToModel(json_doc);
    }else{
        QMessageBox::information(_start_widget, "", tr("IP adress or port number are wrong!"));
    }
}

