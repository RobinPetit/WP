-- WARNING: run this script reset database
-- run with: sqlite3 :memory: ".read resources/common/initdatabase.sql"

.log stderr
.trace /tmp/WP_initdatabase.log
.nullvalue NULL

PRAGMA foreign_keys = ON;

-----------------------------------------------------------
--------------------- SERVER DATABASE ---------------------
.shell rm -f resources/server/database.db
.open resources/server/database.db

-- /!\ The database is not (yet) designed to support
-- all UPDATE and DELETE operations.
-- Examples to add card, friendRequest and account at the end of the document.

-- Cards
---- To add cards (TRIGGERS)
-- Accounts
-- Decks
-- Friends
-- Examples and testing data
-- Export to client database

-- SQLite does not support foreign key to table defined in an other schema.
-- So we can't use common database
-- ATTACH DATABASE "file:resources/common/database.db" AS common;

-- TODO: ondeleteaccount, ondeletecard

BEGIN;
----------------------
-- Cards
SELECT "Cards";

SELECT "Cards/Tables";
CREATE TABLE Card (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT UNIQUE NOT NULL COLLATE NOCASE, -- only used by client but due to SQLite3 behaviour...
    description TEXT NOT NULL, -- ... store it here to simplify the management of adding cards
    effect INTEGER NOT NULL, -- used as effects[difference_type]
    -- parametersListSize INTEGER, -- not stored because it is sizeof(parameters)/sizeof(int32_t)
    parameters BLOB, -- int32_t[]
    cost INTEGER
);

-- qualified table names are not allowed on INSERT, UPDATE, and DELETE statements within triggers
CREATE TABLE Monster (
    id UNIQUE NOT NULL REFERENCES Card,
    health INTEGER NOT NULL CHECK (health >= 0),
    attack INTEGER NOT NULL CHECK (attack >= 0)
);

SELECT "Cards/Views";
CREATE VIEW FullCard
    AS SELECT * FROM Card LEFT OUTER JOIN Monster USING(id);

CREATE VIEW MonsterCard
    AS SELECT * FROM Card INNER JOIN Monster USING(id);

CREATE VIEW SpellCard
    AS SELECT Card.*
        FROM Card LEFT OUTER JOIN Monster USING(id)
        WHERE Monster.health IS NULL;

---- To add Cards

--View can not references objects in other schema/database.
--ATTACH DATABASE "file:resources/client/database.db" AS client;

SELECT "Cards/Triggers";
CREATE TRIGGER addMonsterCard
    INSTEAD OF INSERT ON MonsterCard
    FOR EACH ROW
    BEGIN
        INSERT INTO Card(name, description, effect, parameters, cost)
            VALUES(NEW.name, NEW.description, NEW.effect, NEW.parameters, NEW.cost);
        INSERT INTO Monster(id, health, attack)
            VALUES(
                (SELECT id FROM Card WHERE name = NEW.name), -- Do not use sqlite_sequence internal table in case if explicit id specification
                NEW.health, NEW.attack);
    END;

CREATE TRIGGER addSpellCard
    INSTEAD OF INSERT ON SpellCard
    FOR EACH ROW
    BEGIN
        INSERT INTO Card(name, description, effect, parameters, cost)
            VALUES(NEW.name, NEW.description, NEW.effect, NEW.parameters, NEW.cost);
    -- Duplication of TRIGGER addMonsterCard but
    -- - this allows only one trigger to fire
    -- - this is not executable code strictly speaking but schema definition
    -- - this allows to not have to evaluate a WHEN clause
    END;

----------------------
-- Accounts
SELECT "Accounts";

SELECT "Accounts/Tables";
CREATE TABLE Account (
    id INTEGER PRIMARY KEY ASC,
    login UNIQUE NOT NULL,
    password BLOB NOT NULL,
    victories INTEGER NOT NULL DEFAULT 0,
    defeats INTEGER NOT NULL DEFAULT 0,
    givingup INTEGER NOT NULL DEFAULT 0
    -- ##cards
    -- a way to store cards in the same table would have been:
    -- card1 INTEGER CHECK (card1 >= 0) DEFAULT 0
    --               CHECK (card1 = 0 OR login = 'testing'), -- debug card
    -- card2 INTEGER CHECK (card2 >= 0) DEFAULT 0
    --               CHECK (card2 = 0 OR login = 'testing')  -- debug card
    -- with a new column by new card.
    --
    -- another way to store cards is to use another table: GivenCard
);

