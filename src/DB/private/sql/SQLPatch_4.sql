-- Adds version for card:
-- See task 380
-- https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/380

BEGIN TRANSACTION;

ALTER TABLE Config ADD COLUMN darkTheme BOOLEAN DEFAULT 0;

COMMIT;
