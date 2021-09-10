BEGIN TRANSACTION;

-- Base data tables:
CREATE TABLE IF NOT EXISTS "Cards" (
        "id"                INTEGER NOT NULL,
        "name"              TEXT NOT NULL UNIQUE,
        "title"             TEXT NOT NULL,

        "logo"              BLOB DEFAULT NULL,
        "seal"              BLOB DEFAULT NULL,
        "background"        BLOB DEFAULT NULL,

        "phone"             TEXT NOT NULL DEFAULT "",
        "telegramm"         TEXT NOT NULL DEFAULT "",
        "instagramm"        TEXT NOT NULL DEFAULT "",
        "physicalAddress"   TEXT NOT NULL DEFAULT "",
        "webSite"           TEXT NOT NULL DEFAULT "",

        "color"             INTEGER DEFAULT 0x00777777,

        "purchasesNumber"   INTEGER DEFAULT 0,
        "freeIndex"         INTEGER DEFAULT 0,
        PRIMARY KEY("id" AUTOINCREMENT)
);

CREATE TABLE IF NOT EXISTS "Users" (
        "id"            INTEGER NOT NULL,
        "name"          TEXT NOT NULL UNIQUE,
        "key"           BLOB NOT NULL,
        "visiblename"	TEXT NOT NULL,
        "fSaller" BOOLEAN DEFAULT false,
        PRIMARY KEY("id" AUTOINCREMENT)
);

-- Confiuration tabel
CREATE TABLE IF NOT EXISTS "Config" (
       "user"   INTEGER NOT NULL UNIQUE,
       "fFirst" BOOLEAN NOT NULL DEFAULT false,

       FOREIGN KEY(user) REFERENCES Users(id)
               ON UPDATE CASCADE
               ON DELETE CASCADE
);

-- Matches tables
CREATE TABLE IF NOT EXISTS "UsersCards" (
       "user" INTEGER NOT NULL,
       "card" INTEGER NOT NULL,
       "owner" BOOLEAN NOT NULL DEFAULT false,

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
