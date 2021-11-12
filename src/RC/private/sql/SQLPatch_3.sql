-- Adds version for card:
-- See task 201
-- https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/201

BEGIN TRANSACTION;

ALTER TABLE Users ADD COLUMN secret BLOB DEFAULT NULL;
ALTER TABLE Cards ADD COLUMN ownerSignature BLOB DEFAULT NULL;

COMMIT;
