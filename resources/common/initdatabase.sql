-- WARNING: run this script reset database
-- run with: sqlite3 :memory: ".read resources/common/initdatabase.sql"

.header off
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
    login TEXT UNIQUE NOT NULL,
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

CREATE TABLE GivenCard ( -- 20 first cards are not stored (everyone own its)
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    card INTEGER NOT NULL REFERENCES Card,
    owner INTEGER NOT NULL REFERENCES Account
    -- counter INTEGER NOT NULL DEFAULT 1 , -- CHECK(counter > 0) 
    -- UNIQUE(card, owner)
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
    name TEXT NOT NULL,
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
    card19 INTEGER NOT NULL REFERENCES GivenCard,

    UNIQUE (owner, name)
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

--
-- SELECT Deck.name, C0.name, C1.name, C2.name, C3.name, C4.name, C5.name, C6.name, C7.name, C8.name, C9.name,
--     C10.name, C11.name, C12.name, C13.name, C14.name, C15.name, C16.name, C17.name, C18.name, C19.name
--     FROM Deck
--     INNER JOIN Card AS C0 ON Card0 == C0.id
--     INNER JOIN Card AS C1 ON Card1 == C1.id
--     INNER JOIN Card AS C2 ON Card2 == C2.id
--     INNER JOIN Card AS C3 ON Card3 == C3.id
--     INNER JOIN Card AS C4 ON Card4 == C4.id
--     INNER JOIN Card AS C5 ON Card5 == C5.id
--     INNER JOIN Card AS C6 ON Card6 == C6.id
--     INNER JOIN Card AS C7 ON Card7 == C7.id
--     INNER JOIN Card AS C8 ON Card8 == C8.id
--     INNER JOIN Card AS C9 ON Card9 == C9.id
--     INNER JOIN Card AS C10 ON Card10 == C10.id
--     INNER JOIN Card AS C11 ON Card11 == C11.id
--     INNER JOIN Card AS C12 ON Card12 == C12.id
--     INNER JOIN Card AS C13 ON Card13 == C13.id
--     INNER JOIN Card AS C14 ON Card14 == C14.id
--     INNER JOIN Card AS C15 ON Card15 == C15.id
--     INNER JOIN Card AS C16 ON Card16 == C16.id
--     INNER JOIN Card AS C17 ON Card17 == C17.id
--     INNER JOIN Card AS C18 ON Card18 == C18.id
--     INNER JOIN Card AS C19 ON Card19 == C19.id;
--

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

CREATE INDEX FriendRequestTo ON FriendRequest(to_);

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
        INSERT INTO Friend VALUES(NEW.from_, NEW.to_);
    END;

CREATE TRIGGER addFriend
    AFTER INSERT ON Friend
    FOR EACH ROW
    BEGIN
        DELETE FROM FriendRequest WHERE (from_ = NEW.first AND to_ = NEW.second); -- SQLite do not support INSTEAD OF on tables
        DELETE FROM FriendRequest WHERE (from_ = NEW.second AND to_ = NEW.first);
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
    VALUES('Voler spell test','This is just a test of spell card',-1,'<insertbinarydatahere>',5);
INSERT INTO MonsterCard(name,description,effect,parameters,cost,health,attack)
    VALUES('Scorpion gardien creature test','This is just a test of creature card',-1,'<insertbinarydatahere>',1,800,10);
INSERT INTO MonsterCard(name,description,effect,parameters,cost,health,attack)
    VALUES('Briseur de l''air creature test','This is just a test of creature card',-1,'<insertbinarydatahere>',2,1200,16);
INSERT INTO MonsterCard(name,description,effect,parameters,cost,health,attack)
    VALUES('Aps creature test','This is just a test of creature card',-1,'<insertbinarydatahere>',2,1800,22);
INSERT INTO MonsterCard(name,description,effect,parameters,cost,health,attack)
    VALUES('Reno creature test','This is just a test of creature card',-1,'<insertbinarydatahere>',2,1000,22);
INSERT INTO MonsterCard(name,description,effect,parameters,cost,health,attack)
    VALUES('H0512 creature test','This is just a test of creature card',-1,'<insertbinarydatahere>',2,1000,30);
INSERT INTO MonsterCard(name,description,effect,parameters,cost,health,attack)
    VALUES('Cent artilleurs creature test','This is just a test of creature card',-1,'<insertbinarydatahere>',2,1600,35);
INSERT INTO MonsterCard(name,description,effect,parameters,cost,health,attack)
    VALUES('Hélimitrailleuse creature test','This is just a test of creature card',-1,'<insertbinarydatahere>',2,1000,25);
INSERT INTO MonsterCard(name,description,effect,parameters,cost,health,attack)
    VALUES('Rufus creature test','This is just a test of creature card',-1,'<insertbinarydatahere>',1,500,35);
INSERT INTO MonsterCard(name,description,effect,parameters,cost,health,attack)
    VALUES('Boule motorisée creature test','This is just a test of creature card',-1,'<insertbinarydatahere>',3,2600,45);
INSERT INTO MonsterCard(name,description,effect,parameters,cost,health,attack)
    VALUES('Grand Œdème creature test','This is just a test of creature card',-1,'<insertbinarydatahere>',3,2500,52);
INSERT INTO MonsterCard(name,description,effect,parameters,cost,health,attack)
    VALUES('Jénova°naissance creature test','This is just a test of creature card',-1,'<insertbinarydatahere>',4,4000,64);
INSERT INTO MonsterCard(name,description,effect,parameters,cost,health,attack)
    VALUES('Dayne creature test','This is just a test of creature card',-1,'<insertbinarydatahere>',2,1200,55);
INSERT INTO MonsterCard(name,description,effect,parameters,cost,health,attack)
    VALUES('Rude creature test','This is just a test of creature card',-1,'<insertbinarydatahere>',3,2000,70);
INSERT INTO MonsterCard(name,description,effect,parameters,cost,health,attack)
    VALUES('Gi Nattack creature test','This is just a test of creature card',-1,'<insertbinarydatahere>',6,5500,150);
INSERT INTO SpellCard(name,description,effect,parameters,cost)
    VALUES('Sentir spell test','This is just a test of spell card',-1,'<insertbinarydatahere>',4);
INSERT INTO SpellCard(name,description,effect,parameters,cost)
    VALUES('Coup mortel spell test','This is just a test of spell card',-1,'<insertbinarydatahere>',4);
INSERT INTO SpellCard(name,description,effect,parameters,cost)
    VALUES('Transformation spell test','This is just a test of spell card',-1,'<insertbinarydatahere>',10);
INSERT INTO SpellCard(name,description,effect,parameters,cost)
    VALUES('Attaque double spell test','This is just a test of spell card',-1,'<insertbinarydatahere>',15);
INSERT INTO SpellCard(name,description,effect,parameters,cost)
    VALUES('FurieMax spell test','This is just a test of spell card',-1,'<insertbinarydatahere>',15);

SELECT "Create testing friendships";
INSERT INTO FriendRequest
    VALUES(1,2); -- testing to testting2
INSERT INTO FriendRequest
    VALUES(2,1); -- testing2 to testing -> confirm
INSERT INTO FriendRequest(from_, to_)
    VALUES(
        (SELECT id FROM Account WHERE login='Alice'),
        (SELECT id FROM Account WHERE login='Bob'));
INSERT INTO Friend
    VALUES(
        (SELECT id FROM Account WHERE login='Alice'),
        (SELECT id FROM Account WHERE login='Bob')); -- -> remove request

SELECT "Create testing friend requests";
INSERT INTO FriendRequest(from_, to_)
    VALUES(
        (SELECT id FROM Account WHERE login='Alice'),
        (SELECT id FROM Account WHERE login='Eve'));

SELECT "Give cards";
INSERT INTO GivenCard(card,owner)
    VALUES(15,1), (16,1);

SELECT "Create deck";
INSERT INTO Deck VALUES
    (1, 'Test', 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 1, 16, 17, 18, 19, 20); 

SELECT "Update victories/defeats";
UPDATE Account SET victories = 4 WHERE id == 1 OR id == 3;
UPDATE Account SET defeats = 4 WHERE id == 1 OR id == 4;
UPDATE Account SET victories = 3 WHERE id == 2 OR id == 4;
UPDATE Account SET defeats = 2 WHERE id == 2 OR id == 5;
UPDATE Account SET defeats = 1 WHERE id == 3;
UPDATE Account SET victories = 2 WHERE id == 5;

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

