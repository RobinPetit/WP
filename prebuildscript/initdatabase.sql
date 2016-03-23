-- WARNING: do not run this script
-- If you want (re)initialize the database run initdatabase.sh
-- To create cards to be added to database when initdatabase.sh is executed
--    insert into Cards.sql file
-- To add cards to an existing database, use sqlite3 shell interface
--    execute '.read "CardData.sql"' to import enums from C++ code
--    then use the same commands than the Cards.sql file

-- TODO: Do not forget accounts (by CREATE TABLE IF NOT EXISTS and INSERT OR REPLACE)
-- TODO: ondeleteaccount, ondeletecard
-- TODO: use WITH instead of JOINS

-- NOTE: The database was designed before the database courses, sorry for the mistakes

.header off
.log stderr
.trace /tmp/WP_initdatabase.log
.nullvalue NULL

PRAGMA foreign_keys = ON;

-----------------------------------------------------------
--------------------- SERVER DATABASE ---------------------
SELECT "SERVER";
-- .shell rm -f ../resources/server/database.db
.open ../resources/server/database.db

-- Examples to add card, friendRequest and account at the end of the document.

-- I have grouped here all the comments about SQLite3 limitations
--
-- SQLite does not support foreign key to table defined in an other schema.
-- So we can't use common database
-- ATTACH DATABASE "file:resources/common/database.db" AS common;
-- qualified table names are not allowed on INSERT, UPDATE, and DELETE statements within triggers
-- View can not references objects in other schema/database.
-- ATTACH DATABASE "file:resources/client/database.db" AS client;
-- subqueries prohibited in CHECK constraints
-- It s not possible to create a trigger to allow
-- DELETE FROM Friend requests to be rearranged (first and second)
-- because sqlite neither support INSTEAD OF triggers on table
-- nor FOR EACH STATEMENT tiggers


BEGIN;
----------------------
-- Cards
SELECT "Cards";

SELECT "Cards/Tables";
CREATE TABLE Card (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	name TEXT UNIQUE NOT NULL COLLATE NOCASE, -- only used by client but due to SQLite3 behaviour...
	cost INTEGER DEFAULT 0,
	description TEXT NOT NULL -- ... store it here to simplify the management of adding cards
);

CREATE TABLE Creature (
	id UNIQUE NOT NULL REFERENCES Card,
	attack INTEGER DEFAULT 0,
	health INTEGER DEFAULT 0,
	shield INTEGER DEFAULT 0,
	shieldType INTEGER DEFAULT 0
);

CREATE TABLE Effect (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	owner NOT NULL REFERENCES Card,
	parameter0 INTEGER DEFAULT NULL,
	parameter1 INTEGER DEFAULT NULL,
	parameter2 INTEGER DEFAULT NULL,
	parameter3 INTEGER DEFAULT NULL,
	parameter4 INTEGER DEFAULT NULL,
	parameter5 INTEGER DEFAULT NULL,
	parameter6 INTEGER DEFAULT NULL
);

CREATE INDEX effectOwner ON Effect(owner);

SELECT "Cards/Views";
CREATE VIEW FullCard
	AS SELECT * FROM Card LEFT OUTER JOIN Creature USING(id);

CREATE VIEW CreatureCard
	AS SELECT * FROM Card INNER JOIN Creature USING(id);

-- TODO: without JOIN and with WHERE ... NOT IN ...
CREATE VIEW SpellCard
	AS SELECT Card.*
		FROM Card LEFT OUTER JOIN Creature USING(id)
		WHERE Creature.health IS NULL;

SELECT "Cards/Triggers";
CREATE TRIGGER addCreatureCard
	INSTEAD OF INSERT ON CreatureCard
	FOR EACH ROW
	BEGIN
		INSERT INTO Card(name, cost, description)
			VALUES(NEW.name, NEW.cost, NEW.description);
		INSERT INTO Creature(id, attack, health, shield, shieldType)
			VALUES(
				(SELECT id FROM Card WHERE name = NEW.name), -- Do not use sqlite_sequence internal table in case if explicit id specification
				NEW.attack, NEW.health, NEW.shield, NEW.shieldType);
	END;

