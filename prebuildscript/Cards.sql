-- To create cards to be added to database when initdatabase.sh is executed
--    insert into this file
-- To add cards to an existing database, use sqlite3 shell interface
--    execute '.read "CardData.sql"' to import enums from C++ code
--    then adapt the commands below to your needs

-- TODO:
-- UNLIMITED_TURNS=100

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'Tower guard',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_3'),
	'This creature gains 1 HEALTH each turn',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_2'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_8'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_1'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_BLUE')
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Tower guard'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_THIS'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_SET_CONSTRAINT'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CC_TURN_HEALTH_CHANGE'),
	(1),
	(100),
	(SELECT id FROM ENUM_VALUES WHERE name == 'NO_CASTER_NEEDED'),
	(NULL)
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'Diabolic imp',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_2'),
	'This creature gains 1 ATTACK each turn',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_3'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_2'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_1'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_NONE')
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Diabolic imp'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_THIS'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_SET_CONSTRAINT'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CC_TURN_ATTACK_CHANGE'),
	(1),
	(100),
	(SELECT id FROM ENUM_VALUES WHERE name == 'NO_CASTER_NEEDED'),
	(NULL)
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'Little Ragnaros',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_4'),
	'Each turn, your opponent loses 2 HP',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_4'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_4'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_1'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_LEGENDARY')
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Little Ragnaros'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PLAYER_OPPO'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PE_SET_CONSTRAINT'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PC_TURN_HEALTH_CHANGE'),
	(-2),
	(100),
	(SELECT id FROM ENUM_VALUES WHERE name == 'IF_CASTER_ALIVE'),
	(NULL)
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'White wizard',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_7'),
	'Blocks the next 2 attacks on your team',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_7'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_7'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_1'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_NONE')
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'White wizard'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_THIS'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_SET_CONSTRAINT'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CC_TEMP_BLOCK_ATTACKS'),
	(2),
	(100),
	(SELECT id FROM ENUM_VALUES WHERE name == 'NO_CASTER_NEEDED'),
	(NULL)
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'Greedy imp',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_2'),
	'Allows you to draw one more card at the beginning of your next turn',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_2'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_2'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_2'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_ORANGE')
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Greedy imp'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PLAYER_SELF'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PE_SET_CONSTRAINT'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PC_TURN_CARDS_PICKED'),
	(2),
	(1),
	(SELECT id FROM ENUM_VALUES WHERE name == 'NO_CASTER_NEEDED'),
	(NULL)
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'Unbridled bear',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_3'),
	'Takes away 1 shield of each opposite creatures when placed',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_2'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_4'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_1'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_NONE')
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Unbridled bear'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_OPPO_TEAM'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_CHANGE_SHIELD'),
	(-1),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'Giant soldier',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_9'),
	'No effect',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_8'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_8'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_3'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_BLUE')
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'White dragon',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_10'),
	'All your friendly creatures gain 1 ATTACK each turn',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_9'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_10'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_1'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_NONE')
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'White dragon'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_TEAM'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_SET_CONSTRAINT'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CC_TURN_ATTACK_CHANGE'),
	(1),
	(100),
	(SELECT id FROM ENUM_VALUES WHERE name == 'IF_CASTER_ALIVE'),
	(NULL)
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'Mecha spider',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_3'),
	'Reset the attack for a random opponent''s creature',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_4'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_2'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_1'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_ORANGE')
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Mecha spider'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_OPPO_RAND'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_RESET_ATTACK'),
	(1),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'Mirage of hapiness',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_3'),
	'Your opponent can''t play more than 2 spells per turn',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_1'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_5'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_3'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_BLUE')
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Mirage of hapiness'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PLAYER_OPPO'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PE_SET_CONSTRAINT'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PC_TEMP_SPELL_CALL_LIMIT'),
	(2),
	(100),
	(SELECT id FROM ENUM_VALUES WHERE name == 'NO_CASTER_NEEDED'),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Abracadabra',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_10'),
	'Your opponent loses 2 random cards from his hand'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Abracadabra'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PLAYER_OPPO'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PE_LOSE_HAND_CARDS'),
	(2),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Block',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_9'),
	'Block the next 3 attacks for the whole team'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Block'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_TEAM'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_SET_CONSTRAINT'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CC_TEMP_BLOCK_ATTACKS'),
	(3),
	(100),
	(SELECT id FROM ENUM_VALUES WHERE name == 'NO_CASTER_NEEDED'),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Care',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_5'),
	'Increase a creature''s health of 2 points'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Care'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_INDX'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_CHANGE_HEALTH'),
	(2),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Complete Care',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_7'),
	'Restore the health point of a creature of your choice'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Complete Care'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_INDX'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_RESET_HEALTH'),
	(NULL),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Discard',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_4'),
	'Your opponent loses a random card from his hand'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Discard'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PLAYER_OPPO'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PE_LOSE_HAND_CARDS'),
	(1),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Sickness',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_3'),
	'Decrease all your opponent''s creatures'' attack of 1 point'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Sickness'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_OPPO_TEAM'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_CHANGE_ATTACK'),
	(-1),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Epidemic',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_4'),
	'Decrease all your opponent''s creatures'' attack of 2 points'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Epidemic'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_OPPO_TEAM'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_CHANGE_ATTACK'),
	(-1),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Flash heal',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_1'),
	'Increase a random creature''s health of 2 points' -- TODO: Are you sure?
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Flash heal'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_RAND'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_CHANGE_HEALTH'),
	(1),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Robbery',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_5'),
	'Decrease all your opponent''s creatures'' attack of 1 point and increase yours of 1 point'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Robbery'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_OPPO_TEAM'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_CHANGE_ATTACK'),
	(-1),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Robbery'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_TEAM'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_CHANGE_ATTACK'),
	(1),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Grand Theft Card',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_1'),
	'You steal a random card from the opponent''s hand'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Grand Theft Card'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PLAYER_SELF'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PE_STEAL_HAND_CARD'),
	(NULL),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

