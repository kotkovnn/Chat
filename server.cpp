#include "server.h"

Server::Server(int port, QObject* parent) : QObject(parent)
{
    m_server = new QTcpServer(this);
    if (!m_server->listen(QHostAddress::AnyIPv4,port))
    {
        std::cout << ">>> Server Error";
        return;
    }
    connect(m_server,SIGNAL(newConnection()),this,SLOT(createConnection()));

    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for (auto it : list)
        if (it.protocol() == QAbstractSocket::IPv4Protocol)
        {
            std::cout << ">>> Server address " << it.toString().toStdString() << ":" << port << std::endl;
            break;
        }
}

void Server::createConnection()
{
    QTcpSocket* socket = m_server->nextPendingConnection();
    connect(socket,SIGNAL(disconnected()),this,SLOT(deleteConnection()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    std::cout << ">>> New connection established" << std::endl;
    sockets.push_back(socket);
}

void Server::readMessage()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    QTextStream input(socket);
    QString str;
    input >> str;
    std::cout << str.toStdString() << std::endl;
}

void Server::sendMessage(const QString& message)
{
    for (auto socket : sockets)
    {
        QTextStream stream(socket);
        stream << message;
    }
}

void Server::deleteConnection()
{
    std::cout << ">>> client disconnected" << std::endl;
    int index = sockets.indexOf((QTcpSocket*)sender());
    sockets[index]->deleteLater();
    sockets.removeAt(index);
}
