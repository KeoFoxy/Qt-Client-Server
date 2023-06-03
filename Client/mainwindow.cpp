#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    nextBlockSize = 0;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_connectButton_clicked() {
    socket->connectToHost("26.155.213.206", 2323); // Производим подключение по локальному IP и порту, указанному в Сервере
} // Использован RADMIN VPN для локальной сети

void MainWindow::SendToServer(QString str) {
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0) << QTime::currentTime() << str; // Тк пока сообщение не записано, нельзя определить его размер полностью.
    out.device()->seek(0);                            // Записывает 0, выделяется 2 байта, след. часть сообщения будет записана с 17го бита
    socket->write(Data);                              // Первые 16 бит остаются зарезервированы под наше сообщение
    out << quint16(Data.size() - sizeof(quint16));    // Размер находим разностью)
    ui->sendLineEdit->clear();                        // Заходим в начало блока и считаем его размер, чтоб в дальнейшем передать это значение серверу
}

void MainWindow::slotReadyRead() {
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_15);
    if (in.status() == QDataStream::Ok) {
        for (;;) {
            if (nextBlockSize == 0) {
                if (socket->bytesAvailable() < 2) {
                    break;
                } else {
                    in >> nextBlockSize;
                }
                if (socket->bytesAvailable() < nextBlockSize) {
                    break;
                } else {
                    QString str;
                    QTime time;
                    in >> time >> str;
                    nextBlockSize = 0;
                    ui->textBrowser->append(time.toString() + " " + str);
                }
            }
        }
    } else {
        ui->textBrowser->append("Error");
    }
}

void MainWindow::on_sendButton_clicked() {
    SendToServer(ui->sendLineEdit->text());
}

void MainWindow::on_sendLineEdit_returnPressed() {
    SendToServer(ui->sendLineEdit->text());
}
