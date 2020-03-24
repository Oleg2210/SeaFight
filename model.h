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

signals:

public slots:
private slots:
    void someConnection();

private:
    View *_view;
    QTcpServer *_server_socket;
    QTcpSocket *_client_socket;
    int _connection_status;
    int _game_phase;
};

#endif // MODEL_H
