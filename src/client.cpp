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
        SpecificTime st;
        fprintf(stderr, "[%s Host Error]:\tThe host name %s is illegal.\n", st.getTime().c_str(), server_ip);
        exit(1);
    }

    if((this->sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        SpecificTime st;
        fprintf(stderr, "[%s Socket Error]:\tInit socket fd error!\n", st.getTime().c_str());
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

char * Client::getServerIP() {
    return this->server_ip;
}

int Client::getServerPort() {
    return this->server_port;
}

int Client::Write(char * buff)
{
    int nbytes = 0;
    if ((nbytes = write(this->sock_fd, buff, strlen(buff))) == -1) {
        SpecificTime st;
        fprintf(stderr, "[%s Write Error]:\t%s\n", st.getTime().c_str(), strerror(errno));
    }

    return nbytes;
}

int Client::Read(char * buff)
{
    int nbytes = 0;
    if ((nbytes = read(this->sock_fd, buff, MAX_BUFFER_SIZE)) == -1) {
        SpecificTime st;
        fprintf(stderr, "[%s Read Error]:\t%s\n", st.getTime().c_str(), strerror(errno));
    } else
        buff[nbytes] = '\0';

    return nbytes;
}

int Client::Close() {
    close(this->sock_fd);
}