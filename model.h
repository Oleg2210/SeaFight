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
    inline void clearMoveIndicator(){
        _move_indicator.first = 0;
        _move_indicator.second = 0;
    }
    void connectPeersHandlers();
    void disconnectFromPeer(QJsonObject);
    void peerLogicError();
    void writeToPeer(QJsonObject);
    void analizePeerCommand(QJsonObject);
    void readinessCheck();
    QString getPeerIp();
    SFcom::Status strikeCheck(int cell_number);
    void updateField(int cell_number, SFcom::Status strike_status, bool my_field);
    void gameEndCheck();

    void viewLetUsPlay(QJsonObject);
    void viewReady(QJsonObject);
    void viewStrike(QJsonObject);

    void peerLetUsPlay(QJsonObject);
    void peerReady(QJsonObject);
    void peerStrike(QJsonObject);


    View *_view;
    QTcpServer *_server_socket;
    QTcpSocket *_client_socket;
    int _connection_status;
    int _game_phase;
    int _my_drown_ships;
    int _enemies_drown_ships;
    quint16 _next_block_size;
    QPair<QUuid, QUuid> _move_indicator;

};

#endif // MODEL_H
