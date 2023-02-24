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
	"fontColor"	INTEGER DEFAULT 0x00777777,
	"freeIndex"	INTEGER DEFAULT 0,
	"time"	INTEGER NOT NULL DEFAULT 0,
	"cardVersion"	INT DEFAULT 0,
	"ownerSignature"	VARCHAR(64) DEFAULT NULL,
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
	FOREIGN KEY("card") REFERENCES "Cards"("id") ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY("user") REFERENCES "Users"("id") ON UPDATE CASCADE ON DELETE CASCADE
);
CREATE TABLE IF NOT EXISTS "Contacts" (
	"userKey"	VARCHAR(44) NOT NULL,
	"childUserKey"	VARCHAR(44) NOT NULL,
	"info"	TEXT DEFAULT ''
);
INSERT INTO "DataBaseAttributes" VALUES ('version',6);
INSERT INTO "Cards" VALUES (-3193611896,NULL,NULL,NULL,'LOCAL','','','','','','Vjgc','#777777','#4baf88',12,1642617815,2,'fkQtCbQFD3UOb2p_RsY0LMcDQVDdjq2PPmQmyIWnvSo=');
INSERT INTO "Cards" VALUES (-3125471765,NULL,NULL,NULL,'This is super Card','','','','','','gfsdg','#777777','#000000',14,1642617815,1,'fkQtCbQFD3UOb2p_RsY0LMcDQVDdjq2PPmQmyIWnvSo=');
INSERT INTO "Cards" VALUES (2829571015,NULL,NULL,NULL,'hsvy card','','','','','','metal','#d4ffff','#000000',14,1642617815,1,'fkQtCbQFD3UOb2p_RsY0LMcDQVDdjq2PPmQmyIWnvSo=');
INSERT INTO "Sessions" VALUES (2946952661691534762,3292231956754912664,1639940822);
INSERT INTO "Sessions" VALUES (274993584650143898,3292231956754912664,1639940956);
INSERT INTO "Sessions" VALUES (274993584650143898,3292231957360160303,1639941217);
INSERT INTO "Sessions" VALUES (1520666106835167110,8798608286869106223,1640349290);
INSERT INTO "Sessions" VALUES (99703799342579585,8798608286869106223,1640349809);
INSERT INTO "Sessions" VALUES (99703799342579585,8798608286263858584,1640350071);
INSERT INTO "Sessions" VALUES (2662339576935984873,8798608286263858584,1640718118);
INSERT INTO "Sessions" VALUES (956472473731481304,8798608286263858584,1640772656);
INSERT INTO "Sessions" VALUES (70612948197460960,-6839193493960988139,1641581104);
INSERT INTO "Sessions" VALUES (70612948197460960,-6839193493892848008,1641581191);
INSERT INTO "Sessions" VALUES (70612948197460960,-6839193494256888889,1641581292);
INSERT INTO "Config" VALUES (1194704293,0,1,'#ff6b01',1,'/dev/video0',1,'localhost',NULL,0);
INSERT INTO "Config" VALUES (2019509448,0,1,NULL,NULL,NULL,NULL,NULL,NULL,0);
INSERT INTO "Config" VALUES (766532485,0,1,NULL,1,NULL,0,NULL,NULL,0);
INSERT INTO "Config" VALUES (3266044092,0,0,'#ff6b01',1,'/dev/video0',0,NULL,NULL,1);
INSERT INTO "Users" VALUES (766532485,'gfdgdf',1640962994,'j2dV0Etyvf6hY15eBy5sGpRYsMug_RDlWKdVvOYaMYw=','n-AjjcVb4gk30tjaACs_sgOW_G8WDTcStCj__zeqPL8=');
INSERT INTO "Users" VALUES (1194704293,'sdf',1640854039,'EqIzz2V6TrXkC68sBOzaAGp-XqwDj1ewthjSOBdgTjQ=','zjCNjuwkXSMYEiSSgAlAh_IWpEIl8nfD9JLEU74z-2o=');
INSERT INTO "Users" VALUES (2048585630,'ЯАН',1640772656,'gQsXBmQT3uY9XWns-upKB3bAOH1x_bIWdqrwzOpru9Y=','CAOADk5dT5vxqHmj9bHVitiLfRsrKtuHD1PWZMI0L9c=');
INSERT INTO "Users" VALUES (2702593472,'sdf',1641581293,'-538XQ5cBNI4Shxjko-MZGr4VNZsc4TDdbprlKAmJLs=','FlZQyr_YskKHeR6RtmFdRpNRw2kDDqsBt37w4W9-gdk=');
INSERT INTO "Users" VALUES (3266044092,'sdf',1642617981,'RC9lkJPyuAELwwrLRvBVPcOmZON1nvZ43sfmy4E-DB8=','fkQtCbQFD3UOb2p_RsY0LMcDQVDdjq2PPmQmyIWnvSo=');
INSERT INTO "UsersCards" VALUES (5131215870048818532,1194704293,3223016804,4,0,1639932344);
INSERT INTO "UsersCards" VALUES (5131215870419154582,1194704293,3593352854,1,0,1639932281);
INSERT INTO "UsersCards" VALUES (5131215868698849568,1194704293,1873047840,4,0,1639932447);
INSERT INTO "UsersCards" VALUES (3292231956754912664,766532485,2358302104,11,3,1639941123);
INSERT INTO "UsersCards" VALUES (3292231957360160303,766532485,2963549743,3,0,1639941217);
INSERT INTO "UsersCards" VALUES (8798608286869106223,2048585630,2963549743,14,2,1640350038);
INSERT INTO "UsersCards" VALUES (8798608286263858584,2048585630,2358302104,9,1,1640772656);
INSERT INTO "UsersCards" VALUES (-6839193493960988139,2702593472,3125471765,10,0,1641581111);
INSERT INTO "UsersCards" VALUES (-6839193493892848008,2702593472,3193611896,9,0,1641581193);
INSERT INTO "UsersCards" VALUES (-6839193494256888889,2702593472,2829571015,10,0,1641581293);
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
COMMIT;
