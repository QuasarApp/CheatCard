//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef APIV0_H
#define APIV0_H

#include "api0/session.h"
#include "datapack.h"
#include <api/apibase.h>
#include <accesstoken.h>

#define USERREQUEST_TIMEOUT 3


namespace RC {
namespace API {

namespace V0 {
class CardStatusRequest;
class CardDataRequest;
class UsersCards;
class Card;
}

class ApiV0: public API::APIBase
{
    Q_OBJECT

    struct ExtraNodeInfo {
        unsigned long long token;
    };
public:
    ApiV0(const QSharedPointer<Interfaces::iDB>& dataBase, QH::AbstractNode* mainNode);
    ~ApiV0() override;

    int version() const override;

    QH::ParserResult parsePackage(const QSharedPointer<QH::PKG::AbstractData> &pkg,
                                  const QH::Header &pkgHeader,
                                  QH::AbstractNodeInfo *sender) override;

    // using on visitor
    void sendCardStatusRequest(long long userSession, QH::AbstractNodeInfo *dist) override;

    // using on seller
    void sendSessions(const QHash<long long, QSharedPointer<Interfaces::iSession> > &sessions,
                      QH::AbstractNodeInfo *dist) override;
    void initSupportedCommands() override;

signals:

    void sigPurchaseWasSuccessful(QSharedPointer<RC::Interfaces::iUsersCards> userData, bool alert) override;
    void sigCardReceived(QSharedPointer<RC::Interfaces::iCard>) override;
    void sigSessionStatusResult(QSharedPointer<RC::Interfaces::iSession>, bool succesed) override;
    void sigContactsStatusResult(QSharedPointer<RC::Interfaces::iContacts> contact,
                                 bool succesed, bool removed) override;
    void sigContactsListChanged() override;

protected:

    bool processCardStatusRequest(const QSharedPointer<V0::CardStatusRequest> &message,
                                  const QH::AbstractNodeInfo *sender, const QH::Header&);

    bool processSession(const QSharedPointer<API::V0::Session> &message,
                        const QH::AbstractNodeInfo *sender, const QH::Header&);
    bool processCardStatus(const QSharedPointer<QH::PKG::DataPack<API::V0::UsersCards>> &cardStatuses,
                           const QH::AbstractNodeInfo *sender, const QH::Header&);
    bool applayPurchases(const QSharedPointer<API::V0::UsersCards> &dbCard,
                         const QH::AbstractNodeInfo *sender, bool alert = true);
    bool processCardRequest(const QSharedPointer<V0::CardDataRequest> &cardStatus,
                            const QH::AbstractNodeInfo *sender, const QH::Header&);
    bool processCardData(const QSharedPointer<QH::PKG::DataPack<API::V0::Card> > &cardrequest,
                         const QH::AbstractNodeInfo *sender, const QH::Header &);

    /**
     * @brief sessionProcessed invoke when session has been processed
     */
    virtual void sessionProcessed(unsigned long long sessionId);

private:

    /**
     * @brief initializeSupportedCommands This method initialize all supported commnad of api.
     */
    void initializeSupportedCommands();

    QHash<QH::HostAddress, ExtraNodeInfo> _extraData;

};
}
}
#endif // APIV0_H
