SELECT movies.title, ratings.rating
FROM ratings
INNER JOIN movies ON ratings.movie_id=movies.id
WHERE movie_id IN (SELECT id FROM movies WHERE year = 2010)
ORDER BY ratings.rating DESC, movies.title