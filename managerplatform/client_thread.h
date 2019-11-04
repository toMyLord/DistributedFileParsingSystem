#ifndef CLIENT_THREAD_H
#define CLIENT_THREAD_H
#include <QThread>
#include "../src/client.h"

class ClientThread : public QThread
{
    Q_OBJECT
public:
    explicit ClientThread(char * ip, int port, QWidget *parent = nullptr);
    ~ClientThread() override;

protected:
    void run() override;

private:
    Client manager_client;
    int manager_fd;

signals:
    void  Signal_t(QString);

public slots:
};

#endif // CLIENT_THREAD_H
