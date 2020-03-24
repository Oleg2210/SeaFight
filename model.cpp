#include "model.h"
#include <QMessageBox>
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>

Model::Model(QObject *parent) : QObject(parent)
{
    _view = new View();
    _server_socket=new QTcpServer(this);
    _connection_status = SFcom::ConnectionType::NOCONN;
}

Model::~Model(){
    delete _view;
}

bool Model::run(){
    connect(_server_socket,SIGNAL(newConnection()),this,SLOT(someConnection()));
    if(_server_socket->listen(QHostAddress::Any, SFcom::PORT_NUMBER)){
        _view->show();
        return true;
    }else{
        QString port_error = tr("Port %port_numb is already allocated");
        port_error.replace("%port_numb", QString::number(SFcom::PORT_NUMBER));
        QMessageBox::critical(nullptr, "", port_error);
        return false;
    }
}

void Model::someConnection(){
    if(_connection_status == SFcom::ConnectionType::NOCONN){
        _client_socket = _server_socket->nextPendingConnection();
        _connection_status = SFcom::ConnectionType::INCOMINGCONN;

//        connect(clientSocket,SIGNAL(connected()),this,SLOT(connectedToPeer()));
//        connect(clientSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(peerConnectionError(QAbstractSocket::SocketError)));
//        connect(clientSocket,SIGNAL(readyRead()),this,SLOT(messageFromPeer()));
    }else{
        QTcpSocket *temp = _server_socket->nextPendingConnection();
        temp->disconnectFromHost();
        temp->deleteLater();
    }
}