CREATE TRIGGER addSpellCard
	INSTEAD OF INSERT ON SpellCard
	FOR EACH ROW
	BEGIN
		INSERT INTO Card(name, cost, description)
			VALUES(NEW.name, NEW.cost, NEW.description);
	-- Duplication of TRIGGER addCreatureCard but
	-- - this allows only one trigger to fire
	-- - this is not executable code strictly speaking but schema definition
	-- - this allows to not have to evaluate a WHEN clause
	END;

-- Temporary import of CardData's enums
.read "CardData.sql"
-- Create cards
.read "Cards.sql"

----------------------
-- Accounts
SELECT "Accounts";

SELECT "Accounts/Tables";
CREATE TABLE Account (
	id                          INTEGER PRIMARY KEY ASC,
	login                       TEXT UNIQUE NOT NULL,
	password                    BLOB NOT NULL,
	victories                   INTEGER NOT NULL DEFAULT 0,
	defeats                     INTEGER NOT NULL DEFAULT 0,
	givingup                    INTEGER NOT NULL DEFAULT 0,
	-- ##achievements
	secondsSpentPlaying         INTEGER DEFAULT 0,
	maxVictoriesInARow          INTEGER DEFAULT 0,
	lastWasVictory              INTEGER DEFAULT 0, -- boolean
	gameWithInDaClub            INTEGER DEFAULT 0,
	ragequits                   INTEGER DEFAULT 0,
	maxStartsInARow             INTEGER DEFAULT 0,
	maxDaysPlayedInARow         INTEGER DEFAULT 0,
	lastGameDateTime            INTEGER DEFAULT 0,
	perfectWins                 INTEGER DEFAULT 0,
	closeWins                   INTEGER DEFAULT 0,
	maxSameCardCounter          INTEGER DEFAULT 0,
	bestLadderPosition          INTEGER DEFAULT 0
);

CREATE TABLE GivenCard ( -- 20 first cards are not stored (everyone own its)
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	card INTEGER NOT NULL REFERENCES Card,
	owner INTEGER NOT NULL REFERENCES Account
	-- do not use counter for now to reflect actual use of data
	-- counter INTEGER NOT NULL DEFAULT 1 , -- CHECK(counter > 0)
	-- UNIQUE(card, owner)
);

CREATE INDEX givenCardOwner ON GivenCard(owner);

CREATE TABLE Achievement (
	id               INTEGER UNIQUE NOT NULL,
	name             TEXT UNIQUE NOT NULL,
	description      TEXT,
	progressRequired INTEGER CHECK(progressRequired > 0) NOT NULL DEFAULT 1
);

-- Create achievement
.read "Achievements.sql"

CREATE INDEX achievementId ON Achievement(id);

SELECT "Accounts/Trigger";
CREATE TRIGGER defaultDeckToNewAccount
	AFTER INSERT ON Account
	BEGIN
		INSERT INTO Deck(owner, name, Card0, Card1, Card2, Card3, Card4, Card5, Card6, Card7, Card8, Card9,
						 Card10, Card11, Card12, Card13, Card14, Card15, Card16, Card17, Card18, Card19)
			VALUES (NEW.id, 'Beginner''s Deck', 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20);
	END;

-- do not use counter for now to reflect actual use of data
-- CREATE TRIGGER removeFromGivenCard
-- 	AFTER UPDATE ON GivenCard
-- 	WHEN(NEW.counter == 0)
-- 	BEGIN
-- 		DELETE FROM GivenCard WHERE id == NEW.id;
-- 	END;

----------------------
-- Decks
SELECT "Decks";

