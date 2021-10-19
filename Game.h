#pragma once

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
private:
															/// Variables
															// Window
	sf::RenderWindow*               _window;
	sf::VideoMode                   _videoMode;
	sf::Event                       _event;

															// Mouse position
	sf::Vector2i                    _mousePosWindow;
	sf::Vector2f                    _mousePosView;

															// Resources
	sf::Font                        _font;

															// Text variables
	sf::Text                        _uiText;

															// Game logic
	bool                            _endGame;
	unsigned                        _points;
	int                             _health;
	int                             _maxEnemies;
	float                           _enemySpawnTimer;
	float                           _enemySpawnTimerMax;
	bool                            _mouseHeld;


															// Game objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape              enemy;

															// Initialized function
	void                            initVariables();
	void                            initWindow();
	void                            initFonts();
	void                            initText();
	void                            initEnemies();

public:
															// Constructor & Destructor
	Game();

	virtual ~Game();

															// Accessor
	const bool                      getWindowIsOpen() const;
	const bool                      getEndGame() const;

															// Function
	void                            spawnEnemy();

	void                            pollEvents();
	void                            updateMousePositions();
	void                            updateText();
	void                            updateEnemies();
	void                            update();

	void                            renderText(sf::RenderTarget& target);
	void                            renderEnemies(sf::RenderTarget& target);
	void                            render();
};