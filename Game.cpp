#include "Game.h"

#include <iostream>
#include<fstream>

Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
	// TODO: read in config file here
	//       use the premade PlayerConfig, EnemyConfig, BulletConfig variables

	std::ifstream fin(path);
	//fin >> m_playerConfig.SR >> m_playerConfig.CR

	// Set up default window parameters
	m_window.create(sf::VideoMode(1280, 720), "Assignment 2");
	m_window.setFramerateLimit(60);

	spawnPlayer();
}

void Game::run()
{
	// TODO: add pause functionality in here
	//       some systems should function while paused (rendering)
	//       some systems shouldn't (movement/input)

	while (m_running)
	{
		m_entities.update();

		sEnemySpawner();
		sMovement();
		sCollision();
		sUserInput();
		sRender();

		// increment the current frame
		// may need to be moved when pause implemented
		m_currentFrame++;
	}
}

void Game::setPaused(bool paused)
{
	// TODO
}

void Game::spawnPlayer()
{
	// TODO:: Finish adding all properties of the player with the correct values from the config

	// We create every entity by calling EntityManager.addEntity(tag)
	// This returns a std::shared_ptr<Entity>, so we use 'auto' to save typing
	auto entity = m_entities.addEntity("player");

	// Give this entity a Transform so it spawns at (200,200) with velocity (1,1) and angle 0
	float mx = m_window.getSize().x / 2.0f;
	float my = m_window.getSize().y / 2.0f;

	entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(1.0f, 1.0f), 
		0.0f);

	// The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
	entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), 
		sf::Color(255, 0, 0), 4.0f);

	// Add an input component to the player so that we can use inputs
	entity->cInput = std::make_shared<CInput>();

	// Since we want this Entity to be our player, set our Game's player variable to this Entity
	// This goes slightly against the EntityManager paradigm, but we use the player so myuch it's worth it
	m_player = entity;
}

// spawn an enemy at a random position
void Game::spawnEnemy()
{
	// TODO: make sure the enemy is spawned properly with the m_enemyconfig variables
	//       the enemy must be spawned completely within the bounds of the window

	auto entity = m_entities.addEntity("enemy");

	// Give this entity a Transform so it spawns at (200,200) with velocity (1,1) and angle 0
	float ex = static_cast<float>(rand() % m_window.getSize().x);
	float ey = static_cast<float> (rand() % m_window.getSize().y);


	entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(0.0f, 0.0f),
		0.0f);

	// The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
	entity->cShape = std::make_shared<CShape>(16.0f, 3, sf::Color(0, 0, 255),
		sf::Color(255, 255, 255), 4.0f);

	// Add an input component to the player so that we can use inputs
	entity->cInput = std::make_shared<CInput>();


	// record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::sCollision()
{
	// TODO: implememt all proper collisions between entities
	//       be sure to use the collision radius, NOT the shape radius
}

void Game::sEnemySpawner()
{
	// TODO: code which implements enemy spawning should go here
	//       (use m_currentFrame - m_lastEnemySpawnTime) to determine
	//       how long it has been since the last enemy spawned
	spawnEnemy();
}

void Game::sRender()
{
	// TODO: change the code below to draw ALL of the entities
	//       sample drawing of the player Entity that we have created

	m_window.clear();

	// set the positio of the shape based on the entity's transform->pos
	m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);

	// set the rotation of the shap based on the entity's transform->angle
	m_player->cTransform->angle += 1.0f;
	m_player->cShape->circle.setRotation(m_player->cTransform->angle);

	// draw the entity's sf::CircleShape
	m_window.draw(m_player->cShape->circle);

	std::cout << m_entities.getEntities().size() << "\n";

	for (const auto& e : m_entities.getEntities())
	{
		// set the position of the shape based on the entity's transform->pos
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

		// set the rotation of the shape based on the entity's transform->angle
		e->cTransform->angle += 1.0f;
		e->cShape->circle.setRotation(e->cTransform->angle);

	//	// draw the entity's sf::CircleShape
		m_window.draw(e->cShape->circle);
	}



	m_window.display();
}

void Game::sLifespan()
{

}

void Game::sMovement()
{
	// TODO: implememt all entity movement in this function
	//       you should read the m_player->cInput component to determine if the player is moving

	m_player->cTransform->velocity = { 0,0 };
	// implememt player movement
	if (m_player->cInput->up)
	{
		m_player->cTransform->velocity.y = -5;
	}

	// sample movement speed update
	 m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	 m_player->cTransform->pos.y += m_player->cTransform->velocity.y;

}

void Game::sUserInput()
{
	// TODO: handle user input here
	//       note that you should only be setting the player's input component variables here
	//       you should no implement the player's movement logic here
	//       the movement system will read the variables you set in this function
	
	sf::Event event;;
	while (m_window.pollEvent(event))
	{
		// this event triggers when the window is closed
		if (event.type ==  sf::Event::Closed)
		{
			m_running = false;
		}

		// this event is triggered when a key is pressed
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code) 
			{
			case sf::Keyboard::W:
				std::cout << "W key predded \n";
				// TODO: set player's input component "up" to true
			break;

			default: break;
			}
		}

		// this event is triggered when a key is released
		if (event.type == sf::Event::KeyReleased)
		{
			switch(event.key.code)
			{
			case sf::Keyboard::W:
				break;


			default:
				break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left:
				std::cout << "Left Mouse Button Clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")\n";
				// call spawn bullet here
			break;

			case sf::Mouse::Right:
				std::cout << "Right Mouse Button Clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")\n";
				// call spawn bullet here
			break;

			default:
				break;
			}

		}
	}
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	// TODO: spawn small enemies at the location of the input enemy e

	// When we create the smaller enemy, we have to read the values of the original enemy
	// - spawn a number od small enemies equal to the vertices of the original enemy
	// - set each small enemy to the same color as the original, half the size
	// - small enemies are worth double points of the original enemy
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
	// TODO: implement the spawning of a bullet which travels towad target
	//       - bullet speed is given as a scalar speed
	//       - you must set the velocity by using formula in notes
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity) 
{
	// TODO: implement your own special weapon
}

