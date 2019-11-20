#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <iostream>
#include "server.h"
#include "client.h"
#include <QThread>
#include <QRegExp>

class Chat : public QObject
{
    Q_OBJECT
public:
    explicit Chat(QObject *parent = nullptr);
    void run();
    virtual ~Chat();

private:
    bool chatReady = false;
    Server* server = nullptr;
    Client* client = nullptr;
    QThread* pThread = nullptr;
    void stopAllConnections();
    bool tryDeleteConnections();
    void killAllConnections();

signals:
    void sendMessage(const QString& message);
public slots:
};

#endif // CHAT_H
