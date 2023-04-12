-- changes on the database after refactoring

BEGIN TRANSACTION;
DROP TABLE IF EXISTS Users_old;
DROP TABLE IF EXISTS Cards_old;
DROP TABLE IF EXISTS Contacts_old;

COMMIT;
