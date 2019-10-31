//
// Created by mylord on 2019/10/31.
//

#ifndef DISTRIBUTEDFILEPARSING_PARSING_STATE_H
#define DISTRIBUTEDFILEPARSING_PARSING_STATE_H

#include "state.h"
#include "recording_log.h"

//文件解析状态,在这种状态下会使用记录log的类RecordingLog
class ParsingState : public State {
private:

public:
    ~ParsingState();
};


#endif //DISTRIBUTEDFILEPARSING_PARSING_STATE_H
