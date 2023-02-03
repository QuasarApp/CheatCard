//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "api.h"

#include <apiv3.h>
#include <params.h>

namespace RC {

namespace API {

QHash<unsigned short, QSharedPointer<API::APIBase> >
init(const QVector<unsigned short> &neededVersions,
     const QSharedPointer<Interfaces::iDB> &dataBase,
     QH::AbstractNode* mainNode) {

    QHash<unsigned short, QSharedPointer<API::APIBase> > result;

    for (const unsigned short version : neededVersions) {
        switch (version) {
        case 3: {
            mainNode->addApiParser(result[version] = QSharedPointer<API::ApiV3>::create(dataBase, mainNode));
            break;
        }

        default: {
            break;
        }
        }
    }

    return result;
}
}

}

