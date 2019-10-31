//
// Created by mylord on 2019/10/31.
//
#include "src/distribution_server.h"
#include <sys/epoll.h>

int main() {
    struct epoll_event ev, events[3];      //声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
    int epoll_fd = epoll_create(3);    //创建一个epoll的句柄，并告诉内核这个监听的数目为10
    int nfds;                               //记录需要处理的事件数

    ev.events = EPOLLIN | EPOLLET;          //linsten_fd可读，边缘触发

    //将用于接受工作站信息的连接监听fd加入epoll池中
    DistributionServer workstation_server(5555, 10);
    ev.data.fd = workstation_server.getListenFd();
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, workstation_server.getListenFd(), &ev);

    //将用于接受文件解析服务器信息的连接监听fd加入epoll池中
    DistributionServer parsing_server(6666, 10);
    ev.data.fd = parsing_server.getListenFd();
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, parsing_server.getListenFd(), &ev);

    //将用于接受管理平台的连接监听fd加入epoll池中
    Server manager_server(7777, 1);
    ev.data.fd = manager_server.getListenFd();
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, manager_server.getListenFd(), &ev);

    while(true) {
        nfds = epoll_wait(epoll_fd, events, 10, -1);
        for(int i = 0; i < nfds; i++){
            if(events[i].data.fd == workstation_server.getListenFd()) {
                //有工作站连接至任务分配服务器
                //开线程监视一个完整的文件解析过程，接收来自工作站的解析请求以及接收来自文件解析服务器的解析完成回应都需要用阻塞的read

            } else if(events[i].data.fd == parsing_server.getListenFd()) {
                //有文件解析服务器连接至任务分配服务器
                //当有文件解析服务器加入任务分配服务器时，直接将其加入就绪队列

            } else if(events[i].data.fd == manager_server.getListenFd()) {
                //有文件管理平台连接至任务分配服务器
                //谨记：管理平台只能连接一个!!!

            } else {
                time_t curtime;
                time(&curtime);
                cout << "[Epoll Error]:" << ctime(&curtime) << " no corresponding Epoll event found" << endl;
            }
        }
    }




}