//
// Created by mylord on 2019/10/31.
//

#ifndef DISTRIBUTEDFILEPARSING_RECORDING_LOG_H
#define DISTRIBUTEDFILEPARSING_RECORDING_LOG_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

//记录文件分析结果log的类，使用单例模式
class RecordingLog {
private:
    //阻止编译器自动生成copying操作
    RecordingLog(RecordingLog &) = delete;
    RecordingLog & operator=(const RecordingLog &) = delete;

    //必须为静态，类外会将其赋值nullptr
    static RecordingLog * log_ptr;

    ofstream out_file;

public:
    RecordingLog();

    //懒汉单例模式,设置为内联以提高效率
    static inline RecordingLog * getLogInstance() {
        if(log_ptr == nullptr){
            log_ptr = new RecordingLog();
        }
        return log_ptr;
    }

    void WriteLog(const string & log);
};


#endif //DISTRIBUTEDFILEPARSING_RECORDING_LOG_H
