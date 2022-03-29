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
    nextBlockSize = 0;
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
        for(;;)                                                  //Да-да, бесконечный цикл
        {                                                        //Для начала считываем размер блока
            if(nextBlockSize == 0)                               //Он нам не известен, следовательно равен 0
            {                                                    //Далее проверяем, что для чтения нам доступно не менее 2х байт
                qDebug() << "nextBlockSize = 0";
                if(socket->bytesAvailable() < 2)                 //Если условия не выполнены, то выходим из беск. цикла
                {
                    break;
                }
                else
                {
                    in >> nextBlockSize;
                    qDebug() << "nextBlockSize" << nextBlockSize;//Если же условие выполняется, то считываем размер блока
                }                                                //После того, как стал известен размер блока, начинается сравнение с количеством байт
            if(socket->bytesAvailable() <  nextBlockSize )       //которое пришло от сервера, если размер блока больше, то какие то данные еще не дошли
            {
                qDebug() << "Data is not full;";
                break;
            }
            else                                                 //Если же все впорядке, то создаем строку и записываем данные в нее
            {
                QString str;
                QTime time;
                in >> time >> str;
                qDebug() << str;
                nextBlockSize = 0;                               //Также необходимо обнулить размер блока для дальнейших данных
                SendToClient(str);
                break;
            }
            }
        }
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
    out << quint16(0) <<QTime::currentTime() << str;        //Тк пока сообщение не записано, нельзя определить его размер полностью.
    out.device()->seek(0);                                  //Записывает 0, выделяется 2 байта, след. часть сообщения будет записана с 17го бита
    out << quint16(Data.size() - sizeof(quint16));          //Первые 16 бит остаются зарезервированы под наше сообщение
                                                            //Размер находим разностью)
    for(int i = 0; i < Sockets.size(); i++)
    {
        Sockets[i]->write(Data);
    }
}
