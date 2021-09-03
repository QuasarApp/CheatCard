BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "Cards" (
        "id"                INTEGER NOT NULL,
        "name"              TEXT NOT NULL UNIQUE,
        "image"             BLOB DEFAULT NULL,
        "purchasesNumber"   INTEGER DEFAULT 0,
        "freeIndex"         INTEGER DEFAULT 0,
        PRIMARY KEY("id" AUTOINCREMENT)
);

CREATE TABLE IF NOT EXISTS "Sallers" (
        "id"	INTEGER NOT NULL,
        "name"	TEXT NOT NULL UNIQUE,
        PRIMARY KEY("id" AUTOINCREMENT),
        FOREIGN KEY("card") REFERENCES "Cards"("id")
        ON UPDATE CASCADE
        ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS "Users" (
        "id"	INTEGER NOT NULL,
        "name"	TEXT NOT NULL UNIQUE,
        PRIMARY KEY("id" AUTOINCREMENT),
        FOREIGN KEY("Card") REFERENCES "Cards"("id")
        ON UPDATE CASCADE
        ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS "UsersCards" (
       "user" INTEGER NOT NULL,
       "card" INTEGER NOT NULL,

       FOREIGN KEY(user) REFERENCES Users(id)
               ON UPDATE CASCADE
               ON DELETE CASCADE,
       FOREIGN KEY(card) REFERENCES Cards(id)
               ON UPDATE CASCADE
               ON DELETE CASCADE
)

CREATE TABLE IF NOT EXISTS "Contacts" (
       "user"   INTEGER NOT NULL,
       "saller" INTEGER NOT NULL,

       FOREIGN KEY(user) REFERENCES Users(id)
               ON UPDATE CASCADE
               ON DELETE CASCADE,
       FOREIGN KEY(saller) REFERENCES Sallers(id)
               ON UPDATE CASCADE
               ON DELETE CASCADE
)

CREATE TABLE IF NOT EXISTS "SallersCards" (
       "saller" INTEGER NOT NULL,
       "card" INTEGER NOT NULL,


       FOREIGN KEY(saller) REFERENCES Sallers(id)
               ON UPDATE CASCADE
               ON DELETE CASCADE,
       FOREIGN KEY(card) REFERENCES Cards(id)
               ON UPDATE CASCADE
               ON DELETE CASCADE
)

CREATE UNIQUE INDEX IF NOT EXISTS UsersCardsIndex ON UsersCards(user, card);
CREATE UNIQUE INDEX IF NOT EXISTS ContactsIndex ON Contacts(user, saller);
CREATE UNIQUE INDEX IF NOT EXISTS SallersCardsIndex ON SallersCards(saller, card);


COMMIT;
