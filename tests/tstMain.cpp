//#
//# Copyright (C) 2020-2021 QuasarApp.
//# Distributed under the GPLv3 software license, see the accompanying
//# Everyone is permitted to copy and distribute verbatim copies
//# of this license document, but changing it is not allowed.
//#

#include <QtTest>
#include "connectiontest.h"
#include "restoredatatest.h"
#include "fixdbidspilltest.h"
#include "userscardstest.h"
#include "securitytest.h"
#include "contactstest.h"
#include "fixbrokenuserstest.h"
#include "apitest.h"

// Use This macros for initialize your own test classes.
// Check exampletests
#define TestCase(name, testClass) \
    void name() { \
        initTest(new testClass()); \
    }

/**
 * @brief The tstMain class - this is main test class
 */
class tstMain : public QObject
{
    Q_OBJECT


public:
    tstMain();

    ~tstMain();

private slots:


    // BEGIN TESTS CASES
    TestCase(apiTest, APITest)
    TestCase(usersCardsTest, UsersCardsTest)
    TestCase(connectionTest, ConnectionTest)
    TestCase(securityTest, SecurityTest)
    TestCase(fixDBIdsPillTest, FixDBIdsPillTest)
    TestCase(restoreDataTest, RestoreDataTest)
    TestCase(contactsTest, ContactsTest)
    TestCase(fixBrokenUsersTest, FixBrokenUsersTest)
    // END TEST CASES

private:

    /**
     * @brief initTest This method prepare @a test for run in the QApplication loop.
     * @param test are input test case class.
     */
    void initTest(Test* test);

    QCoreApplication *_app = nullptr;
};

/**
 * @brief tstMain::tstMain
 * init all availabel units for testsing
 */
tstMain::tstMain() {
    srand(time(0));

    // init xample unit test
    int argc =0;
    char * argv[] = {nullptr};

    _app = new QCoreApplication(argc, argv);
    QCoreApplication::setApplicationName("testCheatCard");
    QCoreApplication::setOrganizationName("QuasarApp");

    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir(path).removeRecursively();

}

tstMain::~tstMain() {
    _app->exit(0);
    delete _app;
}

void tstMain::initTest(Test *test) {
    QTimer::singleShot(0, this, [this, test]() {
        test->test();
        delete test;
        _app->exit(0);
    });

    _app->exec();
}

QTEST_APPLESS_MAIN(tstMain)

#include "tstMain.moc"
