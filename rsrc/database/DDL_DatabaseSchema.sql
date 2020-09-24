-- PRAGMA foreign_keys = ON

-- #####################################################################################################################
-- create player_pictures_list table
-- #####################################################################################################################
--
-- in this table are all pictures for the players
--
CREATE TABLE player_pictures_list
(
    id      INTEGER NOT NULL
        CONSTRAINT player_pictures_pk
            PRIMARY KEY AUTOINCREMENT,
    picture BLOB    NOT NULL
);

CREATE INDEX player_pictures_id_uindex
    ON player_pictures_list (id);


-- #####################################################################################################################
-- create player_list table
-- #####################################################################################################################
--
-- in this table are all players. You can't delete a player from the tale. You can only set the is_available flag to 0.
--
CREATE TABLE player_list
(
    id           INTEGER NOT NULL
        CONSTRAINT player_list_pk
            PRIMARY KEY AUTOINCREMENT,
    name         TEXT    NOT NULL,
    birthday     TEXT,
    country      TEXT,
    gender       TEXT,
    is_available INT     DEFAULT 1 NOT NULL,
    picture_id   INTEGER DEFAULT 1 NOT NULL
        REFERENCES player_pictures_list,
    CONSTRAINT player_list_pk_2
        UNIQUE (name, birthday, country)
);

-- #####################################################################################################################
-- create sport_type table
-- #####################################################################################################################
--
-- on this table you defines sport_types like darts, football, handball, volleyball, ...
--
CREATE TABLE sport_type
(
    id   INTEGER NOT NULL,
    name TEXT    NOT NULL,
    CONSTRAINT sport_type_pk
        PRIMARY KEY (id)
);

-- #####################################################################################################################
-- create game_mode_type table
-- #####################################################################################################################
--
-- on this table you sport_types for every sport_type from table sport_type.
-- For Example darts hab the game_mode 3 Leg that would played on the WM.
--
CREATE TABLE game_mode_type
(
    id            INTEGER NOT NULL,
    sport_type_id INTEGER NOT NULL,
    name          TEXT,
    description   TEXT,
    CONSTRAINT game_mode_type_pk
        PRIMARY KEY (sport_type_id, id),
    CONSTRAINT game_mode_type_sport_type_fk
        FOREIGN KEY (sport_type_id) REFERENCES sport_type (id)
);

-- #####################################################################################################################
-- create history_value_type table
-- #####################################################################################################################
--
-- on this table you can define variables for every sport_type for the leg_history.
-- for example darts have 4 values 0-miss, 1-normal, 2-double, 3-triple
--
CREATE TABLE history_value_type
(
    id            INTEGER NOT NULL,
    sport_type_id INTEGER NOT NULL,
    name          TEXT    NOT NULL,
    description   TEXT,
    CONSTRAINT history_value_type_pk
        PRIMARY KEY (id, sport_type_id),
    CONSTRAINT history_value_type_sport_type_fk
        FOREIGN KEY (sport_type_id) REFERENCES sport_type (id)
);

-- #####################################################################################################################
-- create tournament_list table
-- #####################################################################################################################
--
-- on this table you can add a tournament.
--
CREATE TABLE tournament_list
(
    id            INTEGER NOT NULL,
    sport_type_id INTEGER NOT NULL,
    game_mode_id  INTEGER NOT NULL,
    name          TEXT    NOT NULL,
    date          TEXT    NOT NULL,
    CONSTRAINT tournament_list_pk
        PRIMARY KEY (id, sport_type_id, game_mode_id),
    CONSTRAINT tournament_list_game_mode_type_sport_type_id_game_mode_id_sport_type_id_fk
        FOREIGN KEY (sport_type_id, game_mode_id) REFERENCES game_mode_type (sport_type_id, id),
    CONSTRAINT tournament_list_pk2
        UNIQUE (sport_type_id, game_mode_id, name, date)

);

-- #####################################################################################################################
-- create game_board_list table
-- #####################################################################################################################
--
-- on this table the game board is saved.
--
CREATE TABLE game_board_list
(
    id              INTEGER NOT NULL,
    sport_type_id   INTEGER NOT NULL,
    game_mode_id    INTEGER NOT NULL,
    tournament_id   INTEGER NOT NULL,
    game_board_time TEXT    NOT NULL,
    player_a_id     INTEGER, -- player = team
    player_b_id     INTEGER,
    winner_id       INTEGER,
    CONSTRAINT game_board_list_pk
        PRIMARY KEY (id, sport_type_id, game_mode_id, tournament_id),
    CONSTRAINT game_board_list_tournament_list_tournament_id_sport_type_id_game_mode_id_fk
        FOREIGN KEY (sport_type_id, game_mode_id, tournament_id) REFERENCES tournament_list (sport_type_id, game_mode_id, id),
    CONSTRAINT game_board_list_player_list_fk
        FOREIGN KEY (player_a_id, player_b_id, winner_id) REFERENCES player_list (id, id, id)
);

