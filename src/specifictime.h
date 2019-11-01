//
// Created by mylord on 2019/11/1.
//

#ifndef DISTRIBUTEDFILEPARSING_SPECIFICTIME_H
#define DISTRIBUTEDFILEPARSING_SPECIFICTIME_H

#include <sstream>
#include <string>
#include <time.h>

using namespace std;

class SpecificTime {
public:
    string getTime(){
        time_t now = time(0);
        tm * t = localtime(&now);


        stringstream time_stream;
        time_stream << t->tm_year + 1900 << "/" << t->tm_mon + 1 << "/"
            << t->tm_mday << " " << t->tm_hour << ":" << t->tm_min << ":"
            << t->tm_sec;

        return time_stream.str();
    }
};


#endif //DISTRIBUTEDFILEPARSING_SPECIFICTIME_H
