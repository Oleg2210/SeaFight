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
    connect(_server_socket, SIGNAL(newConnection()), this, SLOT(someConnection()));
    if(_server_socket->listen(QHostAddress::Any, SFcom::PORT_NUMBER)){
        connect(_view, SIGNAL(commandToModel(QJsonDocument)), this, SLOT(commandFromView(QJsonDocument)));
        connect(this, SIGNAL(commandToView(QJsonDocument)), _view, SLOT(commandFromModel(QJsonDocument)));
        _view->show();
        return true;
    }else{
        QString port_error = tr("Port %port_numb is already allocated");
        port_error.replace("%port_numb", QString::number(SFcom::PORT_NUMBER));
        QMessageBox::critical(nullptr, "", port_error);
        return false;
    }
}

void Model::commandFromView(QJsonDocument json_doc){
    QJsonObject json_obj = json_doc.object();

    if(json_obj["command"] == SFcom::Commands::LETUSSPLAY){
        viewLetUsPlay(json_doc);
    }
}

void Model::viewLetUsPlay(QJsonDocument json_doc){
    QJsonObject json_obj = json_doc.object();
    if(json_obj["status"] == SFcom::Status::REQUEST){
        if(_connection_status == SFcom::ConnectionType::NOCONN){
            QJsonObject payload = json_obj["payload"].toObject();
            _client_socket = new QTcpSocket(this);
            _client_socket->connectToHost(payload["IP"].toString(), payload["port"].toInt());
            connectPeersHandlers();
            _connection_status = SFcom::ConnectionType::OUTCOMINGCONN;
        }
    }
}

void Model::connectPeersHandlers(){
    connect(clientSocket,SIGNAL(connected()),this,SLOT(connectedToPeer()));
    connect(clientSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(peerConnectionError(QAbstractSocket::SocketError)));
    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(messageFromPeer()));
}

void Model::someConnection(){
    if(_connection_status == SFcom::ConnectionType::NOCONN){
        _client_socket = _server_socket->nextPendingConnection();
        _connection_status = SFcom::ConnectionType::INCOMINGCONN;
        connectPeersHandlers();
    }else{
        QTcpSocket *temp = _server_socket->nextPendingConnection();
        temp->disconnectFromHost();
        temp->deleteLater();
    }
}