-- #####################################################################################################################
-- create game_player table
-- #####################################################################################################################
--
-- on this table are all game players for every game from the game_board.
--
CREATE TABLE game_player_list
(
    id            INTEGER NOT NULL,
    game_board_id INTEGER NOT NULL,
    sport_type_id INTEGER NOT NULL,
    game_mode_id  INTEGER NOT NULL,
    tournament_id INTEGER NOT NULL,
    player_id     INTEGER,
    team_id       INTEGER, -- only 0, 1 for different teams
    CONSTRAINT leg_list_pk
        PRIMARY KEY (id, game_board_id, sport_type_id, game_mode_id, tournament_id),
    CONSTRAINT leg_list_game_board_list_fk
        FOREIGN KEY (game_board_id, sport_type_id, game_mode_id, tournament_id) REFERENCES game_board_list (id, sport_type_id, game_mode_id, tournament_id),
    CONSTRAINT leg_list_player_list
        FOREIGN KEY (player_id) REFERENCES player_list (id)
);

-- #####################################################################################################################
-- create leg_list table
-- #####################################################################################################################
--
-- on this table are all legs for every game from the game_board.
--
CREATE TABLE leg_list
(
    id            INTEGER NOT NULL,
    game_board_id INTEGER NOT NULL,
    sport_type_id INTEGER NOT NULL,
    game_mode_id  INTEGER NOT NULL,
    tournament_id INTEGER NOT NULL,
    winner_id     INTEGER,
    CONSTRAINT leg_list_pk
        PRIMARY KEY (id, game_board_id, sport_type_id, game_mode_id, tournament_id),
    CONSTRAINT leg_list_game_board_list_fk
        FOREIGN KEY (game_board_id, sport_type_id, game_mode_id, tournament_id) REFERENCES game_board_list (id, sport_type_id, game_mode_id, tournament_id),
    CONSTRAINT leg_list_player_list
        FOREIGN KEY (winner_id) REFERENCES player_list (id)
);

-- #####################################################################################################################
-- create leg_history_list table
-- #####################################################################################################################
--
-- on this table is the complete history.
--
CREATE TABLE leg_history_list
(
    id            INTEGER NOT NULL,
    sport_type_id INTEGER NOT NULL,
    game_mode_id  INTEGER NOT NULL,
    tournament_id INTEGER NOT NULL,
    game_board_id INTEGER NOT NULL,
    leg_id        INTEGER NOT NULL,
    player_id     INTEGER NOT NULL,
    time          TEXT    NOT NULL,
    value_type_id INTEGER NOT NULL,
    value         INTEGER NOT NULL,
    CONSTRAINT history_list_pk
        PRIMARY KEY (id, sport_type_id, game_mode_id, tournament_id, game_board_id, leg_id),
    CONSTRAINT leg_history_list_leg_list_fk
        FOREIGN KEY (sport_type_id, game_mode_id, tournament_id, game_board_id, leg_id) REFERENCES leg_list (sport_type_id, game_mode_id, tournament_id, game_board_id, id),
    CONSTRAINT leg_history_list_history_value_type_fk
        FOREIGN KEY (value_type_id, sport_type_id) REFERENCES history_value_type (id, sport_type_id),
    CONSTRAINT leg_history_list_player_list_fk
        FOREIGN KEY (player_id) REFERENCES player_list (id)
);

-- #####################################################################################################################
-- create tournament_players_list table
-- #####################################################################################################################
--
-- on this table are the players for the next tournier. So you can select and store players without start a game.
--
CREATE TABLE tournament_players_list
(
    player_id     INTEGER NOT NULL
        REFERENCES player_list,
    sport_type_id INTEGER NOT NULL,
    game_mode_id  INTEGER NOT NULL,
    tournament_id INTEGER NOT NULL,
    CONSTRAINT tournament_players_list_pk
        PRIMARY KEY (player_id, sport_type_id, game_mode_id, tournament_id),
    CONSTRAINT tournament_players_list_tournament_list_sport_type_id_game_mode_id_id_fk
        FOREIGN KEY (sport_type_id, game_mode_id, tournament_id) REFERENCES tournament_list (sport_type_id, game_mode_id, id)
);

