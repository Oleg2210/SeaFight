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
    void commandFromView(QJsonDocument);

signals:
    void commandToView(QJsonDocument);

private slots:
    void someConnection();
    void connectedToPeer();
    void peerConnectionError(QAbstractSocket::SocketError);
    void messageFromPeer();

private:
    void viewLetUsPlay(QJsonDocument);

    void connectPeersHandlers();
    void peerLetUsPlay(QJsonDocument);

    View *_view;
    QTcpServer *_server_socket;
    QTcpSocket *_client_socket;
    int _connection_status;
    int _game_phase;
};

#endif // MODEL_H
