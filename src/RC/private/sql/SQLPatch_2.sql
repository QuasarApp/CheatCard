-- Adds colorTheme, shareName, cameraDevice for card:
-- See task 111
-- https://quasarapp.ddns.net:3000/QuasarApp/CheatCard/issues/111

BEGIN TRANSACTION;

CREATE TABLE "Config_tmp" (
    "user"   INTEGER NOT NULL UNIQUE,
    "fFirst" BOOLEAN DEFAULT false,
    "fSellerMode" BOOLEAN DEFAULT true,

    FOREIGN KEY(user) REFERENCES Users(id)
            ON UPDATE CASCADE
            ON DELETE CASCADE
);

INSERT INTO Config_tmp SELECT user,fFirst,fSellerMode FROM Config;
DROP TABLE Config;
CREATE TABLE IF NOT EXISTS "Config" (
    "user"   INTEGER NOT NULL UNIQUE,
    "fFirst" BOOLEAN DEFAULT NULL,
    "fSellerMode" BOOLEAN DEFAULT NULL,
    "colorTheme" TEXT DEFAULT NULL,
    "shareName" BOOLEAN DEFAULT NULL,
    "cameraDevice" TEXT DEFAULT NULL,
    "devSettingEnable" BOOLEAN DEFAULT NULL,
    "host" TEXT DEFAULT NULL,
    "port" TEXT DEFAULT NULL,

    FOREIGN KEY(user) REFERENCES Users(id)
            ON UPDATE CASCADE
            ON DELETE CASCADE
);

INSERT INTO Config(user,fFirst,fSellerMode) SELECT user,fFirst,fSellerMode FROM Config_tmp;
DROP TABLE Config_tmp;

COMMIT;
