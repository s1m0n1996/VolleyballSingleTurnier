#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# ******************************************************************************
#   Create: 09.03.2020
#   Author: Simon Flörke
#
#
# Only to fill the Database with random Data
#
# ******************************************************************************

import os
import csv
import sqlite3
import random
import datetime

# ======================================================================================================================
# SETTINGS
# ======================================================================================================================
DATABASE_FILE_PATH = os.path.normpath(
    os.path.join(os.path.dirname(os.path.abspath(__file__)), "../", "../", "new.sqlite"))

CREATE_DATABASE_FIRST = False
CREATE_PLAYERS_FROM_CSV = False

PLAYER_CSV_PATH = os.path.normpath(
    os.path.join(os.path.dirname(os.path.abspath(__file__)), "../", "data/", "players.csv"))

TOURNAMENT_NAME = "Turnier1"
COUNT_OF_PLAYERS = 16  # ust be 2^n compatible


# ======================================================================================================================
# SETTINGS END
# ======================================================================================================================

def sql_query(sql: str):
    connection = sqlite3.connect(DATABASE_FILE_PATH)
    cursor = connection.cursor()

    data_list = cursor.execute(sql).fetchall()
    connection.commit()
    connection.close()
    return data_list


def create_database() -> None:
    ddl_path = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "rsrc", "database",
                                             "DDL_DatabaseSchema.sql"))
    ddl_insert_path = os.path.normpath(
        os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "rsrc", "database",
                     "DDL_DatabaseInserts.sql"))

    with open(ddl_path, "r", encoding="UTF-8") as file:
        statements = file.read().split(";")

        for statement in statements:
            sql_query(statement)

    with open(ddl_insert_path, "r", encoding="UTF-8") as file:
        statements = file.read().split(";")

        for statement in statements:
            sql_query(statement)


def save_players_in_database() -> None:
    players = list()

    with open(PLAYER_CSV_PATH, "r", encoding="UTF-8", newline='') as csvfile:
        csv_reader = csv.reader(csvfile, delimiter=";")
        for name, birthday, country in csv_reader:
            players.append(dict(
                name=name,
                birthday=birthday,
                country=country
            ))

    for player in players:
        sql = f"""
INSERT INTO player_list (name, birthday, country)
VALUES ('{player["name"]}', '{player["birthday"]}', '{player["country"]}')
"""

        sql_query(sql)


def create_tournament(name: str, date: str, count_players: int) -> int:
    sql = "SELECT MAX(id + 1) FROM tournament_list"

    if sql_query(sql)[0][0] is None:
        tournament_id = 1
    else:
        tournament_id = int(sql_query(sql)[0][0])

    sql = f"""
INSERT INTO tournament_list (id, sport_type_id, game_mode_id, name, date) 
VALUES ({tournament_id}, 1, 1, '{name}', '{date}')
"""
    sql_query(sql)

    create_gameboard(tournament_id, count_players)

    return tournament_id


def create_gameboard(tournament_id: int, count_players: int) -> None:
    sql = """
SELECT id FROM player_list ORDER BY id
    """

    raw_data = sql_query(sql)

    player_ids = list()
    for player_id in raw_data:
        player_ids.append(player_id[0])

    random.shuffle(player_ids)
    player_ids = player_ids[:count_players]

    game_board = list()
    i = 0
    for id in player_ids:
        if not i % 2:
            game = list()
            game.append(id)
        else:
            game.append(id)
            game_board.append(game)
        i += 1

    for game_board_id in range(1, count_players):
        sql = f"""
INSERT INTO game_board_list (id, sport_type_id, game_mode_id, tournament_id, game_board_time)
VALUES ({game_board_id}, 1, 1, {tournament_id}, 'TIME')
        """

        sql_query(sql)

    game_board_id = 1

    for player_a, player_b in game_board:
        sql = f"""
UPDATE game_board_list
SET player_a_id = {player_a},
    player_b_id = {player_b}
WHERE id = {game_board_id}
  AND sport_type_id = 1
  AND game_mode_id = 1
  AND tournament_id = {tournament_id}
"""
        sql_query(sql)

        game_board_id += 1


# ======================================================================================================================
# begin the play
# ======================================================================================================================

def play_tournament(tournament_id: int) -> None:
    sql = f"""
SELECT id
FROM game_board_list
WHERE sport_type_id = 1
  AND game_mode_id = 1
  AND tournament_id = {tournament_id}
"""
    data = sql_query(sql)
    games = list()
    for game in data:
        games.append(game[0])

    for game in games:
        play_game(tournament_id, game)


# play a game complete
def play_game(tournament_id: int, game_id: int) -> None:
    sql = f"""
SELECT winner_id, count(winner_id)
FROM leg_list
WHERE winner_id IS NOT NULL
  AND game_board_id = {game_id}
  AND sport_type_id = 1
  AND game_mode_id = 1
  AND tournament_id = {tournament_id}
GROUP BY winner_id
"""
    game_winner_id = -1
    for row in sql_query(sql):
        if row[1] >= 3:
            game_winner_id = row[0]
            break

    while game_winner_id < 0:

        play_leg(tournament_id=tournament_id, game_id=game_id)

        # check winning legs
        for row in sql_query(sql):
            if row[1] >= 3:
                game_winner_id = row[0]
                break

    set_winner_for_game(game_id, tournament_id, game_winner_id)
    print("player: ", game_winner_id, " wins in game: ", game_id)


