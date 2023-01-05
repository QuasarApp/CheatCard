//#
//# Copyright (C) 2020-2023 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#



#ifndef CONTACTSTEST_H
#define CONTACTSTEST_H
#include "connectiontest.h"


/**
 * @brief The ContactsTest class This test check share access to for another accounts
 */
class ContactsTest: public ConnectionTest
{
public:
    ContactsTest();
    void test();

};

#endif // CONTACTSTEST_H
