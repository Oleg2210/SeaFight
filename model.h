#ifndef MODEL_H
#define MODEL_H

#include "view.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>

class Model : public QObject
{
    Q_OBJECT
public:
    Model(QObject *parent = nullptr);
    ~Model();
    bool run();

public slots:
    void commandFromView(QJsonObject);

signals:
    void commandToView(QJsonObject);

private slots:
    void someConnection();
    void connectedToPeer();
    void peerConnectionError(QAbstractSocket::SocketError);
    void commandFromPeer();

private:
    void connectPeersHandlers();
    void disconnectFromPeer(QJsonObject);
    void peerLogicError();
    void writeToPeer(QJsonObject);
    void analizePeerCommand(QJsonObject);
    QString getPeerIp();

    void viewLetUsPlay(QJsonObject);

    void peerLetUsPlay(QJsonObject);


    View *_view;
    QTcpServer *_server_socket;
    QTcpSocket *_client_socket;
    int _connection_status;
    int _game_phase;
    quint16 _next_block_size;

};

#endif // MODEL_H
