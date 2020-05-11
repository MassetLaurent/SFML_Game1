#include <iostream>
#include"Game.h"

int main()
{
	//Init Srand()
	std::srand(static_cast<unsigned>(time(NULL)));

	//Init game
	Game game;
	
	//Game loop
	while (game.running() && !game.getEndGame())
	{
		//update
		game.update();
		//Render
		game.render();
	}
	   
	//End of application
	return 0;
}