#include "model.h"
#include <QMessageBox>
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>

Model::Model(QObject *parent) : QObject(parent)
{
    _view = new View();
    _server_socket = new QTcpServer(this);
    _connection_status = SFcom::ConnectionType::NOCONN;
    _game_phase = SFcom::GamePhase::CONNECTION;
    _next_block_size = 0;
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

    if(json_obj["command"] == SFcom::Commands::LETUSPLAY){
        viewLetUsPlay(json_doc);
    }
}

void Model::viewLetUsPlay(QJsonDocument json_doc){
    QJsonObject json_obj = json_doc.object();
    if(json_obj["status"] == SFcom::Status::REQUEST){
        if(_connection_status == SFcom::ConnectionType::NOCONN){
            QJsonObject payload = json_obj["payload"].toObject();
            _connection_status = SFcom::ConnectionType::OUTCOMINGCONN;
            _client_socket = new QTcpSocket(this);
            connectPeersHandlers();
            _client_socket->connectToHost(payload["IP"].toString(), payload["port"].toInt());
        }
    }
}

void Model::connectPeersHandlers(){
    connect(_client_socket, SIGNAL(connected()), this, SLOT(connectedToPeer()));
    connect(_client_socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(peerConnectionError(QAbstractSocket::SocketError)));
    connect(_client_socket, SIGNAL(readyRead()), this, SLOT(commandFromPeer()));
}

void Model::someConnection(){
    if(_connection_status == SFcom::ConnectionType::NOCONN){
        qDebug()<<"some connection";
        _client_socket = _server_socket->nextPendingConnection();
        _connection_status = SFcom::ConnectionType::INCOMINGCONN;
        connectPeersHandlers();
    }else{
        QTcpSocket *temp = _server_socket->nextPendingConnection();
        temp->disconnectFromHost();
        temp->deleteLater();
    }
}

void Model::connectedToPeer(){
    if(_connection_status == SFcom::ConnectionType::OUTCOMINGCONN){
        QJsonDocument json_doc = SFcom::createJsonCommand(SFcom::Commands::LETUSPLAY, SFcom::Status::REQUEST);
        writeToPeer(json_doc);
    }
}

void Model::disconnectFromPeer(QJsonDocument json_doc){
    if(_client_socket->isOpen())
        _client_socket->close();
    _client_socket->disconnect();
    _client_socket->deleteLater();
    _client_socket = nullptr;

    _connection_status = SFcom::ConnectionType::NOCONN; //add payload gamephase
    if(_game_phase != SFcom::GamePhase::CONNECTION){
        _game_phase = SFcom::GamePhase::CONNECTION;
        emit commandToView(json_doc);
    }
}

void Model::peerConnectionError(QAbstractSocket::SocketError){
    disconnectFromPeer(SFcom::createJsonCommand(SFcom::Commands::ERROR, SFcom::Status::CONNERROR));
}

void Model::peerLogicError(){
    disconnectFromPeer(SFcom::createJsonCommand(SFcom::Commands::ERROR, SFcom::Status::LOGICERROR));
}

void Model::commandFromPeer(){
    QDataStream input_stream(_client_socket);
    if(!_next_block_size){
        if(_client_socket->bytesAvailable() >= sizeof(_next_block_size))
            input_stream >> _next_block_size;
        else
            return;
    }

    if(_next_block_size <= _client_socket->bytesAvailable()){
        QByteArray command;
        command.resize(_next_block_size);
        input_stream.readRawData(command.data(), _next_block_size);
        _next_block_size = 0;
        QJsonDocument json_doc = QJsonDocument::fromJson(command);
        analizePeerCommand(json_doc);
    }
}

void Model::analizePeerCommand(QJsonDocument json_doc){
    QJsonObject json_obj = json_doc.object();
    if(SFcom::checkCommandFormat(json_obj)){
        switch (json_obj["command"].toInt()){
            case SFcom::Commands::LETUSPLAY: peerLetUsPlay(json_obj); break;
            default: peerLogicError(); break;
        }
    }else{
        peerLogicError();
    }
}

void Model::writeToPeer(QJsonDocument json_doc){
    QByteArray message;
    QByteArray json_string = json_doc.toJson();
    QDataStream output_stream(&message, QIODevice::WriteOnly);
    output_stream << static_cast<quint16>(json_string.size());
    message.push_back(json_string);
    _client_socket->write(message, message.size());
}

QString Model::getPeerIp(){
    bool isIpv4;
    QHostAddress ipv4(_client_socket->peerAddress().toIPv4Address(&isIpv4));
    if(isIpv4)
        return ipv4.toString();
    else
        return _client_socket->peerAddress().toString();
}


void Model::peerLetUsPlay(QJsonObject json_obj){
    QJsonObject paylod;
    paylod["IP"] = getPeerIp();
}
