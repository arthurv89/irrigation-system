DELETE FROM open_times WHERE 1=1;

INSERT INTO open_times(id, valve_id, time, opening_time)
VALUES ("0", "h1", NOW(), 10);

INSERT INTO open_times(id, valve_id, time, opening_time)
VALUES ("1", "h1", '2020-03-28 18:00:00', 10);

INSERT INTO open_times(id, valve_id, time, opening_time)
VALUES ("2", "h1", '2020-03-28 18:10:00', 10);

INSERT INTO open_times(id, valve_id, time, opening_time)
VALUES ("3", "h1", '2020-03-28 18:11:00', 10);

INSERT INTO open_times(id, valve_id, time, opening_time)
VALUES ("4", "h1", '2020-03-28 18:15:00', 10);

INSERT INTO open_times(id, valve_id, time, opening_time)
VALUES ("5", "h2", '2020-03-28 18:00:00', 10);

INSERT INTO open_times(id, valve_id, time, opening_time)
VALUES ("6", "h2", '2020-03-28 18:10:00', 10);

INSERT INTO open_times(id, valve_id, time, opening_time)
VALUES ("7", "h3", '2020-03-28 16:00:00', 10);



SELECT valve_id
FROM open_times
WHERE time < NOW() - INTERVAL 20 MINUTE
GROUP BY valve_id;


-- #######################################


DELETE FROM valve_box WHERE 1=1;

INSERT INTO valve_box(id, valve_box_id, valve_id, valve_position)
VALUES
("0", "37961240619157332318", "12053270542092411048", 0),
("1", "37961240619157332318", "12053270542092411049", 1),
("2", "37961240619157332318", "12053270542092411050", 2);




--
