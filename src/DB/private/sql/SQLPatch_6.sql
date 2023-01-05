-- changes on the database after refactoring

BEGIN TRANSACTION;

CREATE TABLE "Users_tmp" (
    "id"            INTEGER NOT NULL,
    "name"          TEXT NOT NULL,
    "time" INTEGER NOT NULL DEFAULT 0,
    secret TEXT DEFAULT NULL,
    key TEXT DEFAULT '',
    PRIMARY KEY("id")
);

INSERT INTO Users_tmp SELECT * FROM Users;
DROP TABLE Users;

CREATE TABLE "Users" (
    "id"            INTEGER NOT NULL UNIQUE,
    "key"           TEXT NOT NULL UNIQUE,
    "name"          TEXT NOT NULL DEFAULT "",
    "time"          INTEGER NOT NULL DEFAULT 0,
    "secret"        TEXT DEFAULT NULL,
    PRIMARY KEY("id")
);

INSERT INTO Users SELECT id,key,name,time,secret FROM Users_tmp;
DROP TABLE Users_tmp;

COMMIT;
