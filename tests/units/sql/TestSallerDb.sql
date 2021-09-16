BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "Cards" (
	"id"	INTEGER NOT NULL,
	"logo"	BLOB DEFAULT NULL,
	"seal"	BLOB DEFAULT NULL,
	"background"	BLOB DEFAULT NULL,
        "title"	TEXT DEFAULT "",
        "phone"	TEXT DEFAULT "",
        "telegramm"	TEXT DEFAULT "",
        "instagramm"	TEXT DEFAULT "",
        "physicalAddress"	TEXT DEFAULT "",
        "webSite"	TEXT DEFAULT "",
	"color"	INTEGER DEFAULT 0x00777777,
	"freeIndex"	INTEGER DEFAULT 0,
	PRIMARY KEY("id")
);
CREATE TABLE IF NOT EXISTS "Config" (
	"user"	INTEGER NOT NULL UNIQUE,
	"fFirst"	BOOLEAN NOT NULL DEFAULT false,
	FOREIGN KEY("user") REFERENCES "Users"("id") ON UPDATE CASCADE ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS "Contacts" (
	"user"	INTEGER NOT NULL,
	"contactUser"	INTEGER NOT NULL,
	FOREIGN KEY("user") REFERENCES "Users"("id") ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY("contactUser") REFERENCES "Users"("id") ON UPDATE CASCADE ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS "MemberPermisions" (
	"memberId"	VARCHAR(32) NOT NULL,
	"dbAddress"	VARCHAR(44) NOT NULL,
	"lvl"	tinyint NOT NULL,
	FOREIGN KEY("memberId") REFERENCES "NetworkMembers"("id") ON UPDATE CASCADE ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS "NetworkMembers" (
	"id"	VARCHAR(32),
	"authenticationData"	BLOB NOT NULL,
	"trust"	INTEGER DEFAULT 0,
	PRIMARY KEY("id")
);
CREATE TABLE IF NOT EXISTS "Users" (
	"id"	INTEGER NOT NULL,
	"name"	TEXT NOT NULL,
	"key"	BLOB NOT NULL,
	"fSaller"	BOOLEAN DEFAULT false,
	PRIMARY KEY("id")
);
CREATE TABLE IF NOT EXISTS "UsersCards" (
	"user"	INTEGER NOT NULL,
	"card"	INTEGER NOT NULL,
	"owner"	BOOLEAN NOT NULL DEFAULT false,
	"purchasesNumber"	INTEGER DEFAULT 1,
	FOREIGN KEY("user") REFERENCES "Users"("id") ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY("card") REFERENCES "Cards"("id") ON UPDATE CASCADE ON DELETE CASCADE
);
INSERT INTO "Cards" VALUES (3618836081,X'',X'',X'','Test','','','','','','#777777',5);
INSERT INTO "Config" VALUES (2425662745,0);
INSERT INTO "Users" VALUES (2425662745,'Test',X'5fe0ebbcbaebcf33a823deaf8f0c1dc27d009dda614b8904f58be2f03af072a6',1);
INSERT INTO "UsersCards" VALUES (2425662745,3618836081,1,0);
CREATE UNIQUE INDEX IF NOT EXISTS "ContactsIndex" ON "Contacts" (
	"user",
	"contactUser"
);
CREATE UNIQUE INDEX IF NOT EXISTS "MemberPermisionsIndex" ON "MemberPermisions" (
	"memberId",
	"dbAddress"
);
CREATE UNIQUE INDEX IF NOT EXISTS "UsersCardsIndex" ON "UsersCards" (
	"user",
	"card"
);
COMMIT;
