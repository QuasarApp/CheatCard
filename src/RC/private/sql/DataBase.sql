BEGIN TRANSACTION;

-- Base data tables:
CREATE TABLE IF NOT EXISTS "Cards" (
        "id"                INTEGER NOT NULL,
        "name"              TEXT NOT NULL UNIQUE,
        "image"             BLOB DEFAULT NULL,
        "purchasesNumber"   INTEGER DEFAULT 0,
        "freeIndex"         INTEGER DEFAULT 0,
        PRIMARY KEY("id" AUTOINCREMENT)
);

CREATE TABLE IF NOT EXISTS "Users" (
        "id"	INTEGER NOT NULL,
        "name"	TEXT NOT NULL UNIQUE,
        "fSaller" BOOLEAN DEFAULT false,
        PRIMARY KEY("id" AUTOINCREMENT),
        FOREIGN KEY("Card") REFERENCES "Cards"("id")
        ON UPDATE CASCADE
        ON DELETE CASCADE
);

-- Confiuration tabel
CREATE TABLE IF NOT EXISTS "Config" (
       "user"   INTEGER NOT NULL UNIQUE,
       "fFirst" BOOLEAN NOT NULL DEFAULT false,

       FOREIGN KEY(user) REFERENCES Users(id)
               ON UPDATE CASCADE
               ON DELETE CASCADE
)

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
)

CREATE TABLE IF NOT EXISTS "Contacts" (
       "user"   INTEGER NOT NULL,
       "contactUser" INTEGER NOT NULL,

       FOREIGN KEY(user) REFERENCES Users(id)
               ON UPDATE CASCADE
               ON DELETE CASCADE,
       FOREIGN KEY(contactUser) REFERENCES Users(id)
               ON UPDATE CASCADE
               ON DELETE CASCADE
)

-- Indexes
CREATE UNIQUE INDEX IF NOT EXISTS UsersCardsIndex ON UsersCards(user, card);
CREATE UNIQUE INDEX IF NOT EXISTS ContactsIndex ON Contacts(user, saller);


COMMIT;
