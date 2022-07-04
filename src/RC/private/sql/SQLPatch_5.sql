-- Adds version for card:
-- See task 380
-- https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/380

BEGIN TRANSACTION;

DROP TABLE Contacts;

CREATE TABLE IF NOT EXISTS "Contacts" (
       "userKey" VARCHAR(44) NOT NULL,
       "childUserKey" VARCHAR(44) NOT NULL,
       "info" TEXT DEFAULT ''
);

CREATE UNIQUE INDEX ContactsIndex ON Contacts(userKey, childUserKey);

COMMIT;
