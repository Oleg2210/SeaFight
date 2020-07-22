#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QtCore>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

namespace SFcom{
    const quint16 PORT_NUMBER = 55443;
    enum ConnectionType{NOCONN, INCOMINGCONN, OUTCOMINGCONN};
    enum GamePhase{CONNECTION, PREPARATION, GAME};
    enum Status{REQUEST, OK, NO, CONNERROR, LOGICERROR, MISS, WOUND, DROWN};
    enum Commands{LETUSPLAY, STRIKE, ERROR, READY};
    const QList<QString> command_keys = {"command", "status", "payload"};

    inline QJsonObject createJsonCommand(int command_name, int status, QJsonObject payload = QJsonObject()){
        return QJsonObject{{"command", command_name}, {"status", status}, {"payload", payload}};
    }

    inline bool checkCommandFormat(const QJsonObject &json_obj){
        foreach (QString key, command_keys) {
            if(!json_obj.contains(key))
                return false;
        }
        return true;
    }

    inline bool checkLetUsPlayFormat(const QJsonObject &json_obj){
        int status = json_obj["status"].toInt();
        if(status == Status::REQUEST || status == Status::NO || status == Status::OK)
            return true;
        return false;
    }
};

#endif // COMMUNICATION_H
