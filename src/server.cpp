//
// Created by mylord on 2019/10/31.
//

#include "server.h"

Server::Server(int port, int size): listen_port(port), listen_size(size) {
    this->Init();
    this->Listen();
}

void Server::Init() {
    if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {   //服务器端开始建立socket描述符
        SpecificTime st;
        fprintf(stderr, "[%s Socket Error]:\t%s \n\a", st.getTime().c_str(), strerror(errno));
        exit(1);
    }
    //服务器端填充tcp sockaddr结构
    bzero(&server_addr, sizeof(struct sockaddr_in));    //先将套接字地址数据结构清零
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(listen_port);

    if(bind(listen_fd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)
    {
        SpecificTime st;
        fprintf(stderr, "[%s Bind Error]:\t%s\n\a", st.getTime().c_str(), strerror(errno));
        exit(1);
    }
}

void Server::Listen() {
    if(listen(listen_fd, listen_size) == -1)
    {   //端口绑定成功，监听socketfd描述符，同时处理的最大连接请求数为10
        SpecificTime st;
        fprintf(stderr, "[%s Listen Error]:\t%s\n\a", st.getTime().c_str(), strerror(errno));
        exit(1);
    }
}

int Server::getListenFd() {
    return this->listen_fd;
}

int Server::AcceptConnection(ClientInfo & client_info) {
    int sockaddr_size = sizeof(struct sockaddr_in);

    if ((client_info.client_fd = accept(listen_fd, (struct sockaddr *)(&client_info.client_sock),
                                 (socklen_t *) &sockaddr_size)) == -1)
    {   //调用accept接受一个连接请求
        SpecificTime st;
        fprintf(stderr, "[%s Accept error]:\t%s\n\a", st.getTime().c_str(), strerror(errno));
        exit(1);
    }

    client_info.client_ip.assign(inet_ntoa(client_info.client_sock.sin_addr));

    return client_info.client_fd;
}

int Server::Write(int sock_fd, char buff[]) {
    int nbytes = 0;
    if((nbytes = write(sock_fd, buff, strlen(buff))) == -1) {
        SpecificTime st;
        fprintf(stderr, "[%s Write Error]:\t%s\n", st.getTime().c_str(), strerror(errno));
    }
    return nbytes;
}

int Server::Read(int sock_fd, char buff[]) {
    int nbytes = 0;
    if ((nbytes = read(sock_fd, buff, MAX_BUFFER_SIZE)) == -1) {
        SpecificTime st;
        fprintf(stderr, "[%s Read Error]:\t%s\n", st.getTime().c_str(), strerror(errno));
    } else
        buff[nbytes] = '\0';

    return nbytes;
}

int Server::Close(int client_fd) {
    close(client_fd);
}