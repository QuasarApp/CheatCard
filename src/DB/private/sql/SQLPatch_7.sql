-- changes on the database after refactoring

BEGIN TRANSACTION;

CREATE TABLE "Users_old" (
    "id"            INTEGER NOT NULL,
    "name"          TEXT NOT NULL,
    "time" INTEGER NOT NULL DEFAULT 0,
    secret TEXT DEFAULT NULL,
    key TEXT DEFAULT '',
    PRIMARY KEY("id")
);

INSERT INTO Users_old SELECT * FROM Users;
DROP TABLE Users;

CREATE TABLE "Users" (
    "key"           TEXT NOT NULL UNIQUE,
    "name"          TEXT NOT NULL DEFAULT "",
    "time"          INTEGER NOT NULL DEFAULT 0,
    "secret"        TEXT DEFAULT NULL,
    PRIMARY KEY("key")
);

INSERT INTO Users SELECT key,name,time,secret FROM Users_old;

CREATE TABLE IF NOT EXISTS "Cards_old" (
        "id"	INTEGER NOT NULL,
        "logo"	BLOB DEFAULT NULL,
        "seal"	BLOB DEFAULT NULL,
        "background"	BLOB DEFAULT NULL,
        "title"	TEXT,
        "phone"	TEXT,
        "telegramm"	TEXT,
        "instagramm"	TEXT,
        "physicalAddress"	TEXT,
        "webSite"	TEXT,
        "freeItemName"	TEXT NOT NULL,
        "color"	INTEGER DEFAULT 0x00777777,
        "fontColor"	INTEGER DEFAULT 0x00777777,
        "freeIndex"	INTEGER DEFAULT 0,
        "time"	INTEGER NOT NULL DEFAULT 0,
        "cardVersion"	INT DEFAULT 0,
        "ownerSignature"	VARCHAR(50) DEFAULT NULL,
        PRIMARY KEY("id")
);

INSERT INTO Cards_old SELECT * FROM Cards;
DROP TABLE Cards;

CREATE TABLE IF NOT EXISTS "Cards" (
        "id"	TEXT NOT NULL,
        "logo"	BLOB DEFAULT NULL,
        "seal"	BLOB DEFAULT NULL,
        "background"	BLOB DEFAULT NULL,
        "title"	TEXT,
        "phone"	TEXT,
        "telegramm"	TEXT,
        "instagramm"	TEXT,
        "physicalAddress"	TEXT,
        "webSite"	TEXT,
        "freeItemName"	TEXT NOT NULL,
        "color"	INTEGER DEFAULT 0x00777777,
        "fontColor"	INTEGER DEFAULT 0x00777777,
        "freeIndex"	INTEGER DEFAULT 0,
        "time"	INTEGER NOT NULL DEFAULT 0,
        "cardVersion"	INT DEFAULT 0,
        "ownerSignature"	VARCHAR(50) DEFAULT NULL,
        PRIMARY KEY("id")
);

CREATE TABLE IF NOT EXISTS "UsersData" (
        "user"	TEXT NOT NULL,
        "card"	TEXT NOT NULL,
        "purchasesNumber"	INTEGER DEFAULT 1,
        "received"	INTEGER DEFAULT 0,
        "time"	INTEGER NOT NULL DEFAULT 0,
        FOREIGN KEY("user") REFERENCES "Users"("key") ON UPDATE CASCADE ON DELETE CASCADE,
        FOREIGN KEY("card") REFERENCES "Cards"("id") ON UPDATE CASCADE ON DELETE CASCADE
);

DROP TABLE Config;
DROP TABLE NetworkMembers;
DROP TABLE MemberPermisions;
DROP TABLE DefaultPermissions;

COMMIT;
