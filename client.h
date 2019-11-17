#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <iostream>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(const QString& host, int port = 3445, QObject *parent = nullptr);
    virtual ~Client();

private:
    QTcpSocket* socket;

public slots:
    void connected();
    void readMessage();
    void err(QAbstractSocket::SocketError error);
    void sendMessage(const QString& message);
};

#endif // CLIENT_H
