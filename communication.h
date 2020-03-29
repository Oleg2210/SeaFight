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
    enum Status{REQUEST, OK, NO, CONNERROR, LOGICERROR};
    enum Commands{LETUSPLAY, STRIKE, ERROR};
    const QList<QString> command_keys = {"command", "status", "payload"};

    inline QJsonDocument createJsonCommand(int command_name, int status, QJsonObject payload = QJsonObject()){
        QJsonDocument json_doc;
        QJsonObject json_obj;
        json_obj["command"] = command_name;
        json_obj["status"] = status;
        json_obj["payload"] = payload;
        json_doc.setObject(json_obj);
        return json_doc;
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
