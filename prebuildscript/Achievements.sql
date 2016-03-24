-- To create achievement to be added to database when initdatabase.sh is executed
--    insert into this file
-- To add achievements to an existing database, use sqlite3 shell interface

-- /!\ Update include/server/ServerDatabase.hpp when updating this file

-- Do not reuse already used id, even if not currently used!
-- Do not change already defined id

INSERT INTO Achievement(id, name, description, progressRequired) VALUES
(
	1,
	"Felt like a year",
	"Spend 365 minutes playing",
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
	"Threw it on the ground",
	"Lose a game by destroying your computer, or find some other way (do not trust the system)",
	1
),
(
	6,
	"Legen-wait for it...",
	"Unlock all day cards",
	1
),
(
	7,
	"...dary!",
	"Make it to the top 1% of the ladder",
	1
),
(
	8,
	"Better luck next time",
	"Unlock the same card 5 times",
	5
),
(
	9,
	"You lucky dog",
	"Get to play first 5 times in a row",
	5
),
(
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
