//
// Created by mylord on 2019/10/31.
//

#include "recording_log.h"

RecordingLog * RecordingLog::log_ptr;

RecordingLog::RecordingLog() : out_file("./file_parse.log", ios::app){

}

void RecordingLog::WriteLog(const string & log) {
    out_file << log << endl;
}