//#
//# Copyright (C) 2021-2022 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef LANGUAGESMODEL_H
#define LANGUAGESMODEL_H

#include <QAbstractListModel>
#include <QMap>
#include "CheatCardGui/gui_global.h"

namespace RC {

class LanguagesModel: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString currentLanguage READ getCurrentLanguage NOTIFY currentLanguageChanged)

public:
    LanguagesModel();

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief selectLanguagge
     * @param lang language name or language code.
     * @param gui
     * @return selected language code.
     */
    Q_INVOKABLE void selectLanguagge(const QString& lang, QObject *gui);
    Q_INVOKABLE QString getCurrentLanguage();

private:

    QMap<QString, QString> _languagesMap;
    QMap<QString, QString> _reverceLanguagesMap;

signals:
    void currentLanguageChanged();
};

}

#endif // LANGUAGESMODEL_H
