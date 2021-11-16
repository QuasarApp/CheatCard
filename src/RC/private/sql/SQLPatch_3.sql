-- Adds version for card:
-- See task 201
-- https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/201

BEGIN TRANSACTION;

ALTER TABLE Users ADD COLUMN secret TEXT DEFAULT NULL;
ALTER TABLE Cards ADD COLUMN ownerSignature TEXT DEFAULT NULL;

-- Change type of column
ALTER TABLE Users DROP COLUMN key;
ALTER TABLE Users ADD COLUMN key;

COMMIT;