SELECT "Decks/Table";
CREATE TABLE Deck (
-- Do not check "less than 2 times in same deck"
-- Do not check "card owned by player" (for now)
	id INTEGER PRIMARY KEY ASC, -- easier access to rowid column
	owner INTEGER NOT NULL REFERENCES Account, -- allows indexing
	name TEXT NOT NULL,
	card0  INTEGER NOT NULL REFERENCES Card,
	card1  INTEGER NOT NULL REFERENCES Card,
	card2  INTEGER NOT NULL REFERENCES Card,
	card3  INTEGER NOT NULL REFERENCES Card,
	card4  INTEGER NOT NULL REFERENCES Card,
	card5  INTEGER NOT NULL REFERENCES Card,
	card6  INTEGER NOT NULL REFERENCES Card,
	card7  INTEGER NOT NULL REFERENCES Card,
	card8  INTEGER NOT NULL REFERENCES Card,
	card9  INTEGER NOT NULL REFERENCES Card,
	card10 INTEGER NOT NULL REFERENCES Card,
	card11 INTEGER NOT NULL REFERENCES Card,
	card12 INTEGER NOT NULL REFERENCES Card,
	card13 INTEGER NOT NULL REFERENCES Card,
	card14 INTEGER NOT NULL REFERENCES Card,
	card15 INTEGER NOT NULL REFERENCES Card,
	card16 INTEGER NOT NULL REFERENCES Card,
	card17 INTEGER NOT NULL REFERENCES Card,
	card18 INTEGER NOT NULL REFERENCES Card,
	card19 INTEGER NOT NULL REFERENCES Card,

	UNIQUE (owner, name)
);

CREATE INDEX deckOwner ON Deck(owner);

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

CREATE INDEX friendRequestTo ON FriendRequest(to_);

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
INSERT INTO Account(login, password) VALUES('testing','t'); -- id = 1
INSERT INTO Account(login, password) VALUES('testing2','t');-- id = 2
INSERT INTO Account(login, password) VALUES('Alice','A');
INSERT INTO Account(login, password) VALUES('Bob','B');
INSERT INTO Account(login, password) VALUES('Eve','E');


SELECT "Create testing friendships";
INSERT INTO FriendRequest
	VALUES(1,2); -- testing to testing2
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
INSERT INTO Deck(owner, name, Card0, Card1, Card2, Card3, Card4, Card5, Card6, Card7, Card8, Card9,
				 Card10, Card11, Card12, Card13, Card14, Card15, Card16, Card17, Card18, Card19)
	VALUES (1, 'Test', 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 1, 16, 17, 18, 19, 20);

INSERT INTO Deck(owner, name, Card0, Card1, Card2, Card3, Card4, Card5, Card6, Card7, Card8, Card9,
				 Card10, Card11, Card12, Card13, Card14, Card15, Card16, Card17, Card18, Card19)
	VALUES (2, 'Test', 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 1);

SELECT "Update victories/defeats";
UPDATE Account SET victories = 4 WHERE id == 1 OR id == 3;
UPDATE Account SET defeats = 4 WHERE id == 1 OR id == 4;
UPDATE Account SET victories = 3 WHERE id == 2 OR id == 4;
UPDATE Account SET defeats = 2 WHERE id == 2 OR id == 5;
UPDATE Account SET defeats = 0 WHERE id == 3;
UPDATE Account SET victories = 2 WHERE id == 5;

END;
SELECT '-------';

----------------------
-- Exports
.output "toClient.sql"
SELECT 'SELECT "CLIENT";';
SELECT 'SELECT "CLIENT/Cards";';
SELECT 'SELECT "CLIENT/Cards/Tables";';
.dump Card
.dump Creature
.dump Effect
.output stdout

-----------------------------------------------------------
--------------------- CLIENT DATABASE ---------------------
-- its seems that some versions of sqlite have troubles with the .shell
-- metacommand, so this must be executed from outside
-- .shell rm -f ../resources/client/database.db
.open ../resources/client/database.db

.read "toClient.sql"

SELECT "CLIENT/Cards/Views";

CREATE VIEW FullCard
	AS SELECT * FROM Card LEFT OUTER JOIN Creature USING(id);

CREATE VIEW CreatureCard
	AS SELECT * FROM Card INNER JOIN Creature USING(id);

-- TODO: without JOIN and with WHERE ... NOT IN ...
CREATE VIEW SpellCard
	AS SELECT Card.*
		FROM Card LEFT OUTER JOIN Creature USING(id)
		WHERE Creature.health IS NULL;
