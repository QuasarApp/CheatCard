-- Adds version for card:
-- See task 201
-- https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/201

BEGIN TRANSACTION;

-- Change type of column
-- All data pulled to app and modify to new type

CREATE TABLE "Users_tmp" (
        "id"            INTEGER NOT NULL,
        "name"          TEXT NOT NULL,
        "time"          INTEGER NOT NULL DEFAULT 0,

        PRIMARY KEY("id")
);

INSERT INTO Users_tmp SELECT id,name,time FROM Users;
DROP TABLE Users;
CREATE TABLE IF NOT EXISTS "Users" (
        "id"            INTEGER NOT NULL,
        "name"          TEXT NOT NULL,
        "time" INTEGER NOT NULL DEFAULT 0,

        PRIMARY KEY("id")
);

INSERT INTO Users SELECT id,name,time FROM Users_tmp;
DROP TABLE Users_tmp;

ALTER TABLE Users ADD COLUMN secret TEXT DEFAULT NULL;
ALTER TABLE Cards ADD COLUMN ownerSignature VARCHAR(50) DEFAULT NULL;
ALTER TABLE Users ADD COLUMN key TEXT DEFAULT '';


-- Drop deprecated column from usersCard table (owner)

CREATE TABLE "UsersCards_tmp" (
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

INSERT INTO UsersCards_tmp SELECT * FROM UsersCards;
DROP TABLE UsersCards;
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

INSERT INTO UsersCards SELECT id,user,card,purchasesNumber,received,time FROM UsersCards_tmp;
DROP TABLE UsersCards_tmp;
COMMIT;
