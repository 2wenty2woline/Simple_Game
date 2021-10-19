#include <iostream>
#include "Game.h"

int main()
{
														// Init srand
	std::srand(static_cast<unsigned>(time(NULL)));

														/// Initialized game
	Game game;
														/// Game loop
	while (game.getWindowIsOpen() && !game.getEndGame())
	{
														/// Update ///
		game.update();

														/// Render ///
		game.render();

	}
	return 0;
}