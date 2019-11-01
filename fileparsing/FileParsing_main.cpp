//
// Created by mylord on 2019/10/31.
//

#include <iostream>
#include "src/file_parsing.h"

int main() {
    FileParsing file_parsing("127.0.0.1");

    file_parsing.setState(new WatingState);

    while (true) {
        file_parsing.Handler();
    }
}