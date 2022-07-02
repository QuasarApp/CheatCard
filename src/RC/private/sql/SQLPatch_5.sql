-- Adds version for card:
-- See task 380
-- https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/380

BEGIN TRANSACTION;

DROP TABLE Contacts;

CREATE TABLE IF NOT EXISTS "Contacts" (
       "user" INTEGER NOT NULL,
       "genesisKey" INTEGER NOT NULL,
       "childuserId" INTEGER NOT NULL,

       "info" TEXT DEFAULT '',

       FOREIGN KEY(user) REFERENCES Users(id)
               ON UPDATE CASCADE
               ON DELETE CASCADE
);

CREATE UNIQUE INDEX ContactsIndex ON Contacts(user, genesisKey);

COMMIT;
