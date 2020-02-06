#include "model.h"
#include <QMessageBox>
#include <QApplication>

Model::Model(QObject *parent) : QObject(parent)
{
    _view = new View();
    _server_socket=new QTcpServer(this);
}

Model::~Model(){
    delete _view;
}

void Model::run(){
    connect(_server_socket,SIGNAL(newConnection()),this,SLOT(someConnection()));
    if(_server_socket->listen(QHostAddress::Any, _PORT_NUMBER)){
        _view->show();
    }else{
        QString port_error = tr("Port %port_numb is already allocated");
        port_error.replace("%port_numb", QString::number(_PORT_NUMBER));
        QMessageBox::critical(nullptr, "", port_error);
        QApplication::instance()->exit(); //not execute
    }
}

void Model::someConnection(){

}
