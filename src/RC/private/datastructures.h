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
    unsigned int cardId;
    unsigned int purchasesCount;
};


struct UserHeader {
    unsigned int userId;
    unsigned char token[32];
};
}

#endif // DATASTRUCTURES_H
