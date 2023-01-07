//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef BASENODE_H
#define BASENODE_H
#include "abstractnode.h"
#include "CheatCard/core_global.h"
#include "rci/core/idb.h"
#include "rci/objects/icard.h"
#include "rci/objects/icontacts.h"
#include "rci/objects/isession.h"
#include "rci/objects/iuserscards.h"
#include <QSharedPointer>
#include <dbobject.h>
#include <QSslError>
#include <rci/objects/iuser.h>

#define USERREQUEST_TIMEOUT 3

inline void initCheatCardBaseResources() {
    Q_INIT_RESOURCE(CheatCard);
}

namespace RC {

namespace Interfaces{
class iAPI;
}

class CHEATCARD_CORE_EXPORT BaseNode: public QH::AbstractNode
{
    Q_OBJECT
public:
    BaseNode(const QSharedPointer<Interfaces::iDB> &dataBase);

    static QString libVersion();
    static void init();

    /**
     * @brief createNewContact This method create a new contact and new child user account of current user.
     * The child account's seccret key it is sha256 of the secreet key of the current user + random genesis number.;
     * @param anotherUser This is another childAccount
     * @param resultContact this is result contact object.
     * @return true if function crate all items successfull else false.
     */
    bool createContact(const QSharedPointer<Interfaces::iUser>& anotherUser,
                     QSharedPointer<Interfaces::iContacts>& resultContact);

    bool restoreAllData(const QByteArray &curentUserKey,
                        const QString& domain = "",
                        int port = DEFAULT_CHEAT_CARD_PORT_SSL);

    bool updateContactData(const QSharedPointer<Interfaces::iContacts> &contact,
                           const QByteArray& secreet,
                           bool removeRequest,
                           const QString& domain = "",
                           int port = DEFAULT_CHEAT_CARD_PORT_SSL);

    bool restoreOneCard(unsigned int cardId,
                        const QString& domain = "",
                        int port = DEFAULT_CHEAT_CARD_PORT_SSL);

    QString getServerHost() const;

    const QSharedPointer<Interfaces::iDB>& db() const;

    const QSharedPointer<Interfaces::iUser> &currentUser() const;
    void setCurrentUser(QSharedPointer<Interfaces::iUser> newCurrentUser);

    bool fNetAvailable() const;
    void checkNetworkConnection(const QString& domain = "",
                                int port = DEFAULT_CHEAT_CARD_PORT_SSL);

signals:
    void sigPurchaseWasSuccessful(QSharedPointer<RC::Interfaces::iUsersCards> data, bool alert);
    void sigCardReceived(QSharedPointer<RC::Interfaces::iCard> card);
    void sigNetworkError(QAbstractSocket::SocketError errorCode,
                         QSslError::SslError sslError);
    void sigAvailableNetworkChanged(bool);
    void sigDataExchangingChanged(bool);

    void sigVersionNoLongerSupport(int minimumRequiredVersion);
    void sigSessionStatusResult(QSharedPointer<RC::Interfaces::iSession>, bool succesed);
    void sigContactsStatusResult(QSharedPointer<RC::Interfaces::iContacts>, bool succesed, bool removed);
    void sigContactsListChanged();

protected:

    /**
     * @brief getApi This method return current api of the node.
     * @param node This is node for that will choose api object.
     * @return api object.
     */

    QSharedPointer<Interfaces::iAPI> getApi(QH::AbstractNodeInfo *node);

    QH::AbstractNodeInfo *createNodeInfo(QAbstractSocket *socket,
                                         const QH::HostAddress *clientAddress) const override;

    void nodeConnected(QH::AbstractNodeInfo *node) override;
    void nodeDisconnected(QH::AbstractNodeInfo *node) override;

    void addNodeFailed(QH::AddNodeError error) override;
    void nodeAddedSucessful(QH::AbstractNodeInfo *node) override;

    // AbstractNode interface
protected slots:
    void nodeErrorOccured(QH::AbstractNodeInfo *nodeInfo,
                          QAbstractSocket::SocketError errorCode,
                          QString errorString) override;

    void handleSslErrorOcurred(QH::SslSocket *scket, const QSslError &error) override;

private:
    void setFNetAvailable(bool newFNetAvailable);

    QSharedPointer<Interfaces::iDB> _db = nullptr;
    QSharedPointer<Interfaces::iUser> _currentUser;
    bool _fNetAvailable = false;
};
}
#endif // BASENODE_H
