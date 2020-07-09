#ifndef MODEL_H
#define MODEL_H

#include "view.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QUuid>

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
    void readinessCheck();
    QString getPeerIp();

    void viewLetUsPlay(QJsonObject);
    void viewReady(QJsonObject);

    void peerLetUsPlay(QJsonObject);
    void peerReady(QJsonObject);


    View *_view;
    QTcpServer *_server_socket;
    QTcpSocket *_client_socket;
    int _connection_status;
    int _game_phase;
    quint16 _next_block_size;
    QPair<QUuid, QUuid> _move_indicator;

};

#endif // MODEL_H
