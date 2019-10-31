//
// Created by mylord on 2019/10/31.
//

#ifndef DISTRIBUTEDFILEPARSING_STATE_H
#define DISTRIBUTEDFILEPARSING_STATE_H

//状态模式的基类
class State {
public:
    //析构函数设置成虚函数避免派生类在析构的时候只调用基类的析构函数造成不完全析构
    virtual ~State() = default;
};


#endif //DISTRIBUTEDFILEPARSING_STATE_H
