#include <QCoreApplication>
#include "server.h"
#include "client.h"
#include <iostream>
#include <QThread>
#include <QRegExp>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::string command;
    while(1)
    {
        std::cout << "Commands: \"server\" or \"client\"" << std::endl;
        std::cin >> command;
        if (command == "server" || command == "client") break;
        else std::cout << "Unknown command\n";
    }
    std::cout << std::endl;

    if (command=="server")
    {
        Server* server;
        std::cout << "Set port: \"XXXX\" or \"N\"(default)" << std::endl;
        std::string port;
        std::cin >> port;
        QRegExp expPort(QString("\\d{1,}"));
        std::cout<<"Starting server..." << std::endl;
        if (expPort.exactMatch(QString::fromStdString(port)))
            server = new Server(std::stoi(port));
        else
            server = new Server();

        QThread *pThread = new QThread;
        server->moveToThread(pThread);
        pThread->start();

        while(1)
        {
            std::string message;
            std::cin >> message;
            QString str = QString::fromStdString(message);
            QMetaObject::invokeMethod(server, "sendMessage", Qt::QueuedConnection, Q_ARG(QString,str));
        }
    }
    else if (command=="client")
    {
        QRegExp expIP(QString("(\\d{1,4}\\.){3}\\d{1,4}$"));
        QRegExp expPort(QString("(\\d{1,4}\\.){3}\\d{1,4}\\:\\d{1,}"));
        bool flag=false;
        std::string ip;
        while (!expIP.exactMatch(QString::fromStdString(ip)) && !expPort.exactMatch(QString::fromStdString(ip)))
        {
            if (flag) std::cout << "\nWrong Format\n";
            std::cout << "Type ip: \"XXX.XXX.XXX.XXX\" or \"XXX.XXX.XXX.XXX:port\"" << std::endl;
            std::cin >> ip;
            flag=true;
        }

        std::cout << "Starting client..." << std::endl;
        Client *client;
        if (expIP.exactMatch(QString::fromStdString(ip)))
            client = new Client(QString::fromStdString(ip));
        else
        {
            QRegExp exp("((\\d{1,4}\\.){3}\\d{1,4})(\\:)(\\d{1,})");
            int pos = exp.indexIn(QString::fromStdString(ip));
            if (pos>-1)
                client = new Client(exp.cap(1),exp.cap(4).toInt());
            else
                exit(1);
        }

        QThread *pThread = new QThread;
        client->moveToThread(pThread);
        pThread->start();

        while(1)
        {
            std::string message;
            std::cin >> message;
            QString str = QString::fromStdString(message);
            QMetaObject::invokeMethod(client, "sendMessage", Qt::QueuedConnection, Q_ARG(QString,str));
        }
    }

    return a.exec();
}
