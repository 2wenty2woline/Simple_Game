#include "Game.h"

void Game::initVariables()
{
	_window				= nullptr;

													// Game logic
	_endGame			= false;
	_points				= 0;
	_maxEnemies			= 5;
	_enemySpawnTimerMax = 20.f;
	_enemySpawnTimer	= _enemySpawnTimerMax;
	_mouseHeld			= false;
	_health				= 20;
}

void Game::initWindow()
{
	_videoMode.width	= 800;
	_videoMode.height	= 600;
	_window				= new sf::RenderWindow(_videoMode, "Simple Games", sf::Style::Default);

	_window->setFramerateLimit(60);
}

													// Set all fonts
void Game::initFonts()
{
	if (_font.loadFromFile("Fonts/Dosis-Light.ttf"))
	{
		std::cout << "ERROR::GAME::INITFONTS::failed to load fonts!" << std::endl;
	}
}

													// Set the text
void Game::initText()
{
	_uiText.setFont(_font);
	_uiText.setCharacterSize(24);
	_uiText.setFillColor(sf::Color::White);
	_uiText.setString("NONE");
}

void Game::initEnemies()
{
	this->enemy.setPosition(10.f, 10.f);
	this->enemy.setSize(sf::Vector2f(100.f, 100.f));
													//this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
	this->enemy.setFillColor(sf::Color::Cyan);
}

													// Constructor
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initFonts();
	this->initText();
	this->initEnemies();
}

Game::~Game()
{
	delete _window;
}

													// Accessor
const bool Game::getWindowIsOpen() const
{
	return _window->isOpen();
}

const bool Game::getEndGame() const
{
	return _endGame;
}

													// Enemies
void Game::spawnEnemy()
{
													/*
														Spawn ennemies and set their types and colors.
														Spawns them at random positions.
														- Set a random type.
														- Set a random position.
														- Set a random color.
														- Add enemy to the vector.
													*/

	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>((_window->getSize().x - this->enemy.getSize().x))),
		0.f
	);

													// Randomize enemy type
	int type = rand() % 5;

	switch (type)
	{
	case 0:
		this->enemy.setSize(sf::Vector2f(10.f, 10.f));
		this->enemy.setFillColor(sf::Color::Magenta);
		break;
	case 1:
		this->enemy.setSize(sf::Vector2f(30.f, 30.f));
		this->enemy.setFillColor(sf::Color::Blue);
		break;
	case 2:
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		this->enemy.setFillColor(sf::Color::Cyan);
	case 3:
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		this->enemy.setFillColor(sf::Color::Green);
		break;
	case 4:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Red);
		break;
	default:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Yellow);
		break;
	}

													// Spawn this enemy
	this->enemies.push_back(this->enemy);

}

void Game::pollEvents()
{
	while (_window->pollEvent(_event))
	{
		switch (_event.type)
		{
		case sf::Event::Closed:
			_window->close();
			break;

		case sf::Event::KeyPressed:
			if (_event.key.code == sf::Keyboard::Escape)
				_window->close();
			break;
		}
	}
}

													// Update mouse position
void Game::updateMousePositions()
{
													/*
														Update the mouse position:
														- Mouse position relative to window (Vector2i)
													*/
	_mousePosWindow = sf::Mouse::getPosition(*_window);
	_mousePosView = _window->mapPixelToCoords(_mousePosWindow);
}

													// Update the Text
void Game::updateText()
{
	std::stringstream ss;

	ss << "Points: " << _points << std::endl
		<< "Health: " << _health << std::endl;

	_uiText.setString(ss.str());
}

													// Update timer for enemy spawn
void Game::updateEnemies()
{
													/*
														Updates the enemy spawn timer
														when the total amount is smaller then the max.
														Move the enemies.
														Removes the enemies at the edge of the screen.
													*/
	if (this->enemies.size() < _maxEnemies)
	{
		if (_enemySpawnTimer >= _enemySpawnTimerMax)
		{
													// Spawn enemy reset the timer
			this->spawnEnemy();
			_enemySpawnTimer = 0.f;
		}
		else
			_enemySpawnTimer += 1.f;
	}

													// Move the enemies
	for (int i = 0; i < this->enemies.size(); ++i)
	{
		bool deleted = false;

		this->enemies[i].move(0.f, 3.f);

		if (this->enemies[i].getPosition().y > _window->getSize().y)
		{
			this->enemies.erase(this->enemies.begin() + i);
			_health -= 1;
			std::cout << "Health: " << _health << std::endl;
		}
	}

													// Check if clicked upon
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
													// Mouse button is not held down
		if (_mouseHeld == false)
		{
			_mouseHeld = true;
			bool deleted = false;

			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(_mousePosView))
				{
													// Gain points
					if (this->enemies[i].getFillColor() == sf::Color::Magenta)
						_points += 10;
					else if (this->enemies[i].getFillColor() == sf::Color::Blue)
						_points += 7;
					else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
						_points += 5;
					else if (this->enemies[i].getFillColor() == sf::Color::Green)
						_points += 3;
					else if (this->enemies[i].getFillColor() == sf::Color::Red)
						_points += 1;

					std::cout << "Points: " << _points << std::endl;

													// Delete the enemy
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
	}
	else
	{
		_mouseHeld = false;
	}
}

void Game::update()
{
	this->pollEvents();

	if (!_endGame)
	{
		this->updateMousePositions();

		this->updateText();

		this->updateEnemies();
	}

													// End game if our health is 0 or less
	if (_health <= 0)
	{
		_endGame = true;
	}
}

													// Render Text
void Game::renderText(sf::RenderTarget& target)
{
	target.draw(_uiText);
}

void Game::renderEnemies(sf::RenderTarget& target)
{
													// Move the enemies
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
}

void Game::render()
{
													/*
														- clear frame
														- render objects
														- display window
													*/
	_window->clear(sf::Color(0, 0, 0, 255));

													// Draw game Objects
	this->renderEnemies(*_window);

	this->renderText(*_window);

	_window->display();
}
