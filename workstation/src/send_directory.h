//
// Created by mylord on 2019/11/1.
//

#ifndef DISTRIBUTEDFILEPARSING_SEND_DIRECTORY_H
#define DISTRIBUTEDFILEPARSING_SEND_DIRECTORY_H

#include <dirent.h>
#include <string>
#include <fstream>
#include <string.h>
#include <sstream>
#include "../../src/server.h"
using namespace std;

class SendDirectory {
private:
    DIR * dp;
    struct dirent * dirp;
    string dir_path;
public:
    SendDirectory() = default;

    bool setDir(string path);

    bool sendFile(Server * s, int fd, const char *dname);

    bool sendDir(Server * s, int fd);

};


#endif //DISTRIBUTEDFILEPARSING_SEND_DIRECTORY_H
