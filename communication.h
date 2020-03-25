#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QtCore>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

namespace SFcom{
    const quint16 PORT_NUMBER = 55444;
    enum ConnectionType{NOCONN,INCOMINGCONN,OUTCOMINGCONN};
    enum Status{REQUEST,OK,NO};
    enum Commands{LETUSSPLAY,STRIKE};

    inline QJsonDocument createJsonCommand(int command_name, int status, QJsonObject payload){
        QJsonDocument json_doc;
        QJsonObject json_obj;
        json_obj["command"] = command_name;
        json_obj["status"] = status;
        json_obj["payload"] = payload;
        json_doc.setObject(json_obj);
        return json_doc;
    }
};

#endif // COMMUNICATION_H
