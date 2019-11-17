#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>
#include <QNetworkInterface>
#include <QDataStream>

class Server : public QObject
{
    Q_OBJECT
public:
    Server(int port=3445, QObject* parent = nullptr);
    virtual ~Server();

private:
    QTcpServer* m_server;
    QList<QTcpSocket*> sockets;
public slots:
    void createConnection();
    void readMessage();
    void sendMessage(const QString& message);
    void deleteConnection();
};

#endif // SERVER_H
