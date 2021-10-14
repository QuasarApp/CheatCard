BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "NetworkMembers" (
        "id"	VARCHAR(32),
        "authenticationData"	BLOB NOT NULL,
        "trust"	INTEGER DEFAULT 0,
        PRIMARY KEY("id")
);
CREATE TABLE IF NOT EXISTS "MemberPermisions" (
        "memberId"	VARCHAR(32) NOT NULL,
        "dbAddress"	VARCHAR(44) NOT NULL,
        "lvl"	tinyint NOT NULL,
        FOREIGN KEY("memberId") REFERENCES "NetworkMembers"("id") ON UPDATE CASCADE ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS "DefaultPermissions" (
        "dbAddress"	VARCHAR(44) NOT NULL UNIQUE,
        "lvl"	tinyint NOT NULL,
        FOREIGN KEY("dbAddress") REFERENCES "MemberPermisions"("dbAddress") ON UPDATE CASCADE ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS "DataBaseAttributes" (
        "name"	TEXT NOT NULL,
        "value"	INT NOT NULL UNIQUE,
        PRIMARY KEY("name")
);
CREATE TABLE IF NOT EXISTS "Cards" (
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
        "freeIndex"	INTEGER DEFAULT 0,
        "time"	INTEGER NOT NULL DEFAULT 0,
        PRIMARY KEY("id")
);
CREATE TABLE IF NOT EXISTS "Users" (
        "id"	INTEGER NOT NULL,
        "name"	TEXT NOT NULL,
        "key"	BLOB NOT NULL,
        "fSaller"	BOOLEAN DEFAULT false,
        "time"	INTEGER NOT NULL DEFAULT 0,
        PRIMARY KEY("id")
);
CREATE TABLE IF NOT EXISTS "Config" (
        "user"	INTEGER NOT NULL UNIQUE,
        "fFirst"	BOOLEAN NOT NULL DEFAULT false,
        "fSellerMode"	BOOLEAN NOT NULL DEFAULT true,
        FOREIGN KEY("user") REFERENCES "Users"("id") ON UPDATE CASCADE ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS "UsersCards" (
        "id"	INTEGER NOT NULL UNIQUE,
        "user"	INTEGER NOT NULL,
        "card"	INTEGER NOT NULL,
        "owner"	BOOLEAN NOT NULL DEFAULT false,
        "purchasesNumber"	INTEGER DEFAULT 1,
        "received"	INTEGER DEFAULT 0,
        "time"	INTEGER NOT NULL DEFAULT 0,
        FOREIGN KEY("user") REFERENCES "Users"("id") ON UPDATE CASCADE ON DELETE CASCADE,
        FOREIGN KEY("card") REFERENCES "Cards"("id") ON UPDATE CASCADE ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS "Sessions" (
        "id"	INTEGER NOT NULL,
        "usersCardsID"	INTEGER NOT NULL,
        "time"	INTEGER NOT NULL DEFAULT 0,
        FOREIGN KEY("usersCardsID") REFERENCES "UsersCards"("id") ON UPDATE CASCADE ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS "Contacts" (
        "user"	INTEGER NOT NULL,
        "contactUser"	INTEGER NOT NULL,
        FOREIGN KEY("user") REFERENCES "Users"("id") ON UPDATE CASCADE ON DELETE CASCADE,
        FOREIGN KEY("contactUser") REFERENCES "Users"("id") ON UPDATE CASCADE ON DELETE CASCADE
);
INSERT INTO "Cards" VALUES (3580788802,NULL,NULL,NULL,'Test',NULL,NULL,NULL,NULL,NULL,'TestCard','#777777',6,1634202658);
INSERT INTO "Users" VALUES (2019509448,'TestCompany',X'5fe0ebbcbaebcf33a823deaf8f0c1dc27d009dda614b8904f58be2f03af072a6',1,1634202662);
INSERT INTO "Config" VALUES (2019509448,0,1);
INSERT INTO "UsersCards" VALUES (8673727036703801410,2019509448,3580788802,1,0,0,1634202658);
CREATE UNIQUE INDEX IF NOT EXISTS "MemberPermisionsIndex" ON "MemberPermisions" (
        "memberId",
        "dbAddress"
);
CREATE UNIQUE INDEX IF NOT EXISTS "SessionsIndex" ON "Sessions" (
        "id",
        "usersCardsID"
);
CREATE UNIQUE INDEX IF NOT EXISTS "ContactsIndex" ON "Contacts" (
        "user",
        "contactUser"
);
COMMIT;
