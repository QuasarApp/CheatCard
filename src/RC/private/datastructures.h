//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H


namespace RC {

struct CardStatus {
    unsigned char command = 0;
    unsigned int cardId = 0;
    unsigned int purchasesCount = 0;
};


struct UserHeader {
    unsigned char command = 0;
    unsigned int userId = 0;
    char token[32];
};

struct DataRequest {
    unsigned char command = 0;
};

struct RawData {
    unsigned char command = 0;
    unsigned int size = 0;
    char *data;
};
}

#endif // DATASTRUCTURES_H
