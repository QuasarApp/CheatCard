//#
//# Copyright (C) 2021-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#ifndef MODELSSTORAGE_H
#define MODELSSTORAGE_H

#include <QHash>
#include <QSharedPointer>
#include <softdelete.h>
#include "params.h"
#include <QQmlEngine>
#include <rci/core/imodelsstorage.h>

namespace QH {
class SoftDelete;
}

namespace RC {

namespace Interfaces {
class iModel;
class iDB;
}

#define DEFAULT_CALL_BACK [](const auto&, const auto&){ return true;}

/**
 * @brief The ModelsStorage class is main storage class for all models.
 * @see ModelsStorage::contains
 * @see ModelsStorage::get
 * @see ModelsStorage::contains
 */
class ModelsStorage: public Interfaces::iModelsStorage, public QEnableSharedFromThis<ModelsStorage>
{
public:

    ModelsStorage(const QSharedPointer<Interfaces::iDB>& db);

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
     * @brief getRaw This method return raw pointer to the reqered model.
     * @tparam Model This is model type.
     * @return raw pointer to the reqered model.
     */
    template<typename Model>
    Model* getRaw() const {
        return static_cast<Model*>(get(typeid(Model).hash_code()).data());
    };

    QSharedPointer<Interfaces::iModel> get(int id) const override;

    /**
     * @brief addAndGet This method return shared pointer to the reqered model, if Model is not exists it try to initialize the default model object.
     * @param cb This is call back lamdba function.
     * @param args This arguments packs of the @a Model consturctor..
     * @tparam Model This is model type.
     * @tparam Args This is list of arguments needed for the @a Model object.
     * @return true if the model added successful else false.
     * @see ModelsStorage::contains
     * @see ModelsStorage::get
     * @see ModelsStorage::add
     */
    template<class Model, class... Args>
    QSharedPointer<Model> addAndGet(
        const std::function<bool(const QSharedPointer<Model>&, const ModelsStorage& )>& cb,
        Args &&...args) {
        if (!contains<Model>()) {
            return add<Model>(cb, std::forward<Args>(args)...);
        }

        return Interfaces::iModelsStorage::get<Model>();
    };

    /**
     * @brief addAndGetRaw This method return raw pointer to the reqered model, if Model is not exists it try to initialize the default model object.
     * @param cb This is call back lamdba function.
     * @param args This arguments packs of the @a Model consturctor..
     * @tparam Model This is model type.
     * @tparam Args This is list of arguments needed for the @a Model object.
     * @return true if the model added successful else false.
     * @see ModelsStorage::contains
     * @see ModelsStorage::get
     * @see ModelsStorage::add
     */
    template<class Model, class... Args>
    Model* addAndGetRaw(
        const std::function<bool(const QSharedPointer<Model>&, const ModelsStorage& )>& cb,
        Args &&...args) {
        if (!contains<Model>() && add<Model>(cb, std::forward<Args>(args)...)) {
            return getRaw<Model>();
        }

        return getRaw<Model>();
    };

    /**
     * @brief add This method create and add new model into storage.
     * @param cb This is call back lamdba function.
     * @param args This arguments packs of the @a Model consturctor..
     * @tparam Model This is model type.
     * @tparam Args This is list of arguments needed for the @a Model object.
     * @return true if the model added successful else false.
     * @see ModelsStorage::contains
     * @see ModelsStorage::get
     * @see ModelsStorage::addAndGet
     */
    template<class Model, class... Args>
    bool add(
        const std::function<bool(const QSharedPointer<Model>&, const ModelsStorage& )>& cb,
        Args &&...args
        ) {

        static_assert(std::is_base_of_v<Interfaces::iModel, Model> &&
                      "The Model should based on the Interfaces::iModel.");


        if (contains<Model>()) {

            auto message = QString("The Model Storage alredy contains model. model: %0").
                           arg(typeid(Model).name());
            QuasarAppUtils::Params::log(message, QuasarAppUtils::Error);

            return false;
        }

        QSharedPointer<Model> model = init<Model>(std::forward<Args>(args)...);

        _storage.insert(typeid(Model).hash_code(), model);

        return cb(model, *this);

    };

    /**
     * @brief init This is more simple method of the add, this method just do base initialization and return createed object.
     * @param args This arguments packs of the @a Model consturctor..
     * @tparam Model This is model type.
     * @tparam Args This is list of arguments needed for the @a Model object.
     * @return pointer to the added model if the model added successful else nullptr.
     */
    template<class Model, class... Args>
    QSharedPointer<Model> init( Args&&... args  ) {

        static_assert(std::is_base_of_v<Interfaces::iModel, Model> &&
                      "The Model should based on the Interfaces::iModel.");

        QSharedPointer<Model> model;
        if constexpr (std::is_base_of_v<QH::SoftDelete, Model>) {
            model.reset(new Model(std::forward<Args>(args)...), softRemove);
        } else {
            model.reset(new Model(std::forward<Args>(args)...));
        }

        if constexpr(std::is_base_of_v<QObject, Model>) {
            QQmlEngine::setObjectOwnership(model.data(),
                                           QQmlEngine::CppOwnership);
        }

        model->init(_db, sharedFromThis().staticCast<Interfaces::iModelsStorage>());
        return model;

    };

    /**
     * @brief contains This method check model in storage.
     * @return true if mode is alredy stored in the storage, else false.
     * @tparam Model This is model type.
     * @see ModelsStorage::get
     * @see ModelsStorage::add
     * @see ModelsStorage::addAndGet
     */
    template<typename Model>
    bool contains() const {
        return _storage.contains(typeid(Model).hash_code());
    };

    /**
     * @brief db This method return current db object.
     * @return pointer to the data base object.
     */
    QSharedPointer<Interfaces::iDB> db() const;

private:

    static void softRemove(QH::SoftDelete * obj);

    QHash<int, QSharedPointer<Interfaces::iModel>> _storage;
    QSharedPointer<Interfaces::iDB> _db;

};
}
#endif // MODELSSTORAGE_H
