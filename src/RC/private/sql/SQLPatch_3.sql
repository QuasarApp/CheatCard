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
ALTER TABLE Cards ADD COLUMN ownerSignature TEXT DEFAULT NULL;
ALTER TABLE Users ADD COLUMN key TEXT DEFAULT '';

COMMIT;
