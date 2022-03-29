#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QDataStream>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server();

    QTcpSocket *socket;

private:
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;                                                //данные, котороый перемещаются между сервером и клиентом

    void SendToClient(QString str);                                 //Функция передачи данных в клиент


public slots:
    void incomingConnection(qintptr socketDescriptor);              //Обработчик входящих соединений
    void slotReadyRead();                                           //Обработчик полученных от клиента сообщений
};

#endif // SERVER_H
