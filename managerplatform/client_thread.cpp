#include "client_thread.h"

ClientThread::ClientThread(char * ip,int port, QWidget *parent) : manager_client(ip, port)
{
    while(this->manager_client.Connect())
    {
        qDebug("Waiting for task distribution server!\n");
        sleep(2);
    }
    qDebug("connected to distribution server!\n");
}

void ClientThread::run()
{
    while(true)
    {
        char buffer[MAX_BUFFER_SIZE];
        if(manager_client.Read(buffer) <= 0)
        {
            manager_client.Close();
            break;
        }
        qDebug("READ:\t%s", buffer);
        QString info(buffer);
        emit Signal_t(info);
    }
}

ClientThread::~ClientThread()
{
    this->manager_client.Close();
}