CREATE TABLE GivenCard (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    card INTEGER REFERENCES Card,
    owner INTEGER REFERENCES Account,
    counter INTEGER CHECK(counter > 0), -- TODO
    UNIQUE(card, owner)
);

CREATE INDEX givenCardOwner ON givenCard(owner);

SELECT "Accounts/Trigger";
CREATE TRIGGER removeFromGivenCard
    AFTER UPDATE ON GivenCard
    WHEN(NEW.counter == 0)
    BEGIN
        DELETE FROM GivenCard WHERE id == NEW.id;
    END;

----------------------
-- Decks
SELECT "Decks";

SELECT "Decks/Table";
CREATE TABLE Deck (
-- Do not check "less than 2 times in same deck"
    owner INTEGER NOT NULL REFERENCES Account, -- allows indexing
    card0  INTEGER NOT NULL REFERENCES GivenCard,
    card1  INTEGER NOT NULL REFERENCES GivenCard,
    card2  INTEGER NOT NULL REFERENCES GivenCard,
    card3  INTEGER NOT NULL REFERENCES GivenCard,
    card4  INTEGER NOT NULL REFERENCES GivenCard,
    card5  INTEGER NOT NULL REFERENCES GivenCard,
    card6  INTEGER NOT NULL REFERENCES GivenCard,
    card7  INTEGER NOT NULL REFERENCES GivenCard,
    card8  INTEGER NOT NULL REFERENCES GivenCard,
    card9  INTEGER NOT NULL REFERENCES GivenCard,
    card10 INTEGER NOT NULL REFERENCES GivenCard,
    card11 INTEGER NOT NULL REFERENCES GivenCard,
    card12 INTEGER NOT NULL REFERENCES GivenCard,
    card13 INTEGER NOT NULL REFERENCES GivenCard,
    card14 INTEGER NOT NULL REFERENCES GivenCard,
    card15 INTEGER NOT NULL REFERENCES GivenCard,
    card16 INTEGER NOT NULL REFERENCES GivenCard,
    card17 INTEGER NOT NULL REFERENCES GivenCard,
    card18 INTEGER NOT NULL REFERENCES GivenCard,
    card19 INTEGER NOT NULL REFERENCES GivenCard

    -- subqueries prohibited in CHECK constraints
    --CHECK(owner == (SELECT owner FROM GivenCard WHERE id == card0)),
    -- -> TRIGGER ownCardAddedToDeck
);

CREATE INDEX deckOwner ON Deck(owner);

SELECT "Decks/Trigger";
CREATE TRIGGER ownCardAddedToDeck
    BEFORE INSERT ON Deck
    FOR EACH ROW
    WHEN(NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card0)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card1)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card2)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card3)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card4)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card5)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card6)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card7)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card8)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card9)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card10)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card11)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card12)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card13)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card14)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card15)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card16)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card17)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card18)
        OR NEW.owner != (SELECT owner FROM GivenCard WHERE id == NEW.card19))
    BEGIN
        SELECT RAISE (ROLLBACK, "Error: owner of card and owner of deck mismatch");
    END;

----------------------
-- Friends
SELECT "Friends";

SELECT "Friends/Table";
CREATE TABLE Friend (
------
-- Case1: Asymmetric friendship relations: if not mutual it is a friend request.
-- Cons: Table 2 times bigger. Heavy request to differenciates friends and friend request.
-- Pros: Lightweight editing of relationships.

-- Case2: Symmetric friendship reliations, other table used for friend requests.
-- Cons: Heavy to edit: more checks to perform.
-- Pros: Queries for friendslists and requests are fast

-- More read than write so we will use Case2.
------
    first INTEGER NOT NULL REFERENCES Account,
    second INTEGER NOT NULL REFERENCES Account,

    CHECK(first != second),
    UNIQUE(first,second)
);

CREATE INDEX friendFirst ON Friend(first);

CREATE INDEX friendSecond ON Friend(second);

SELECT "Friends/View";
CREATE VIEW Friendship AS
    SELECT * FROM Friend UNION SELECT second AS first, first AS second FROM Friend;

