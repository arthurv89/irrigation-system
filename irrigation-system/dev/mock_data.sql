DELETE FROM open_times WHERE 1=1;

INSERT INTO open_times(id, hose_id, time, opening_time)
VALUES ("0", "h1", NOW(), 10);

INSERT INTO open_times(id, hose_id, time, opening_time)
VALUES ("1", "h1", '2020-03-28 18:00:00', 10);

INSERT INTO open_times(id, hose_id, time, opening_time)
VALUES ("2", "h1", '2020-03-28 18:10:00', 10);

INSERT INTO open_times(id, hose_id, time, opening_time)
VALUES ("3", "h1", '2020-03-28 18:11:00', 10);

INSERT INTO open_times(id, hose_id, time, opening_time)
VALUES ("4", "h1", '2020-03-28 18:15:00', 10);

INSERT INTO open_times(id, hose_id, time, opening_time)
VALUES ("5", "h2", '2020-03-28 18:00:00', 10);

INSERT INTO open_times(id, hose_id, time, opening_time)
VALUES ("6", "h2", '2020-03-28 18:10:00', 10);

INSERT INTO open_times(id, hose_id, time, opening_time)
VALUES ("7", "h3", '2020-03-28 16:00:00', 10);



SELECT hose_id
FROM open_times
WHERE time < NOW() - INTERVAL 20 MINUTE
GROUP BY hose_id;


-- #######################################


DELETE FROM valve_connector WHERE 1=1;

INSERT INTO valve_connector(id, valve_id, hose_id, hose_position)
VALUES ("0", "00029759903296627115", "12053270542092411048", 0);

INSERT INTO valve_connector(id, valve_id, hose_id, hose_position)
VALUES ("1", "00029759903296627115", "12053270542092411049", 1);

INSERT INTO valve_connector(id, valve_id, hose_id, hose_position)
VALUES ("2", "00029759903296627115", "12053270542092411050", 2);




--
