-- changes on the database after refactoring

BEGIN TRANSACTION;
DROP TABLE Users_old;
DROP TABLE Cards_old;
DROP TABLE UsersCards;

COMMIT;