# play a leg
def play_leg(tournament_id: int, game_id: int) -> None:
    # create leg

    sql = f"""
SELECT max(id + 1)
FROM leg_list
WHERE game_board_id = {game_id}
  AND sport_type_id = 1
  AND game_mode_id = 1
  AND tournament_id = {tournament_id}
"""
    data = sql_query(sql)[0][0]
    leg_id = 1 if data is None else data

    sql = f"""
INSERT INTO leg_list (id, game_board_id, sport_type_id, game_mode_id, tournament_id)
VALUES ({leg_id}, {game_id}, 1, 1, {tournament_id})
"""
    sql_query(sql)

    sql = f"""
SELECT player_a_id, player_b_id
FROM game_board_list
WHERE id = {game_id}
  AND sport_type_id = 1
  AND game_mode_id = 1
  AND tournament_id = {tournament_id}
"""
    players = sql_query(sql)[0]

    is_leg_finished = False

    while not is_leg_finished:

        for player_id in players:
            if is_leg_finished:
                break
            for i in range(3):
                if is_leg_finished:
                    break
                throw_tuple = random_throw()
                write_throw_in_database(
                    tournament_id=tournament_id,
                    game_id=game_id,
                    leg_id=leg_id,
                    player_id=player_id,
                    throw_tuple=throw_tuple
                )

                is_leg_finished = is_leg_won(
                    tournament_id=tournament_id,
                    game_id=game_id,
                    leg_id=leg_id,
                    player_id=player_id,
                )


# frage ob das leg bereits gewonnen ist. wenn ja wird ein True zurückgegeben und der gewinner des Legs eingetragen
def is_leg_won(tournament_id: int, game_id: int, leg_id: int, player_id: int) -> bool:
    sql = f"""
SELECT sum(value * value_type_id)
FROM leg_history_list
WHERE sport_type_id = 1
  AND game_mode_id = 1
  AND game_board_id = {game_id}
  AND tournament_id = {tournament_id}
  AND leg_id = {leg_id}
  AND player_id = {player_id}
"""
    score = sql_query(sql)[0][0]
    is_won = score >= 501

    if not is_won:
        return False

    sql = f"""
UPDATE leg_list
SET winner_id = {player_id}
WHERE id = {leg_id}
  AND game_board_id = {game_id}
  AND sport_type_id = 1
  AND game_mode_id = 1
  AND tournament_id = {tournament_id}
"""
    sql_query(sql)

    return True


# gebe zufälligen wurf zurück
def random_throw() -> tuple():
    valid_throw = False
    while not valid_throw:
        # tuple with (multiplicator, points)
        throw = (random.randrange(1, 4), random.randrange(0, 21))
        return throw


# kontrolliere den wurd auf gültigkeit
def is_throw_valid(tournament_id: int, game_id: int, leg_id: int, player_id: int, throw_tuple: tuple) -> bool:
    sql = f"""
SELECT sum(value * value_type_id)
FROM leg_history_list
WHERE sport_type_id = 1
  AND game_mode_id = 1
  AND tournament_id = {tournament_id}
  AND game_board_id = {game_id}
  AND leg_id = {leg_id}
  AND player_id = {player_id}
"""
    data = sql_query(sql)[0][0]

    if data is None or data < (501 - 181):
        return True

    new_value = data + (throw_tuple[0] * throw_tuple[1])

    if new_value == 501:
        return throw_tuple[0] == 2

    if new_value > 501 - 2:
        return False

    return True


# trage den Wurf in die Datenbank ein, wenn er gültig ist
def write_throw_in_database(tournament_id: int, game_id: int, leg_id: int, player_id: int, throw_tuple: tuple) -> None:
    if not is_throw_valid(tournament_id, game_id, leg_id, player_id, throw_tuple):
        return

    sql = f"""
SELECT max(id + 1)
FROM leg_history_list
WHERE sport_type_id = 1
  AND game_mode_id = 1
  AND tournament_id = {tournament_id}
  AND game_board_id = {game_id}
  AND leg_id = {leg_id}
"""
    data = sql_query(sql)[0][0]
    throw_id = 1 if data is None else data

    str_time = datetime.datetime.now().strftime("%Y-%m-%dT%H:%M:%S.%f")[:-3]

    sql = f"""
INSERT INTO leg_history_list (id, sport_type_id, game_mode_id, tournament_id, game_board_id, leg_id, player_id, time,
                              value_type_id, value)
VALUES ({throw_id}, 1, 1, {tournament_id}, {game_id}, {leg_id}, {player_id}, '{str_time}', {throw_tuple[0]}, {throw_tuple[1]})
"""
    sql_query(sql)


def set_winner_for_game(game_id: int, tournament_id: int, player_id: int) -> None:
    sql = f"""
UPDATE game_board_list
SET winner_id = {player_id}
WHERE id = {game_id}
  AND sport_type_id = 1
  AND game_mode_id = 1
  AND tournament_id = {tournament_id}
"""
    sql_query(sql)

    updated_player = "player_b_id" if game_id % 2 == 0 else "player_a_id"

    sql = f"""
UPDATE game_board_list
SET {updated_player} = {player_id}
WHERE id = (SELECT id
    FROM game_board_list
    WHERE sport_type_id = 1
  AND game_mode_id = 1
  AND tournament_id = {tournament_id}
  AND player_a_id IS NULL
   OR player_b_id IS NULL
    ORDER BY id
    LIMIT 1)
  AND sport_type_id = 1
  AND game_mode_id = 1
  AND tournament_id = {tournament_id}
"""
    sql_query(sql)


if __name__ == '__main__':

    if CREATE_DATABASE_FIRST:
        create_database()
    if CREATE_PLAYERS_FROM_CSV:
        save_players_in_database()

    tournament_id = create_tournament("TOURNAMENT_NAME", "test", COUNT_OF_PLAYERS)
    play_tournament(tournament_id)
