//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#


#ifndef IMODELSTORAGE_H
#define IMODELSTORAGE_H
#include <rci/global.h>

#include <QSharedPointer>


namespace RC {
namespace Interfaces {

class iModel;

/**
 * @brief The iModelsStorage class is interface of the models Storage container.
 */
class CHEATCARD_INTERFACES_EXPORT iModelsStorage
{
public:

    /**
     * @brief get This method return shared pointer to the reqered model.
     * @tparam Model This is model type.
     * @return shared pointer to the reqered model.
     */
    template<typename Model>
    QSharedPointer<Model> get() const {
        return get(typeid(Model).hash_code()).staticCast<Model>();
    };

    /**
     * @brief get This method return shared pointer to the reqered model.
     * @tparam Model This is model type.
     * @return shared pointer to the reqered model.
     */
    virtual QSharedPointer<iModel> get(int modelId) const = 0;

};

}

}
#endif // IMODELSTORAGE_H
