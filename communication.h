#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QtCore>

namespace SFcom{
    const quint16 PORT_NUMBER = 55444;
    enum ConnectionType{NOCONN,INCOMINGCONN,OUTCOMINGCONN};
};

#endif // COMMUNICATION_H
