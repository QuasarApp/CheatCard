-- Adds colorTheme, clientName, cameraDevice for card:
-- See task 111
-- https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/111

BEGIN TRANSACTION;

ALTER TABLE Config ADD COLUMN colorTheme TEXT DEFAULT "#ffffff";
ALTER TABLE Config ADD COLUMN clientName TEXT DEFAULT "Anonim";
ALTER TABLE Config ADD COLUMN cameraDevice TEXT DEFAULT " ";

COMMIT;
