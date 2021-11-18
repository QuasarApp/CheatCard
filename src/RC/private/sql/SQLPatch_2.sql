-- Adds colorTheme, clientName, cameraDevice for card:
-- See task 111
-- https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/111

BEGIN TRANSACTION;

ALTER TABLE Config ADD COLUMN colorTheme TEXT DEFAULT NULL;
ALTER TABLE Config ADD COLUMN clientName TEXT DEFAULT NULL;
ALTER TABLE Config ADD COLUMN cameraDevice TEXT DEFAULT NULL;
ALTER TABLE Config ADD COLUMN devSettingEnable TEXT DEFAULT NULL;
ALTER TABLE Config ADD COLUMN host TEXT DEFAULT NULL;
ALTER TABLE Config ADD COLUMN port TEXT DEFAULT NULL;

COMMIT;
