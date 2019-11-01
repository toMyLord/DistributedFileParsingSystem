//
// Created by mylord on 2019/10/31.
//

#include "recording_log.h"

RecordingLog * RecordingLog::log_ptr;

inline RecordingLog * RecordingLog::getLogInstance() {
    if(log_ptr == nullptr){
//        log_ptr = new RecordingLog();
    }
    return log_ptr;
}