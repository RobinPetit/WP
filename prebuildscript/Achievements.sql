-- To create achievement to be added to database when initdatabase.sh is executed
--    insert into this file
-- To add achievements to an existing database, use sqlite3 shell interface

-- /!\ Update include/server/ServerDatabase.hpp when updating this file

-- Do not reuse already used id, even if not currently used!
-- Do not change already defined id

-- TODO : 
-- 6 : counter should be the total when the game is released
-- 7 : counter was changed to be more relevant (highest position in ladder, in percent), but nothing was implemented

INSERT INTO Achievement(id, name, description, progressRequired) VALUES
(
	1,
	"Felt like a year",
	"Spend 365 minutes playing (21900 seconds)",
	365*60
),
(
	2,
	"Valhar Morgulis",
	"Defeat 25 opponents",
	25
),
(
	3,
	"Much inspiration",
	"Win 5 games in a row",
	5
),
(
	4,
	"In Da Club",
	"Play against an employee of Wizard Poker or someone who has this achievement",
	1
),
(
	5,
	"Rage Quit",
	"Calm down, Michael",
	1
),
(
	6,
	"Legen-wait for it...",
	"Unlock all of the cards",
	1
),
(
	-- Maybe should add a number of game to have played
	7,
	"...dary!",
	"Make it to the top 1% of the ladder (be better than 99%)",
	99
),
(
	8,
	"Better luck next time",
	"Unlock the same card 5 times",
	5
),
(
	9,
	"J*** in my pants",
	"Get to play first, 5 times in a row",
	5
),
(
	-- TODO: use client local time instead of UTC
	10,
	"Half-No-Life",
	"Play every day for 42 days",
	42
),
(
	11,
	"Perfect",
	"Win without taking any damage",
	1
),
(
	12,
	"Close call",
	"Win with only 1 point of health remaining",
	1
);
