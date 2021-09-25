//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the lgplv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#include "server.h"

#include <card.h>
#include <datastructures.h>
#include <userscards.h>

namespace RC {

Server::Server(QH::ISqlDBCache *db): BaseNode(db) {
    registerPackageType<UsersCards>();
    registerPackageType<CardDataRequest>();
    registerPackageType<Card>();
}

QH::ParserResult Server::parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                      const QH::Header &pkgHeader,
                                      const QH::AbstractNodeInfo *sender) {

    auto parentResult = AbstractNode::parsePackage(pkg, pkgHeader, sender);
    if (parentResult != QH::ParserResult::NotProcessed) {
        return parentResult;
    }

    auto result = commandHandler<CardStatusRequest>(&Server::processCardStatusRequest, pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<UsersCards>(&Server::processCardStatus, pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<CardDataRequest>(&Server::processCardRequest, pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    result = commandHandler<Card>(&Server::processCardData, pkg, sender, pkgHeader);
    if (result != QH::ParserResult::NotProcessed) {
        return result;
    }

    return QH::ParserResult::NotProcessed;
}



}
