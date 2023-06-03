#include "server.h"

Server::Server() {
    if (this->listen(QHostAddress::Any, 2323)) {
        qDebug() << "Server has been started";
    } else {
        qDebug() << "Error";
    }
    nextBlockSize = 0;
}

void Server::incomingConnection(qintptr socketDescription) {
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescription);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Sockets.push_back(socket);
    qDebug() << "Connection: success" << socketDescription;
}

void Server::slotReadyRead() {
    socket = (QTcpSocket *)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_15);
    if (in.status() == QDataStream::Ok) {
        for (;;) {
            if (nextBlockSize == 0) {
                qDebug() << "nextBlockSize = 0";
                if (socket->bytesAvailable() < 2) {
                    break;
                } else {
                    in >> nextBlockSize;
                    qDebug() << "nextBlockSize" << nextBlockSize;
                }
                if (socket->bytesAvailable() < nextBlockSize) {
                    qDebug() << "Data is not full;";
                    break;
                } else {
                    QString str;
                    QTime time;
                    in >> time >> str;
                    qDebug() << str;
                    nextBlockSize = 0;
                    SendToClient(str);
                    break;
                }
            }
        }
    } else {
        qDebug() << "Status: DataStream - Error";
    }
}

void Server::SendToClient(QString str) {
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));

    for (int i = 0; i < Sockets.size(); i++) {
        Sockets[i]->write(Data);
    }
}
