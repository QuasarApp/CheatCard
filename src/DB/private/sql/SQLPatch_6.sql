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
	"fontColor"	INTEGER DEFAULT 0x00777777,
	"freeIndex"	INTEGER DEFAULT 0,
	"time"	INTEGER NOT NULL DEFAULT 0,
	"cardVersion"	INT DEFAULT 0,
        "ownerSignature"	VARCHAR(50) DEFAULT NULL,
	PRIMARY KEY("id")
);

CREATE TABLE IF NOT EXISTS "Sessions" (
	"id"	INTEGER NOT NULL,
	"usersCardsID"	INTEGER NOT NULL,
	"time"	INTEGER NOT NULL DEFAULT 0,
	FOREIGN KEY("usersCardsID") REFERENCES "UsersCards"("id") ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS "Config" (
	"user"	INTEGER NOT NULL UNIQUE,
	"fFirst"	BOOLEAN DEFAULT NULL,
	"fSellerMode"	BOOLEAN DEFAULT NULL,
	"colorTheme"	TEXT DEFAULT NULL,
	"shareName"	BOOLEAN DEFAULT NULL,
	"cameraDevice"	TEXT DEFAULT NULL,
	"devSettingEnable"	BOOLEAN DEFAULT NULL,
	"host"	TEXT DEFAULT NULL,
	"port"	TEXT DEFAULT NULL,
	"darkTheme"	BOOLEAN DEFAULT 0,
	FOREIGN KEY("user") REFERENCES "Users"("id") ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS "Users" (
	"id"	INTEGER NOT NULL,
	"name"	TEXT NOT NULL,
	"time"	INTEGER NOT NULL DEFAULT 0,
	"secret"	TEXT DEFAULT NULL,
	"key"	TEXT DEFAULT '',
	PRIMARY KEY("id")
);

CREATE TABLE IF NOT EXISTS "UsersCards" (
	"id"	INTEGER NOT NULL UNIQUE,
	"user"	INTEGER NOT NULL,
	"card"	INTEGER NOT NULL,
	"purchasesNumber"	INTEGER DEFAULT 1,
	"received"	INTEGER DEFAULT 0,
	"time"	INTEGER NOT NULL DEFAULT 0,
	FOREIGN KEY("user") REFERENCES "Users"("id") ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY("card") REFERENCES "Cards"("id") ON UPDATE CASCADE ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS "Contacts" (
	"userKey"	VARCHAR(44) NOT NULL,
	"childUserKey"	VARCHAR(44) NOT NULL,
	"info"	TEXT DEFAULT ''
);

CREATE UNIQUE INDEX IF NOT EXISTS "MemberPermisionsIndex" ON "MemberPermisions" (
        "memberId",
        "dbAddress"
);

CREATE UNIQUE INDEX IF NOT EXISTS "SessionsIndex" ON "Sessions" (
        "id",
        "usersCardsID"
);

CREATE UNIQUE INDEX IF NOT EXISTS "ContactsIndex" ON "Contacts" (
        "userKey",
        "childUserKey"
);
