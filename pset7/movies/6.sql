SELECT AVG(rating) AS 'Average Rating for 2012' FROM ratings
JOIN movies ON movies.id = ratings.movie_id
WHERE year = 2012;