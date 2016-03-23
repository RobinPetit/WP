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
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_3')-1,
	'This creature gains 1 HEALTH each turn',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_2')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_8')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_1')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_BLUE')-1
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Tower guard'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_THIS')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_SET_CONSTRAINT')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CC_TURN_HEALTH_CHANGE')-1,
	(1),
	(100),
	(SELECT id FROM ENUM_VALUES WHERE name == 'NO_CASTER_NEEDED')-1,
	(NULL)
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'Diabolic imp',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_2')-1,
	'This creature gains 1 ATTACK each turn',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_3')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_2')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_1')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_NONE')-1
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Diabolic imp'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_THIS')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_SET_CONSTRAINT')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CC_TURN_ATTACK_CHANGE')-1,
	(1),
	(100),
	(SELECT id FROM ENUM_VALUES WHERE name == 'NO_CASTER_NEEDED')-1,
	(NULL)
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'Little Ragnaros',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_4')-1,
	'Each turn, your opponent loses 2 HP',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_4')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_4')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_1')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_LEGENDARY')-1
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Little Ragnaros'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PLAYER_OPPO')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'PE_SET_CONSTRAINT')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'PC_TURN_HEALTH_CHANGE')-1,
	(-2),
	(100),
	(SELECT id FROM ENUM_VALUES WHERE name == 'IF_CASTER_ALIVE')-1,
	(NULL)
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'White wizard',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_7')-1,
	'Blocks the next 2 attacks on your team',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_7')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_7')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_1')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_NONE')-1
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'White wizard'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_TEAM')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_SET_CONSTRAINT')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CC_TEMP_BLOCK_ATTACKS')-1,
	(2),
	(100),
	(SELECT id FROM ENUM_VALUES WHERE name == 'NO_CASTER_NEEDED')-1,
	(NULL)
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'Greedy imp',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_2')-1,
	'Allows you to draw one more card at the beginning of your next turn',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_2')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_2')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_2')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_ORANGE')-1
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Greedy imp'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PLAYER_SELF')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'PE_SET_CONSTRAINT')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'PC_TURN_CARDS_PICKED')-1,
	(2),
	(2),
	(SELECT id FROM ENUM_VALUES WHERE name == 'NO_CASTER_NEEDED')-1,
	(NULL)
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'Unbridled bear',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_3')-1,
	'Takes away 1 shield of each opposite creatures when placed',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_2')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_4')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_1')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_NONE')-1
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Unbridled bear'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_OPPO_TEAM')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_CHANGE_SHIELD')-1,
	(-1),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'Giant soldier',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_9')-1,
	'No effect',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_8')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_8')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_3')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_BLUE')-1
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'White dragon',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_10')-1,
	'All your friendly creatures gain 1 ATTACK each turn',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_9')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_10')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_1')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_NONE')-1
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'White dragon'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_TEAM')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_SET_CONSTRAINT')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CC_TURN_ATTACK_CHANGE')-1,
	(1),
	(100),
	(SELECT id FROM ENUM_VALUES WHERE name == 'IF_CASTER_ALIVE')-1,
	(NULL)
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'Mecha spider',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_3')-1,
	'Reset the attack for a random opponent''s creature',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_4')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_2')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_1')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_ORANGE')-1
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Mecha spider'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_OPPO_RAND')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_RESET_ATTACK')-1,
	(1),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO CreatureCard(name, cost, description,attack,health,shield,shieldType)
VALUES(
	'Mirage of hapiness',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_3')-1,
	'Your opponent can''t play more than 2 spells per turn',
	(SELECT id FROM ENUM_VALUES WHERE name == 'ATTACK_1')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'HEALTH_5')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_3')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'SHIELD_BLUE')-1
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Mirage of hapiness'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PLAYER_OPPO')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'PE_SET_CONSTRAINT')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'PC_TEMP_SPELL_CALL_LIMIT')-1,
	(2),
	(100),
	(SELECT id FROM ENUM_VALUES WHERE name == 'NO_CASTER_NEEDED')-1,
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Abracadabra',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_10')-1,
	'Your opponent loses 2 random cards from his hand'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Abracadabra'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PLAYER_OPPO')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'PE_LOSE_HAND_CARDS')-1,
	(2),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Block',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_9')-1,
	'Block the next 3 attacks for the whole team'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Block'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_TEAM')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_SET_CONSTRAINT')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CC_TEMP_BLOCK_ATTACKS')-1,
	(3),
	(100),
	(SELECT id FROM ENUM_VALUES WHERE name == 'NO_CASTER_NEEDED')-1,
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Care',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_5')-1,
	'Increase a creature''s health of 2 points'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Care'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_INDX')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_CHANGE_HEALTH')-1,
	(2),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Complete Care',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_7')-1,
	'Restore the health point of a creature of your choice'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Complete Care'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_INDX')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_RESET_HEALTH')-1,
	(NULL),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Discard',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_4')-1,
	'Your opponent loses a random card from his hand'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Discard'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PLAYER_OPPO')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'PE_LOSE_HAND_CARDS')-1,
	(1),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Sickness',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_3')-1,
	'Decrease all your opponent''s creatures'' attack of 1 point'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Sickness'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_OPPO_TEAM')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_CHANGE_ATTACK')-1,
	(-1),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Epidemic',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_4')-1,
	'Decrease all your opponent''s creatures'' attack of 2 points'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Epidemic'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_OPPO_TEAM')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_CHANGE_ATTACK')-1,
	(-1),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Flash heal',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_1')-1,
	'Increase a random creature''s health of 2 points' -- TODO: Are you sure?
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Flash heal'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_RAND')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_CHANGE_HEALTH')-1,
	(2),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Robbery',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_5')-1,
	'Decrease all your opponent''s creatures'' attack of 1 point and increase yours of 1 point'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Robbery'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_OPPO_TEAM')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_CHANGE_ATTACK')-1,
	(-1),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Robbery'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'CREATURE_SELF_TEAM')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'CE_CHANGE_ATTACK')-1,
	(1),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

INSERT INTO SpellCard(name, cost, description)
VALUES(
	'Grand Theft Card',
	(SELECT id FROM ENUM_VALUES WHERE name == 'COST_1')-1,
	'You steal a random card from the opponent''s hand'
);

INSERT INTO Effect(owner, parameter0, parameter1, parameter2, parameter3, parameter4, parameter5, parameter6)
VALUES(
	(SELECT id FROM Card WHERE name == 'Grand Theft Card'),
	(SELECT id FROM ENUM_VALUES WHERE name == 'PLAYER_SELF')-1,
	(SELECT id FROM ENUM_VALUES WHERE name == 'PE_STEAL_HAND_CARD')-1,
	(NULL),
	(NULL),
	(NULL),
	(NULL),
	(NULL)
);