SELECT "Friends/Trigger";
CREATE TRIGGER orderFriend
    AFTER INSERT ON Friend
    FOR EACH ROW
    WHEN(NEW.first > NEW.second)
    BEGIN
        DELETE FROM Friend WHERE(first = NEW.first AND second = NEW.second); -- SQLite do not support INSTEAD OF on tables
        INSERT INTO Friend VALUES(NEW.second,NEW.first);
    END;

SELECT "Friends/Table (requests)";
CREATE TABLE FriendRequest (
    from_ INTEGER REFERENCES Account,
    to_ INTEGER REFERENCES Account,

    CHECK(from_ != to_),
    UNIQUE(from_, to_)
);

SELECT "Friends/Triggers (requests)";
CREATE TRIGGER avoidAlreadyFriendRequest
    BEFORE INSERT ON FriendRequest
    FOR EACH ROW
    WHEN(SELECT 1 FROM Friend
            WHERE ((NEW.from_ = first AND NEW.to_ = second)
                OR (NEW.to_ = first AND NEW.from_ = second)))
    BEGIN
        SELECT RAISE (ROLLBACK, "Error: request for a friend already friend");
    END;

CREATE TRIGGER confirmSymmetricalFriendRequest
    AFTER INSERT ON FriendRequest
    FOR EACH ROW
    WHEN(SELECT 1 FROM FriendRequest WHERE (from_ = NEW.to_ AND to_ = NEW.from_))
    BEGIN
        DELETE FROM FriendRequest WHERE (from_ = NEW.from_ AND to_ = NEW.to_); -- SQLite do not support INSTEAD OF on tables
        DELETE FROM FriendRequest WHERE (from_ = NEW.to_ AND to_ = NEW.from_);
        INSERT INTO Friend VALUES(NEW.from_, NEW.to_);
    END;

END;
SELECT '-------';
----------------------
-- Examples (and tests)
BEGIN;

SELECT "Create testing accounts";
INSERT INTO Account(login, password) VALUES('testing','bianrydatahere'); -- id = 1
INSERT INTO Account(login, password) VALUES('testing2','bianrydatahere');-- id = 2
INSERT INTO Account(login, password) VALUES('Alice','A');
INSERT INTO Account(login, password) VALUES('Bob','B');
INSERT INTO Account(login, password) VALUES('Eve','E');

SELECT "Create testing cards";
INSERT INTO SpellCard(name,description,effect,parameters,cost)
    VALUES('Dernier paradis spell test','This is just a test of spell card',-1,'<insertbinarydatahere>',3);
INSERT INTO MonsterCard(name,description,effect,parameters,cost,health,attack)
    VALUES('Scorpion gardien creature test','This is just a test of creature card',-1,'<insertbinarydatahere>',2,800,10);

SELECT "Create testing friendships";
INSERT INTO FriendRequest
    VALUES(1,2); -- testing to testting2
INSERT INTO FriendRequest
    VALUES(2,1); -- testing2 to testing -> confirm
INSERT INTO Friend
    VALUES(
        (SELECT id FROM Account WHERE login='Alice'),
        (SELECT id FROM Account WHERE login='Bob'));

SELECT "Create testing friend requests";
INSERT INTO FriendRequest(from_, to_)
    VALUES(
        (SELECT id FROM Account WHERE login='Alice'),
        (SELECT id FROM Account WHERE login='Eve'));

SELECT "Give cards";
INSERT INTO GivenCard(card,owner)
    VALUES(1,1), (2,1);

END;
SELECT '-------';

----------------------
-- Exports
.output "resources/client/cards.sql"
SELECT 'SELECT "CLIENT";';
SELECT 'SELECT "CLIENT/Cards";';
SELECT 'SELECT "CLIENT/Cards/Tables";';
.dump Card
.dump Monster
.output stdout

-----------------------------------------------------------
--------------------- CLIENT DATABASE ---------------------
.shell rm -f resources/client/database.db
.open resources/client/database.db

.read "resources/client/cards.sql"

SELECT "CLIENT/Cards/Views";
CREATE VIEW FullCard
    AS SELECT * FROM Card LEFT OUTER JOIN Monster USING(id);

CREATE VIEW MonsterCard
    AS SELECT * FROM Card INNER JOIN Monster USING(id);

CREATE VIEW SpellCard
    AS SELECT Card.*
        FROM Card LEFT OUTER JOIN Monster USING(id)
        WHERE Monster.health IS NULL;

.shell rm -f resources/client/cards.sql
