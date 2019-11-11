//
// Created by mylord on 2019/10/31.
//
#include "src/distribution_server.h"
#include <sys/epoll.h>
#include <queue>
#include <mutex>

typedef struct {
    int parsing_fd;
}ReadyParsingNode;

DistributionServer workstation_server(5555, 10);
DistributionServer parsing_server(6666, 10);
Server manager_server(7777, 1);
bool is_manager_connected = false;              //记录管理平台是否连接
int manager_fd;                                 //管理平台客户端的fd
queue<ReadyParsingNode> ready_parsing_queue;    //需要加互斥锁
mutex queue_mutex;                              //给就绪队列加的互斥锁
struct epoll_event fp_ev, fp_events[10];        //声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
struct epoll_event ws_ev, ws_events[10];        //声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
int fp_epoll_fd, ws_epoll_fd;
SpecificTime st;


void SendFpInfo();

void SendWsInfo();

void FpHandler_t();

void WsHandler_t();

void ParseRequestHandler_t(const int workstation_fd);

int main() {
    struct epoll_event ev, events[3];       //声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件
    int listen_epoll_fd = epoll_create(3);     //创建一个epoll的句柄，并告诉内核这个监听的数目为3
    int nfds;                               //记录需要处理的事件数
    ClientInfo manager_info;                //管理平台客户端的信息
    bool isFpThreaded = false;              //标记接受心跳线程是否建立
    bool isWsThreaded = false;              //标记接受心跳线程是否建立

    fp_epoll_fd = epoll_create(10);
    ws_epoll_fd = epoll_create(10);

    ev.events = EPOLLIN | EPOLLET;          //linsten_fd可读，边缘触发
    fp_ev.events = EPOLLIN;                 //linsten_fd可读，水平触发
    ws_ev.events = EPOLLIN;                 //linsten_fd可读，水平触发

    //将用于接受工作站信息的连接监听fd加入epoll池中
    ev.data.fd = workstation_server.getListenFd();
    epoll_ctl(listen_epoll_fd, EPOLL_CTL_ADD, workstation_server.getListenFd(), &ev);
    cout << "[" << st.getTime().c_str() << " Listening WorkStation]:\tListen to WorkStation at port 5555." << endl;

    //将用于接受文件解析服务器信息的连接监听fd加入epoll池中
    ev.data.fd = parsing_server.getListenFd();
    epoll_ctl(listen_epoll_fd, EPOLL_CTL_ADD, parsing_server.getListenFd(), &ev);
    cout << "[" << st.getTime().c_str() << " Listening Parser]:\tListen to Parser at port 6666." << endl;

    //将用于接受管理平台的连接监听fd加入epoll池中
    ev.data.fd = manager_server.getListenFd();
    epoll_ctl(listen_epoll_fd, EPOLL_CTL_ADD, manager_server.getListenFd(), &ev);
    cout << "[" << st.getTime().c_str() << " Listening Manager]:\tListen to Manager at port 7777." << endl;

    while(true) {
        nfds = epoll_wait(listen_epoll_fd, events, 10, -1);
        for(int i = 0; i < nfds; i++){
            if(events[i].data.fd == workstation_server.getListenFd()) {
                //有工作站连接至任务分配服务器
                //开线程监视一个完整的文件解析过程，接收来自工作站的解析请求以及接收来自文件解析服务器的解析完成回应都需要用阻塞的read
                ClientNode workstation_node;
                int workstation_fd = workstation_server.AcceptConnection(workstation_node);

                //将新连接的工作站加入epoll池
                ws_ev.data.fd =  workstation_fd;
                epoll_ctl(ws_epoll_fd, EPOLL_CTL_ADD, workstation_fd, &ws_ev);

                if (isWsThreaded == false) {
                    isWsThreaded = true;
                    thread ws_t(WsHandler_t);
                    ws_t.detach();
                }

                cout << "[" << st.getTime().c_str() << " Connected WorkStation]:\tWorkstation "
                    << workstation_node.client_info.client_ip << " is connected." << endl;


                if(is_manager_connected == true) {
                    //通知管理平台有新上线的工作站.信息在workstation_node中
                    SendWsInfo();
                }
            } else if(events[i].data.fd == parsing_server.getListenFd()) {
                //有文件解析服务器连接至任务分配服务器
                ClientNode parsing_node;
                int parsing_fd = parsing_server.AcceptConnection(parsing_node);

                //当有文件解析服务器加入任务分配服务器时，直接将其加入就绪队列
                ReadyParsingNode temp;
                temp.parsing_fd = parsing_fd;

                queue_mutex.lock();
                ready_parsing_queue.push(temp);
                queue_mutex.unlock();

                //将新连接的文件解析服务器加入epoll池
                fp_ev.data.fd =  parsing_fd;
                epoll_ctl(fp_epoll_fd, EPOLL_CTL_ADD, parsing_fd, &fp_ev);

                if (isFpThreaded == false) {
                    isFpThreaded = true;
                    thread fp_t(FpHandler_t);
                    fp_t.detach();
                }

                cout << "[" << st.getTime().c_str() << " Connected Parser]:\tParser "
                     << parsing_node.client_info.client_ip << " is connected and added to queue." << endl;

                if(is_manager_connected == true) {
                    //通知管理平台有新上线的文件解析服务器,信息在parsing_node中
                   SendFpInfo();
                }
            } else if(events[i].data.fd == manager_server.getListenFd()) {
                //有文件管理平台连接至任务分配服务器
                //谨记：管理平台只能连接一个!!!
                if(is_manager_connected == false) {
                    //如果管理平台还未连接，则连接管理平台
                    manager_fd = manager_server.AcceptConnection(manager_info);

                    is_manager_connected = true;

                    cout << "[" << st.getTime().c_str() << " Connected Manager]:\tManager "
                         << manager_info.client_ip << " is connected to Task Distributer." << endl;

                    //连接后需要向管理平台发送已经上线的所有工作站及文件解析服务器的信息
                    SendFpInfo();
                    SendWsInfo();
                }

            } else {
                time_t curtime;
                time(&curtime);
                cout << "[Epoll Error]:" << ctime(&curtime) << " no corresponding Epoll event found" << endl;
            }
        }
    }
}


