//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "server.h"

#include <datastructures.h>
#include <userscards.h>

namespace RC {

Server::Server(QH::ISqlDBCache *db): BaseNode(db) {
}

QH::ParserResult Server::parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                      const QH::Header &pkgHeader,
                                      const QH::AbstractNodeInfo *sender) {

    auto parentResult = AbstractNode::parsePackage(pkg, pkgHeader, sender);
    if (parentResult != QH::ParserResult::NotProcessed) {
        return parentResult;
    }


    if (H_16<UsersCards>() == pkg->cmd()) {
        auto data = QSharedPointer<UsersCards>::create();
        data->fromBytes(pkg->toBytes());

        if (!data->isValid()) {
            return QH::ParserResult::Error;
        }

        if(!processCardStatus(data, sender)) {
            return QH::ParserResult::Error;
        }

        return QH::ParserResult::Processed;
    }

    if (H_16<CardDataRequest>() == pkg->cmd()) {
        auto data = QSharedPointer<CardDataRequest>::create();
        data->fromBytes(pkg->toBytes());

        if (!data->isValid()) {
            return QH::ParserResult::Error;
        }

        if(!processCardStatus(data, sender)) {
            return QH::ParserResult::Error;
        }

        return QH::ParserResult::Processed;
    }

    return QH::ParserResult::NotProcessed;
}



}
