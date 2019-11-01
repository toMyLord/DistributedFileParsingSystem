//
// Created by mylord on 2019/10/31.
//

#ifndef DISTRIBUTEDFILEPARSING_CLIENT_H
#define DISTRIBUTEDFILEPARSING_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "specifictime.h"
using namespace std;

#ifndef MAX_BUFFER_SIZE
#define MAX_BUFFER_SIZE 1024
#endif

class Client
{
private:
    int sock_fd;
    struct sockaddr_in client_addr;
    char server_ip[16];
    int server_port;

public:
    Client(char * server_ip, int server_port);

    int Connect();

    int Write(char * buff);

    int Read(char * buff);

    int Close();
};

#endif //DISTRIBUTEDFILEPARSING_CLIENT_H