void SendFpInfo() {
    char sender[1024];
    string buff("fp:");
    parsing_server.getAllClientInfo(buff);
    strcpy(sender, buff.c_str());
    manager_server.Write(manager_fd, sender);
}


void SendWsInfo() {
    char sender[1024];
    string buff("ws:");
    workstation_server.getAllClientInfo(buff);
    strcpy(sender, buff.c_str());
    manager_server.Write(manager_fd, sender);
}


void FpHandler_t() {
    char buffer[MAX_BUFFER_SIZE];

    while(true) {
        int nfds = epoll_wait(fp_epoll_fd, fp_events, 10, -1);
        for (int i = 0; i < nfds; i++) {
            if (parsing_server.Read(fp_events[i].data.fd, buffer) == 0) {
                //收到close请求
                parsing_server.Close(fp_events[i].data.fd);

                //从epoll池中删除该fd
                fp_ev.data.fd = fp_events[i].data.fd;
                epoll_ctl(fp_epoll_fd, EPOLL_CTL_DEL, fp_events[i].data.fd, &fp_ev);
            }
            if (strncmp(buffer, "HeartBeats", 10) == 0) {
                //收到心跳包

            } else if (strncmp(buffer, "ParsingSuccess", 14) == 0) {
                //收到成功解析回执
                ReadyParsingNode temp;

                temp.parsing_fd = fp_events[i].data.fd;

                queue_mutex.lock();
                ready_parsing_queue.push(temp);
                queue_mutex.unlock();

                parsing_server.setState(fp_events[i].data.fd, WAITING);

                cout << "[" << st.getTime().c_str() <<
                     " Parsing Success]:\tParse file succeed!" << endl;

                //向管理平台发送解析完成信息
                SendFpInfo();
            } else {
                //无法解析接收内容
                cout << "[" << st.getTime().c_str() <<
                     " Request Warning]:\tTask distribution server cann't parse the request from workstation!" << endl;
            }
        }
    }
}


void WsHandler_t() {
    char buffer[MAX_BUFFER_SIZE];

    while(true) {
        int nfds = epoll_wait(ws_epoll_fd, ws_events, 10, -1);
        for (int i = 0; i < nfds; i++) {
            if (workstation_server.Read(ws_events[i].data.fd, buffer) == 0) {
                //收到close请求
                workstation_server.Close(ws_events[i].data.fd);

                //从epoll池中删除该fd
                ws_ev.data.fd = ws_events[i].data.fd;
                epoll_ctl(ws_epoll_fd, EPOLL_CTL_DEL, ws_events[i].data.fd, &ws_ev);
            }
            if (strncmp(buffer, "HeartBeats", 10) == 0) {
                //收到心跳包

            } else if (strncmp(buffer, "TransmitSuccess", 15) == 0) {
                //收到成功解析回执

                workstation_server.setState(ws_events[i].data.fd, WAITING);

                cout << "[" << st.getTime().c_str() <<
                     " Transmit Success]:\tWorkStation transmit files succeed!" << endl;

                SendWsInfo();
            } else if (strncmp(buffer, "ParsingRequest", 14) == 0) {
                //有来自工作站的解析请求。可能存在就绪队列空而导致长时间等待的情况因此要用线程的方式处理解析请求。
                int workstation_fd = ws_events[i].data.fd;
                thread parse_t(ParseRequestHandler_t, workstation_fd);
                parse_t.detach();
            } else {
                //无法解析接收内容
                cout << "[" << st.getTime().c_str() <<
                     " Request Warning]:\tTask distribution server cann't parse the request from workstation!" << endl;
            }
        }
    }
}


void ParseRequestHandler_t(const int workstation_fd) {
    char buffer[MAX_BUFFER_SIZE];

    //对发送来的数据进行解析并根据就绪队列向文件解析服务器发送连接命令，如果就绪队列为空，就等待1s
    cout << "[" << st.getTime().c_str() <<
         " Request Received]:\tParsing request was received, start distributing!" << endl;

    while(true) {
        queue_mutex.lock();
        if(ready_parsing_queue.empty() != 1)    //如果就绪队列不空，先不拿掉锁，等pop之后再取锁
            break;
        queue_mutex.unlock();
        sleep(1);
    }

    int parsing_fd = ready_parsing_queue.front().parsing_fd;
    ready_parsing_queue.pop();
    queue_mutex.unlock();

    ClientNode workstation_node = workstation_server.getClientNode(workstation_fd);
    //构造好接口，buffer内需要有工作站的ip信息,端口默认=8888
    int lenth = workstation_node.client_info.client_ip.length();
    workstation_node.client_info.client_ip.copy(buffer, lenth);
    buffer[lenth] = '\0';
    parsing_server.Write(parsing_fd, buffer);

    workstation_server.setState(workstation_fd, TRANSMITING);
    parsing_server.setState(parsing_fd, TRANSMITING);

    //向管理平台发送正在解析文件信息
    SendFpInfo();
    SendWsInfo();
}