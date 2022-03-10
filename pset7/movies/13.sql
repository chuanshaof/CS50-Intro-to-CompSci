SELECT name FROM people JOIN
(SELECT DISTINCT person_id FROM stars
WHERE movie_id IN
(SELECT movie_id FROM stars
JOIN people ON stars.person_id = people.id
WHERE name = 'Kevin Bacon' AND birth = 1958))
ON people.id = person_id
WHERE name IS NOT 'Kevin Bacon';
