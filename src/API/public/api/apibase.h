//#
//# Copyright (C) 2022-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef APIBASE_H
#define APIBASE_H

#include <rci/core/iapi.h>
#include <rci/core/idb.h>
#include <iparser.h>
#include "abstractnode.h"
#include "global.h"

#define API_BASE_PARSE_IS "CheatCardBaseAPI"

namespace RC {

namespace API {

/**
 * @brief The APIBase class This is base class for all api objects.
 * This interface is combined the QH::iParser and RC::Interfaces::iAPI interfaces.
 */
class CHEATCARD_API_EXPORT APIBase: public QH::iParser, public RC::Interfaces::iAPI
{
    Q_OBJECT
public:

         /**
         * @brief The NodeType enum contains types of the nodes.
         */
        enum NodeType: int {
            // Base types (first 4 bits)

            /// This is unkonown node type (invalid node)
            Unknown = 0x00,

            /// This is client node that work in visitor mode (clients of the sellers)
            Visitor = 0x01,

            /// This is client node that work in seller mode
            Seller  = 0x02,

            /// This node work as a server.
            Server  = 0x03,

            };

    /**
     * @brief APIBase is main constructoe of the api object;
     * @param dataBase This is shared pointer of the main database connection.
     * @param mainNode this is raw pointer to the raw node object
     * @note @a mainNode will not distored after distroy api object.
     */
    APIBase(const QSharedPointer<Interfaces::iDB>& dataBase, QH::AbstractNode* mainNode);
    ~APIBase() = default;

    /**
     * @brief db This method return currnet db object
     * @return
     */
    QSharedPointer<Interfaces::iDB> db() const;

    // iParser interface
    QString parserId() const override final;

signals:
    void sigPurchaseWasSuccessful(QSharedPointer<RC::Interfaces::iUsersCards> userData,
                                  bool alert) override;
    void sigCardReceived(QSharedPointer<RC::Interfaces::iCard>) override;
    void sigContactsListChanged() override;
    void sigUserDataRemoved(const QByteArray& cardId, const QByteArray& userId) override;

private:
    QSharedPointer<Interfaces::iDB> _db;

};

}
}
#endif // APIBASE_H
