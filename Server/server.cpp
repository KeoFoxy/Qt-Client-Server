#include "server.h"

Server::Server()                                            //Проверка на включение сервера записанна в этом конструкторе
{
    if(this->listen(QHostAddress::Any, 2323))               //Запуск сервера //Аргументы: QHostAddress - любой адресс, порт 2323
    {
        qDebug() << "Server has been started";
    }
    else
    {
        qDebug() << "Error";
    }
}

void Server::incomingConnection(qintptr socketDescription)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescription);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Sockets.push_back(socket);
    qDebug() << "Connection: success" << socketDescription;
}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_15);
    if(in.status() == QDataStream::Ok)
    {
        qDebug() << "Status: ok";

        QString str;                                        //Созадем переменную и записываем туда информацию
        in >> str;
        qDebug() << str;
    }
    else
    {
        qDebug() << "Status: DataStream - Error";
    }
}

void Server::SendToClient(QString str)
{
    Data.clear();                                           //Очищаем перед отправкой
    QDataStream out(&Data, QIODevice::WriteOnly);           //Делаем только запись
    out.setVersion(QDataStream::Qt_5_15);
    out << str;
    socket->write(Data);
}
