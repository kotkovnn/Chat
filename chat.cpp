#include "chat.h"

Chat::Chat(QObject *parent) : QObject(parent)
{

}

void Chat::run()
{
    auto printCommands = [](){
        std::cout << ">>> Type:\n";
        std::cout << ">>> \"help\" to show commands;\n";
        std::cout << ">>> \"server\" to start server;\n";
        std::cout << ">>> \"client\" connect to server;\n";
        std::cout << ">>> \"close\" to close any connections;\n";
        std::cout << ">>> \"exit\" to exit app;" << std::endl;
    };
    printCommands();

    while(1)
    {
        std::string input;
        std::cin >> input;
        if (input == "help")
        {
            printCommands();
        }
        else if (input == "server")
        {
            if (!tryDeleteConnections()) continue;
            chatReady = true;

            std::cout << ">>> Set port: \"XXXX\" or \"N\"(default)" << std::endl;
            std::string port;
            std::cin >> port;
            QRegExp expPort(QString("\\d{1,}"));
            std::cout<<">>> Starting server..." << std::endl;
            if (expPort.exactMatch(QString::fromStdString(port)))
                server = new Server(std::stoi(port));
            else
                server = new Server();

            pThread = new QThread();
            server->moveToThread(pThread);
            pThread->start();

            connect(pThread,SIGNAL(finished()),server,SLOT(deleteLater()));
            connect(this,SIGNAL(sendMessage(const QString&)),server,SLOT(sendMessage(const QString&)));
        }
        else if (input == "client")
        {
            if (!tryDeleteConnections()) continue;
            chatReady = true;

            QRegExp expIP(QString("(\\d{1,4}\\.){3}\\d{1,4}$"));
            QRegExp expPort(QString("(\\d{1,4}\\.){3}\\d{1,4}\\:\\d{1,}"));
            bool flag=false;
            std::string ip;
            while (!expIP.exactMatch(QString::fromStdString(ip)) && !expPort.exactMatch(QString::fromStdString(ip)))
            {
                if (flag) std::cout << ">>> Wrong Format\n";
                std::cout << ">>> Type ip: \"XXX.XXX.XXX.XXX\" or \"XXX.XXX.XXX.XXX:port\"" << std::endl;
                std::cin >> ip;
                flag=true;
            }

            std::cout << ">>> Starting client..." << std::endl;
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

            pThread = new QThread();
            client->moveToThread(pThread);
            pThread->start();

            connect(pThread,SIGNAL(finished()),client,SLOT(deleteLater()));
            connect(this,SIGNAL(sendMessage(const QString&)),client,SLOT(sendMessage(const QString&)));
        }
        else if (input == "close")
        {
            if (!tryDeleteConnections()) continue;
            std::cout << ">>> All connections are closed" << std::endl;
        }
        else if (input == "exit")
        {
            if (tryDeleteConnections()) break;
            else continue;
        }
        else if (chatReady)
        {
            emit sendMessage(QString::fromStdString(input));
        }
        else
        {
            std::cout << ">>> Unknown command" << std::endl;
        }

    }
}

bool Chat::tryDeleteConnections()
{
    if (server!=nullptr)
    {
        std::cout << ">>> Do you want to stop server? \"Y\" / \"N\"" << std::endl;
        std::string answer;
        std::cin >> answer;
        if (answer == "Y" || answer == "y")
        {
            killAllConnections();
            std::cout << ">>> Server stopped" << std::endl;
            return true;
        }
        else return false;
    }
    else if (client!=nullptr)
    {
        std::cout << ">>> Do you want disconnect from server? \"Y\" / \"N\"" << std::endl;
        std::string answer;
        std::cin >> answer;
        if (answer == "Y" || answer == "y")
        {
            killAllConnections();
            std::cout << ">>> You are disconnected" << std::endl;
            return true;
        }
        else return false;
    }
    return true;
}

void Chat::killAllConnections()
{
    if (server!=nullptr || client!=nullptr)
    {
        pThread->exit();
        pThread->wait();
        pThread = nullptr;
        server = nullptr;
        client = nullptr;
    }
    chatReady = false;
}

Chat::~Chat()
{
    killAllConnections();
}
