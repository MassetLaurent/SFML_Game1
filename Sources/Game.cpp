#include "Game.h"

//constructors
Game::Game()
{
	this->p_initializeVariables();
	this->p_initWindow();
	this->p_initEnemies();
	this->p_initFonts();
	this->p_initText();
}

Game::~Game()
{
	delete this->p_window;
}



//get
const bool Game::running() const
{
	return this->p_window->isOpen();
}

const bool Game::getEndGame() const
{
	return this->p_endGame;
}

void Game::pollEvents()
{
	while (this->p_window->pollEvent(this->p_ev))
	{
		switch (this->p_ev.type)
		{
		case sf::Event::Closed:
			this->p_window->close();
			break;

		case sf::Event::KeyPressed:
			if (this->p_ev.key.code == sf::Keyboard::Escape)
				this->p_window->close();
			break;
		}
	}
}



//public methodes
void Game::updateMousePos()
{
	/**
		@return void
		updates the mouse position relative to the application windows (vector2i)
	*/

	this->p_mousePosWindow = sf::Mouse::getPosition(*this->p_window);
	this->p_mousePosView = this->p_window->mapPixelToCoords(this->p_mousePosWindow);
}

void Game::updateEnemies()
{
	/**
		@return void
		Updates the enemy spawn timer and spawns enemies
		when the total amout of enemies is smaller than the maximum.
		Moves the enemies downwards.
		Removes the enemies at the edge of the screen
	*/

	//Updating the timer for enemy spawning
	if (this->p_enemies.size() < this->p_maxEnemies)
	{
		if (this->p_enemySpawnTimer >= p_enemySpawnTimerMax)
		{
			//Spawn the enemy and reset the timer
			this->p_spawnEnemy();
			this->p_enemySpawnTimer = 0.f;
		}
		else
			this->p_enemySpawnTimer += 1.f;
	}

	
	//Move and update enemies
	for (int i(0); i < this->p_enemies.size(); i++)
	{

		this->p_enemies[i].move(0.f, 2.f);

		//check if enemies reach the bottom of the screen
		if (this->p_enemies[i].getPosition().y > this->p_window->getSize().y)
		{
			this->p_enemies.erase(this->p_enemies.begin() + i);
			this->p_health -= 1;
			std::cout << "Health : " << this->p_health << "\n";
		}
	}

	//check if mouse click on enemies
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->p_mouseHeld == false)
		{
			this->p_mouseHeld = true;
			bool deleted = false;

			for (size_t i = 0; i < this->p_enemies.size() && deleted == false; i++)
			{
				if (this->p_enemies[i].getGlobalBounds().contains(this->p_mousePosView))
				{
					//Player gain points
					if (this->p_enemies[i].getFillColor() == sf::Color::Magenta)
						this->p_points += 10;
					else if (this->p_enemies[i].getFillColor() == sf::Color::Blue)
						this->p_points += 7;
					else if (this->p_enemies[i].getFillColor() == sf::Color::Cyan)
						this->p_points += 5;
					else if (this->p_enemies[i].getFillColor() == sf::Color::Red)
						this->p_points += 3;
					else if (this->p_enemies[i].getFillColor() == sf::Color::Green)
						this->p_points += 1;


					std::cout << "Points : " << this->p_points << "\n";
					
					//Delete enemy from enemies vector
					deleted = true;
					this->p_enemies.erase(this->p_enemies.begin() + i);
				}
			}
		}
	}
	else
	{
		this->p_mouseHeld = false;
	}
}

void Game::updateText()
{
	std::stringstream ss;

	ss << "Points : " << this->p_points << "\n" << "Health : " << this->p_health;

	this->uiText.setString(ss.str());
}

void Game::update()
{
	this->pollEvents();

	if (this->p_endGame == false)
	{
		this->updateMousePos();
		this->updateText();
		this->updateEnemies();
	}

	if (this->p_health <= 0)
	{
		this->p_endGame = true;
	}
}

void Game::renderEnemies(sf::RenderTarget& target)
{
	//Rendering all the enemies in the vector p_enemies
	for (auto &e : this->p_enemies)
	{
		target.draw(e);
	}
}

void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}

void Game::render()
{
	//clear
	this->p_window->clear();

	//draw
	this->renderEnemies(*this->p_window);
	this->renderText(*this->p_window);

	//show
	this->p_window->display();
}




//private methodes
void Game::p_initializeVariables()
{
	this->p_window = nullptr;

	//Game logic
	this->p_points = 0;
	this->p_enemySpawnTimerMax = 20.f;
	this->p_enemySpawnTimer = this->p_enemySpawnTimerMax;
	this->p_maxEnemies = 5;
	this->p_health = 20;
	this->p_mouseHeld = false;
	this->p_endGame = false;
}

void Game::p_initWindow()
{
	this->p_videoMode.height = 600;
	this->p_videoMode.width = 800;
	
	this->p_window = new sf::RenderWindow(this->p_videoMode, "First SFML Game !", sf::Style::Titlebar | sf::Style::Close);
	this->p_window->setFramerateLimit(60);
}

void Game::p_initFonts()
{
	if (this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
	{
		std::cout << "ERROR::GAME::INITION : Fail to load font !\n";
	}
}

void Game::p_initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(24);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("NONE");
}

void Game::p_initEnemies()
{
	this->p_enemy.setPosition(10.f,10.f);

	this->p_enemy.setSize(sf::Vector2f(100.f, 100.f));
	this->p_enemy.setScale(sf::Vector2f(0.8f,0.8f));

	this->p_enemy.setFillColor(sf::Color::Cyan);
}

void Game::p_spawnEnemy()
{
	/**
		@return void

		Spawn enemies and sets their type, colors and positions.
		- Sets a random position.
		- Sets a random color.
		- Adds enemy to the vector.
	*/

	this->p_enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->p_window->getSize().x - this->p_enemy.getSize().x)),
		0.f);

	//random enemy type
	int type = rand() % 5;
	switch (type)
	{
	case 0:
		this->p_enemy.setSize(sf::Vector2f(10.f, 10.f));
		this->p_enemy.setFillColor(sf::Color::Magenta);
		break;
	case 1:
		this->p_enemy.setSize(sf::Vector2f(30.f, 30.f));
		this->p_enemy.setFillColor(sf::Color::Blue);
		break;
	case 2:
		this->p_enemy.setSize(sf::Vector2f(50.f, 50.f));
		this->p_enemy.setFillColor(sf::Color::Cyan);
		break;
	case 3:
		this->p_enemy.setSize(sf::Vector2f(70.f, 70.f));
		this->p_enemy.setFillColor(sf::Color::Red);
		break;
	case 4:
		this->p_enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->p_enemy.setFillColor(sf::Color::Green);
		break;
	default:
		this->p_enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->p_enemy.setFillColor(sf::Color::Yellow);
		break;
	}



	//Spawn the enemy
	this->p_enemies.push_back(this->p_enemy);

	//Remove the enemy who reach the end of the screen
}
