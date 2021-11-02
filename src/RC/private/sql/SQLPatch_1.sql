-- Adds version for card:
-- See task 130
-- https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/130

BEGIN TRANSACTION;

ALTER TABLE Cards ADD COLUMN cardVersion INT DEFAULT 0;

COMMIT;
