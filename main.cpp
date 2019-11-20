#include <QCoreApplication>
#include "server.h"
#include "client.h"
#include <iostream>
#include <QThread>
#include <QRegExp>
#include "chat.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Chat* chat = new Chat();
    chat->run();
    delete chat;

    return a.exec();
}
