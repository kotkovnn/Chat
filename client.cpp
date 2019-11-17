#include "client.h"

Client::Client(const QString& host, int port, QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    socket->connectToHost(host,port);
    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(err(QAbstractSocket::SocketError)));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readMessage()));
}

void Client::connected()
{
    std::cout << "Connected to server" << std::endl;
}

void Client::readMessage()
{
    QTextStream input(socket);
    QString message;
    input >> message;
    std::cout << message.toStdString() << std::endl;
}

void Client::sendMessage(const QString& message)
{
    QTextStream stream(socket);
    stream << message;
}

void Client::err(QAbstractSocket::SocketError error)
{
    if (error == QAbstractSocket::HostNotFoundError)
        std::cout << "Host was not found";
    else if (error == QAbstractSocket::RemoteHostClosedError)
        std::cout << "Remote host is closed";
    else if (error == QAbstractSocket::ConnectionRefusedError)
        std::cout << "Connection was refused";
    std::cout << std::endl;
}

Client::~Client()
{
    // TODO
}
