-- Keep a log of any SQL queries you execute as you solve the mystery.
-- took place on July 28, 2021 and that it took place on Humphrey Street.
-- see what table I have
.table
--airports              crime_scene_reports   people
--atm_transactions      flights               phone_calls
--bakery_security_logs  interviews
--bank_accounts         passengers


-- looking into crime report
.schema crime_scene_reports

-- resd the description of the crime with what I know with data and position
SELECT *
FROM crime_scene_reports
WHERE day = 28
AND month = 7
AND street = "Humphrey Street"
AND year = 2021;
-- description: Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
--              Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.
--              Littering took place at 16:36. No known witnesses.


-- read interviews
.schema interviews
SELECT name, transcript FROM interviews WHERE day = 28 AND month = 7 AND year = 2021 AND transcript LIKE "%bakery%";
-- Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
--          If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
--------> looking parking lot security footage around 10:15am
-- Eugene  | I don't know the thief's name, but it was someone I recognized.
--          Earlier this morning, before I arrived at Emma's bakery, I was walking by the """ATM on Leggett Street""" and saw the thief there withdrawing some money.
--------> Earlier this morning ATM on Leggett Street
-- Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
--           In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
--          The thief then asked the person on the other end of the phone to purchase the flight ticket.
--------> earliest flight out of Fiftyville tomorrow(2021/07/29). And who bought it is accomplice.


.schema bakery_security_logs
-- looking parking lot security footage from 10:15 to 25 am
-- the thief is someone who has the car with these license_plate number
SELECT id, license_plate, minute
FROM bakery_security_logs
WHERE day = 28
AND month = 7
AND year = 2021
AND activity = 'exit'
AND hour = 10
AND minute BETWEEN 15 AND 25
ORDER BY minute;


-- Earlier this morning ATM on Leggett Street withdraw money
-- the thief's bank account is one of these
.schema atm_transactions
SELECT id, account_number, transaction_type, amount
FROM atm_transactions
WHERE day = 28
AND month = 7
AND year = 2021
AND atm_location = 'Leggett Street'
AND transaction_type = 'withdraw';
-- With bank account try to get person id
.schema bank_accounts
SELECT person_id
FROM bank_accounts
WHERE account_number
IN (SELECT account_number
    FROM atm_transactions
    WHERE day = 28
    AND month = 7
    AND year = 2021
    AND atm_location = 'Leggett Street'
    AND transaction_type = 'withdraw');


-- earliest flight out of Fiftyville tomorrow(2021/07/29). And who bought it is accomplice.
-- find the id of Fiftyville id:8 abbreviation:CSF
.schema airports
SELECT id, abbreviation, full_name
FROM airports
WHERE city = 'Fiftyville';
-- looking for earliest flight take off from Fiftyville on 2021/07/29
.schema flights
SELECT id, hour, minute, destination_airport_id
FROM flights
WHERE day = 29
AND month = 7
AND year = 2021
ORDER BY hour, minute
LIMIT 1;
-- flight id:36 destination_airport_id:4
-- looking for the airport with id 4 to get the city
SELECT id, abbreviation, city, full_name
FROM airports
WHERE id = 4;
-- abbreviation:LGA     city:NEW YORK CITY
-------------------------------------------------------------------- What city the thief escaped to : city:NEW YORK CITY
.schema passengers
-- the thief is the passenger in this flight
SELECT * FROM passengers WHERE flight_id = 36;


-- Last clue is phone_calls
.schema phone_calls
SELECT id, caller, receiver
FROM phone_calls
WHERE day = 28
AND month = 7
AND year = 2021
AND duration < 60;
-- Get the phone numbers that thief in


-- Seeking thief with bank account, passport number and plate license
.schema people
SELECT id, name
FROM people
WHERE passport_number IN
    (SELECT passport_number
    FROM passengers
    WHERE flight_id = 36)
AND license_plate IN
    (SELECT license_plate
    FROM bakery_security_logs
    WHERE day = 28
    AND month = 7
    AND year = 2021
    AND activity = 'exit'
    AND hour = 10
    AND minute BETWEEN 15 AND 25)
AND id IN
    (SELECT person_id
    FROM bank_accounts
    WHERE account_number
    IN (SELECT account_number
        FROM atm_transactions
        WHERE day = 28
        AND month = 7
        AND year = 2021
        AND atm_location = 'Leggett Street'
        AND transaction_type = 'withdraw'))
AND phone_number IN
    (SELECT caller
    FROM phone_calls
    WHERE day = 28
    AND month = 7
    AND year = 2021
    AND duration < 60);
-------------------------------------------------------------------- Done! Bruce is thief


-- Then find out who receive Bruce's call
SELECT name
FROM people
WHERE phone_number IN
    (SELECT receiver
    FROM phone_calls
    WHERE day = 28
    AND month = 7
    AND year = 2021
    AND duration < 60
    AND caller IN
        (SELECT phone_number
        FROM people
        WHERE name = 'Bruce'));
------------------------------------------------------------------Robin is accomplice
