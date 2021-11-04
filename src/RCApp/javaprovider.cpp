//#
//# Copyright (C) 2021-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include "javaprovider.h"



#ifdef Q_OS_ANDROID
#include <QAndroidJniEnvironment>

JavaProvider *JavaProvider::instance() {
    static JavaProvider* instance = new JavaProvider();
    return instance;
}

JavaProvider::JavaProvider() {

    JNINativeMethod methods[] {
        {"purchaseReceived", "(Ljava/lang/String;Ljava/lang/String;)V", reinterpret_cast<void *>(purchaseReceived)},
    };

    QAndroidJniObject javaClass("com/quasarapp/androidtools/CppProvider");
    QAndroidJniEnvironment env;
    jclass objectClass = env->GetObjectClass(javaClass.object<jobject>());
    env->RegisterNatives(objectClass,
                         methods,
                         sizeof(methods) / sizeof(methods[0]));
    env->DeleteLocalRef(objectClass);

}

void JavaProvider::getPremium() const {
    // C++ code
    QAndroidJniObject::callStaticMethod<void>("com/quasarapp/androidtools/CppProvider",
                                              "getPremium",
                                              "()V");

}

void JavaProvider::initBilling() const {
    QAndroidJniObject::callStaticMethod<void>("com/quasarapp/androidtools/CppProvider",
                                              "initBilling",
                                              "()V");
}

void JavaProvider::purchaseReceived(JNIEnv *env, jobject thiz, jstring id, jstring token) {
    Q_UNUSED(thiz)

    emit JavaProvider::instance()->sigPurchase(env->GetStringUTFChars(id, 0),
                                               env->GetStringUTFChars(token, 0));
}

#endif
