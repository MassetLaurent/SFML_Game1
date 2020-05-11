#ifndef DEF_GAME
#define DEF_GAME

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class Game
{
public:
	//constructors
	Game();
	virtual ~Game();


	//get
	const bool running() const;
	const bool getEndGame() const;
	void pollEvents();


	//set


	//methodes
	void updateMousePos();
	void updateEnemies();
	void updateText();
	void update();

	void renderEnemies(sf::RenderTarget& target);
	void renderText(sf::RenderTarget& target);
	void render();


private:
	//attributes
	sf::RenderWindow* p_window;
	sf::Event p_ev;
	sf::VideoMode p_videoMode;


	//mouse position
	sf::Vector2i p_mousePosWindow;
	sf::Vector2f p_mousePosView;


	//Ressources
	sf::Font font;


	//Text
	sf::Text uiText;


	//Game logic
	unsigned p_points;
	float p_enemySpawnTimer;
	float p_enemySpawnTimerMax;
	int p_maxEnemies;
	int p_health;
	bool p_mouseHeld;
	bool p_endGame;
	

	//Game objects
	std::vector<sf::RectangleShape> p_enemies;
	sf::RectangleShape p_enemy;

	
	//p_methodes
	void p_initializeVariables();
	void p_initWindow();
	void p_initFonts();
	void p_initText();
	void p_initEnemies();
	void p_spawnEnemy();
};

#endif // !DEF_GAME

