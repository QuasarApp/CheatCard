BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "Cards" (
        "id"	INTEGER NOT NULL,
        "name"	TEXT,
        "image"	BLOB,
        "purchasesNumber"	INTEGER,
        "freeIndex"	INTEGER,
        PRIMARY KEY("id" AUTOINCREMENT)
);

CREATE TABLE IF NOT EXISTS "Sallers" (
        "id"	INTEGER NOT NULL,
        "name"	TEXT,
        "card"	INTEGER,
        PRIMARY KEY("id" AUTOINCREMENT),
        FOREIGN KEY("card") REFERENCES "Cards"("id")
        ON UPDATE CASCADE
        ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS "Users" (
        "id"	INTEGER NOT NULL,
        "Name"	TEXT,
        "Card"	INTEGER,
        PRIMARY KEY("id" AUTOINCREMENT),
        FOREIGN KEY("Card") REFERENCES "Cards"("id")
        ON UPDATE CASCADE
        ON DELETE CASCADE
);
COMMIT;
