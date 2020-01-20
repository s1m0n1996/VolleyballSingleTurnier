--######################################################################################################################
-- insert sport_type darts
--######################################################################################################################

INSERT INTO sport_type (id, name)
VALUES (1, 'Darts');

INSERT INTO game_mode_type (id, sport_type_id, name, description)
VALUES (1, 1, '3 Leg: 501 - Double Out', 'This is the WM-Mode');

INSERT INTO history_value_type (id, sport_type_id, name, description)
VALUES (0, 1, 'miss', ''),
       (1, 1, 'single', ''),
       (2, 1, 'double', ''),
       (3, 1, 'triple', '');
