//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef ICARDLISTMODEL_H
#define ICARDLISTMODEL_H

namespace RC {

class iCardListModel
{
public:
    iCardListModel();

    virtual void removeCard(int cardId) = 0;
    virtual void activateCard(int cardId) = 0;
    virtual void activateCardByIndex(int index) = 0;
};
}
#endif // ICARDLISTMODEL_H
