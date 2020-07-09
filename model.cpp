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
    _move_indicator.first = 0;
    _move_indicator.second = 0;
}

Model::~Model(){
    delete _view;
}

bool Model::run(){
    connect(_server_socket, SIGNAL(newConnection()), this, SLOT(someConnection()));
    quint16 port = SFcom::PORT_NUMBER;

    while(not(_server_socket->listen(QHostAddress::Any, port)) && port > 0){
        port--;
    }

    if(port){
        connect(_view, SIGNAL(commandToModel(QJsonObject)), this, SLOT(commandFromView(QJsonObject)));
        connect(this, SIGNAL(commandToView(QJsonObject)), _view, SLOT(commandFromModel(QJsonObject)));
        _view->setPortNumber(port);
        _view->show();
        return true;
    }

    return false;
}

void Model::commandFromView(QJsonObject json_obj){
    if(json_obj["command"] == SFcom::Commands::LETUSPLAY){
        viewLetUsPlay(json_obj);
    }else if(json_obj["command"] == SFcom::Commands::READY){
        viewReady(json_obj);
    }
}

void Model::viewLetUsPlay(QJsonObject json_obj){
    if(json_obj["status"] == SFcom::Status::REQUEST){
        if(_connection_status == SFcom::ConnectionType::NOCONN){
            QJsonObject payload = json_obj["payload"].toObject();
            _connection_status = SFcom::ConnectionType::OUTCOMINGCONN;
            _client_socket = new QTcpSocket(_server_socket);
            connectPeersHandlers();
            _client_socket->connectToHost(payload["IP"].toString(), payload["port"].toInt());
        }
    }else if(json_obj["status"] == SFcom::Status::NO){
        writeToPeer(json_obj);
        disconnectFromPeer(SFcom::createJsonCommand(SFcom::Commands::ERROR, SFcom::Status::CONNERROR));
    }else if(json_obj["status"] == SFcom::Status::OK){
        _game_phase = SFcom::GamePhase::PREPARATION;
        writeToPeer(json_obj);
    }
}

void Model::viewReady(QJsonObject obj){
    qDebug()<< "here";
    QUuid uuid = QUuid::createUuid();
    _move_indicator.first = uuid;
    obj["payload"] = QJsonObject{{"uuid", uuid.toString()}};
    writeToPeer(obj);
    readinessCheck();
}

void Model::readinessCheck(){
    qDebug()<<"here3";
    qDebug()<<_move_indicator;
    if(_move_indicator.first != 0 && _move_indicator.second != 0){
        qDebug()<< _move_indicator;
        _game_phase = SFcom::GamePhase::GAME;
        bool your_turn = (_move_indicator.first > _move_indicator.second);
        QJsonObject payload = QJsonObject{{"your_turn", your_turn}};
        commandToView(SFcom::createJsonCommand(SFcom::Commands::READY, SFcom::Status::OK, payload));
    }
}

void Model::connectPeersHandlers(){
    connect(_client_socket, SIGNAL(connected()), this, SLOT(connectedToPeer()));
    connect(_client_socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(peerConnectionError(QAbstractSocket::SocketError)));
    connect(_client_socket, SIGNAL(readyRead()), this, SLOT(commandFromPeer()));
}

void Model::someConnection(){
    QTcpSocket *temp = _server_socket->nextPendingConnection();
    if(_connection_status == SFcom::ConnectionType::NOCONN && temp){
        _client_socket = temp;
        _connection_status = SFcom::ConnectionType::INCOMINGCONN;
        connectPeersHandlers();
    }else{
        temp->disconnectFromHost();
        temp->deleteLater();
    }
}

void Model::connectedToPeer(){
    if(_connection_status == SFcom::ConnectionType::OUTCOMINGCONN){
        QJsonObject json_obj = SFcom::createJsonCommand(SFcom::Commands::LETUSPLAY, SFcom::Status::REQUEST);
        writeToPeer(json_obj);
    }
}

void Model::disconnectFromPeer(QJsonObject json_obj){
    if(_client_socket->isOpen()){
        _client_socket->flush();
        _client_socket->close();
    }
    _client_socket->disconnect();

    json_obj["payload"] = QJsonObject{{"phase", _game_phase}};
    _connection_status = SFcom::ConnectionType::NOCONN;
    _game_phase = SFcom::GamePhase::CONNECTION;
    emit commandToView(json_obj);
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

        if(json_doc.isNull()){
            disconnectFromPeer(SFcom::createJsonCommand(SFcom::Commands::ERROR, SFcom::Status::LOGICERROR));
        }else{
            analizePeerCommand(json_doc.object());
        }
    }
}

void Model::analizePeerCommand(QJsonObject json_obj){
    if(SFcom::checkCommandFormat(json_obj)){
        switch (json_obj["command"].toInt()){
            case SFcom::Commands::LETUSPLAY: peerLetUsPlay(json_obj); break;
        case SFcom::Commands::READY: peerReady(json_obj); break;
            default: peerLogicError(); break;
        }
    }else{
        peerLogicError();
    }
}

void Model::writeToPeer(QJsonObject json_obj){
    QByteArray message;
    QByteArray json_string = QJsonDocument(json_obj).toJson();
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
    json_obj["payload"] = QJsonObject{{"IP", getPeerIp()}};
    if(json_obj["status"] == SFcom::Status::OK){
        _game_phase = SFcom::GamePhase::PREPARATION;
    }else if(json_obj["status"] != SFcom::Status::REQUEST && json_obj["status"] != SFcom::Status::NO){
        disconnectFromPeer(SFcom::createJsonCommand(SFcom::Commands::ERROR, SFcom::Status::LOGICERROR));
    }
    emit commandToView(json_obj);
}

void Model::peerReady(QJsonObject obj){
    qDebug()<<"here2";
    qDebug()<< obj;
    _move_indicator.second = QUuid::fromString(obj["payload"].toObject()["uuid"].toString());
    readinessCheck();
}
