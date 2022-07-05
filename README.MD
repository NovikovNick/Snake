# refactoring + remove redundant args copying + unit test
	1. constructor Snake by map<Coord, Direction>
	2. add RenderService.render(Snake* snake)
	3. Who will call new Snake and delete Snake?
	4. copy constrctor for Snake
	5. Snake::move
	6. Snake::moveAndGain
	7. Snake::isInBound(Coord leftTop, Coord rightBottom)
	8. Snake::isCollide(Coodr coord)
# add check previous finded path
# add ability to increase speed for input with same direction
# backward game state and insert new inputs
# RELEASE 0.1.0
# integrate with GGPO
# logging
# google checkstyle
# SFML
# full description in README
# RELEASE 1.0.0 