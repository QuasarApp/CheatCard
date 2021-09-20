BEGIN TRANSACTION;

-- Base data tables:
CREATE TABLE IF NOT EXISTS "Cards" (
        "id"                INTEGER NOT NULL,

        "logo"              BLOB DEFAULT NULL,
        "seal"              BLOB DEFAULT NULL,
        "background"        BLOB DEFAULT NULL,

        "title"             TEXT DEFAULT "",
        "phone"             TEXT DEFAULT "",
        "telegramm"         TEXT DEFAULT "",
        "instagramm"        TEXT DEFAULT "",
        "physicalAddress"   TEXT DEFAULT "",
        "webSite"           TEXT DEFAULT "",
        "freeItemName"      TEXT NOT NULL,

        "color"             INTEGER DEFAULT 0x00777777,

        "freeIndex"         INTEGER DEFAULT 0,
        PRIMARY KEY("id")
);

CREATE TABLE IF NOT EXISTS "Users" (
        "id"            INTEGER NOT NULL,
        "name"          TEXT NOT NULL,
        "key"           BLOB NOT NULL,
        "fSaller"       BOOLEAN DEFAULT false,
        PRIMARY KEY("id")
);

-- Confiuration tabel
CREATE TABLE IF NOT EXISTS "Config" (
       "user"   INTEGER NOT NULL UNIQUE,
       "fFirst" BOOLEAN NOT NULL DEFAULT false,
       "fSellerMode" BOOLEAN NOT NULL DEFAULT true,

       FOREIGN KEY(user) REFERENCES Users(id)
               ON UPDATE CASCADE
               ON DELETE CASCADE
);

-- Matches tables
CREATE TABLE IF NOT EXISTS "UsersCards" (
       "user" INTEGER NOT NULL,
       "card" INTEGER NOT NULL,

       "owner" BOOLEAN NOT NULL DEFAULT false,
       "purchasesNumber" INTEGER DEFAULT 1,
       "received" INTEGER DEFAULT 0,

       FOREIGN KEY(user) REFERENCES Users(id)
               ON UPDATE CASCADE
               ON DELETE CASCADE,
       FOREIGN KEY(card) REFERENCES Cards(id)
               ON UPDATE CASCADE
               ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS "Contacts" (
       "user"   INTEGER NOT NULL,
       "contactUser" INTEGER NOT NULL,

       FOREIGN KEY(user) REFERENCES Users(id)
               ON UPDATE CASCADE
               ON DELETE CASCADE,
       FOREIGN KEY(contactUser) REFERENCES Users(id)
               ON UPDATE CASCADE
               ON DELETE CASCADE
);

-- Indexes
CREATE UNIQUE INDEX IF NOT EXISTS UsersCardsIndex ON UsersCards(user, card);
CREATE UNIQUE INDEX IF NOT EXISTS ContactsIndex ON Contacts(user, contactUser);


COMMIT;
