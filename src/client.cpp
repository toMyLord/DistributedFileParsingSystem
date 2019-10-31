//
// Created by mylord on 2019/10/31.
//

#include "client.h"

Client::Client(char * server_ip, int server_port)
{
    strncpy(this->server_ip, server_ip, 16);
    this->server_port = server_port;

    struct hostent * host;
    if((host = gethostbyname(this->server_ip)) == NULL)
    {
        fprintf(stderr, "The host name %s is illegal.\n", server_ip);
        exit(1);
    }

    if((this->sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Init socket fd error!\n");
        exit(1);
    }

    bzero(&this->client_addr, sizeof(this->client_addr));
    this->client_addr.sin_family = AF_INET;
    this->client_addr.sin_port = htons(this->server_port);
    this->client_addr.sin_addr = *((struct in_addr *)host->h_addr);

}


int Client::Connect()
{
    int nbytes = connect(this->sock_fd, (struct sockaddr *)(&this->client_addr),
                         sizeof(struct sockaddr));

    return nbytes;
}


int Client::Write(char * buff)
{
    int nbytes = write(this->sock_fd, buff, strlen(buff));

    return nbytes;
}

int Client::Read(char * buff)
{
    int nbytes = read(this->sock_fd, buff, MAX_BUFFER_SIZE);
    buff[nbytes] = '\0';

    return nbytes;
}

int Client::Close()
{
    close(this->sock_fd);
    printf("fd-%d exit!\n", this->sock_fd);
}