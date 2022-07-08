#include "languagesmodel.h"

#include <QDir>
#include <QMap>
#include <quasarapp.h>
#include <QQmlEngine>
#include <QLocale>
#include <CheatCard/settingskeys.h>

namespace RC {

LanguagesModel::LanguagesModel() {

    auto addLang = [this](const QString& key, const QString& val) {
        _languagesMap[key] = val;
        _reverceLanguagesMap[val] = key;
    };

    // We use empty string for auto because if code not exits will use system language
    addLang("",   "Auto");
    addLang("en", "English");
    addLang("ru", "Русский");
    addLang("pl", "Polski");
    addLang("uk", "Ukrainian");
    addLang("be", "Беларуский");

}

int LanguagesModel::rowCount(const QModelIndex &) const {
    return _languagesMap.size();
}

QVariant LanguagesModel::data(const QModelIndex &index, int role) const {

    int row = index.row();
    if ( row >= rowCount({})) {
        return false;
    }

    switch (role) {
    case Qt::DisplayRole: {

        auto item = std::next(_languagesMap.begin(), row);
        if (item == _languagesMap.end()) {
            return {};
        }

        return item.value();

        break;
    }
    }

    return {};
}

QHash<int, QByteArray> LanguagesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "modelData";

    return roles;
}

void LanguagesModel::selectLanguagge(const QString &lang, QObject* gui) {

    QString code = _reverceLanguagesMap.value(lang, "");

    if (_languagesMap.contains(lang))
        code = lang;

    QLocale locale = QLocale(code);
    if (code.isEmpty() || lang == "Auto") {
        code = QLocale::system().bcp47Name();
        locale = QLocale::system();
    }

    QuasarAppUtils::Locales::setLocale(locale);

    if (auto engine = qmlEngine(gui)) {
        engine->retranslate();
    }

    emit currentLanguageChanged();

    auto settings = QuasarAppUtils::ISettings::instance();
    settings->setValue(P_CURRENT_LANG, code);
}

QString LanguagesModel::getCurrentLanguage() {
    QLocale locate = QuasarAppUtils::Locales::currentLocate();
    auto code = locate.bcp47Name();
    return _languagesMap.value(code, code);
}

}
