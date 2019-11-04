#include "dialog.h"
#include "client_thread.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    ClientThread client_t("127.0.0.1", 7777);
    QObject::connect(&client_t, SIGNAL(Signal_t(QString)), &w, SLOT(ClientSlot(QString)));
    client_t.start();
    w.show();
    return a.exec();
}
