#include "Game.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm> // for std::transform
#include <cctype>    // for std::tolower
#include <cmath>

#define M_PI 3.14159265358979323846

Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
	// TODO: read in config file here
	//       use the premade PlayerConfig, EnemyConfig, BulletConfig variables

	std::ifstream fin;



	fin.open(path);
	if (!fin.is_open()) {
		std::cerr << "Error opening file" << std::endl;
	}

	std::string key;
	while (fin >> key)
	{
		if (key == "Window")
		{
			fin >> m_windowConfig.W >> m_windowConfig.H >> m_windowConfig.FL >> m_windowConfig.FS;
		}
		else if (key == "Font")
		{
			fin >> m_fontConfig.F >> m_fontConfig.S >> m_fontConfig.R >> m_fontConfig.G >> m_fontConfig.B;
		}
		else if (key == "Player")
		{
			fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB
				>> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V;
		}
		else if (key == "Enemy")
		{
			fin >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX >> m_enemyConfig.OR >> m_enemyConfig.OG >> m_enemyConfig.OB
				>> m_enemyConfig.OT >> m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SP;
		}
		else if (key == "Bullet")
		{
			fin >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >> m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB
				>> m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >> m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
		}
	}

	fin.close();

	// Load font
	if(!m_font.loadFromFile(m_fontConfig.F))
	{
		std::cerr << "Failed to load font!" << "\n";
	}

	m_text.setFont(m_font);
	m_text.setCharacterSize(m_fontConfig.S);
	m_text.setFillColor(sf::Color(m_fontConfig.R, m_fontConfig.G, m_fontConfig.B));


	// Set up default window parameters
	m_window.create(sf::VideoMode(m_windowConfig.W, m_windowConfig.H), "Assignment 2");
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
		if (!m_paused)
		{
			m_entities.update();
			sEnemySpawner();
			sMovement();
			sCollision();
			sLifespan();
		}
			sUserInput();

		
		sRender();

		if (m_isOnCoolDown)
		{
			m_coolDownTimer -= m_currentFrame;
			if (m_coolDownTimer <= 0)
			{
				m_isOnCoolDown = false;
				m_coolDownTimer = 0; 
			}
		}

		// increment the current frame
		// may need to be moved when pause implemented
		m_currentFrame++;
	}
}

void Game::setPaused(bool paused)
{
	m_paused = !paused;
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
	entity->cShape = std::make_shared<CShape>(m_playerConfig.SR,m_playerConfig.V, 
		sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB), 
		sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),
		m_playerConfig.OT);
	// Set players collision radius
	entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);

	// Add an input component to the player so that we can use inputs
	entity->cInput = std::make_shared<CInput>();

	// Since we want this Entity to be our player, set our Game's player variable to this Entity
	// This goes slightly against the EntityManager paradigm, but we use the player so much it's worth it
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

	// Generate random velocity components within a range
	float vx = -2.0f + static_cast<float>(rand() % 5); // Random velocity between -2 and 2 in x-direction
	float vy = -2.0f + static_cast<float>(rand() % 5); // Random velocity between -2 and 2 in y-direction

	entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(vx, vy),
		0.0f);

	// The entity's shape will have radius 32, 8 sides, dark grey fill, and red outline of thickness 4
	float sides = m_enemyConfig.VMIN + rand() % m_enemyConfig.VMAX;
	float R = 0 + rand() % 255;
	float G = 0 + rand() % 255;
	float B = 0 + rand() % 255;

	entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, sides, 
		sf::Color(R, G, B),
		sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), 
		m_enemyConfig.OT);


	// Add an input component to the player so that we can use inputs
	entity->cInput = std::make_shared<CInput>();
	// Add a lifespan component
	entity->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
	// Set an entity's colloission
	entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
	// Set an entity's score
	int initialScore = sides * 100;
	entity->cScore = std::make_shared<CScore>(initialScore);


	// record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::sCollision()
{
	float windowHeight = m_window.getSize().y;
	float windowWidth = m_window.getSize().x;

	// Entities going out of bounds
	for (auto& e : m_entities.getEntities())
	{
		entityOutOfBounds(e, windowHeight, windowWidth);
	}

	for (auto& e : m_entities.getEntities("enemy"))
	{
		for (auto& p : m_entities.getEntities("player"))
		{
			if (checkCollision(e,p))
			{
				p->destroy();
				m_score = 0;
				spawnPlayer();

			}
		}
	}

	// Bullet hits enemy
	for (auto& b : m_entities.getEntities("bullet"))
	{
		for (auto& e : m_entities.getEntities("enemy"))
		{
			if(checkCollision(b, e))
			{
				m_score = m_score + e->cScore->score;
				spawnSmallEnemies(e);
				e->destroy();
			}
		}
	}

	// special bullet hits enemy
	for (auto& b : m_entities.getEntities("specialbullet"))
	{
		for (auto& e : m_entities.getEntities("enemy"))
		{
			if (checkCollision(b, e))
			{
				m_score = m_score + e->cScore->score;
				spawnSmallEnemies(e);
				e->destroy();
			}
		}
	}
	
}

void Game::entityOutOfBounds(const std::shared_ptr<Entity>& e, float windowHeight, float windowWidth)
{

	float radius = e->cShape->circle.getRadius();
	float yMax = windowHeight - radius;
	float yMin = radius;
	float xMax = windowWidth - radius;
	float xMin = radius;

	Vec2 position = e->cTransform->pos;
	Vec2 velocity = e->cTransform->velocity;

	if (position.x < xMin)
	{
		if (e->getTag() == "enemy" || e->getTag() == "smallenemy")
		{
			velocity.x *= -1;
		}
		position.x = xMin;
	}
	else if (position.x > xMax)
	{
		if (e->getTag() == "enemy" || e->getTag() == "smallenemy")
		{
			velocity.x *= -1;
		}
		position.x = xMax;
	}

	if (position.y < yMin)
	{
		if (e->getTag() == "enemy" || e->getTag() == "smallenemy")
		{
			velocity.y *= -1;
		}
		position.y = yMin;
	}
	else if (position.y > yMax)
	{
		if (e->getTag() == "enemy" || e->getTag() == "smallenemy")
		{
			velocity.y *= -1;
		}
		position.y = yMax;
	}

	e->cTransform->pos = position;
	e->cTransform->velocity = velocity;
}

void Game::sEnemySpawner()
{
	// TODO: code which implements enemy spawning should go here
	//       (use m_currentFrame - m_lastEnemySpawnTime) to determine
	//       how long it has been since the last enemy spawned

	int durationPast = m_currentFrame - m_lastEnemySpawnTime;


	if (durationPast > 60)
	{
		spawnEnemy();
	}
	
}

void Game::sRender()
{
	// TODO: change the code below to draw ALL of the entities
	//       sample drawing of the player Entity that we have created

	m_window.clear();

	for (const auto& e : m_entities.getEntities())
	{
		// set the position of the shape based on the entity's transform->pos
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

		// set the rotation of the shape based on the entity's transform->angle
		e->cTransform->angle += 1.0f;
		e->cShape->circle.setRotation(e->cTransform->angle);

		// draw the entity's sf::CircleShape
		m_window.draw(e->cShape->circle);
	}
	m_text.setString("Score: " + std::to_string(m_score));
	m_window.draw(m_text);
	m_window.display();
}

void Game::sLifespan()
{
	for (auto& e : m_entities.getEntities())
	{

		std::string tag = e->getTag();
	
		if (tag == "bullet" || tag == "enemy" || tag == "smallenemy" || tag == "specialbullet")
		{
			//e->cLifespan->remainining -= (tag == "bullet") ? 3 : 1;
			e->cLifespan->remainining -= 3;


			// Calculate the alpha value based on the remaining lifespan
			float alpha = static_cast<float>(e->cLifespan->remainining) / static_cast<float>((tag == "bullet" || tag =="specialbullet") ? m_bulletConfig.L : m_enemyConfig.L); // initialLifespan is the maximum lifespan of the bullet

			// Ensure alpha is within valid range [0, 255]
			alpha = std::max(0.0f, std::min(1.0f, alpha)) * 255; // Scale alpha to range [0, 255]
		
			sf::Color fillColor = e->cShape->circle.getFillColor();
			sf::Color outlineColor = e->cShape->circle.getOutlineColor();

			fillColor.a = static_cast<sf::Uint8>(alpha);
			outlineColor.a = static_cast<sf::Uint8>(alpha);

			e->cShape->circle.setFillColor(fillColor);
			e->cShape->circle.setOutlineColor(outlineColor);


			if (e->cLifespan->remainining < 1)
			{
				// Destroy the bullet
				e->destroy();
			}
		}
	}
}

void Game::sMovement()
{
	// TODO: implememt all entity movement in this function
	//       you should read the m_player->cInput component to determine if the player is moving

	m_player->cTransform->velocity = { 0,0 };

	// implememt player movement
	if (m_player->cInput->up)
	{
		m_player->cTransform->velocity.y = -m_playerConfig.S;
	}

	if (m_player->cInput->down)
	{
		m_player->cTransform->velocity.y = m_playerConfig.S;
	}

	if (m_player->cInput->left)
	{
		m_player->cTransform->velocity.x = -m_playerConfig.S;
	}

	if (m_player->cInput->right)
	{
		m_player->cTransform->velocity.x = m_playerConfig.S;
	}

	// sample movement speed update
	 m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	 m_player->cTransform->pos.y += m_player->cTransform->velocity.y;


	 for (const auto& e : m_entities.getEntities("enemy"))
	 {

		 e->cTransform->pos.x += e->cTransform->velocity.x;
		 e->cTransform->pos.y += e->cTransform->velocity.y;
	 }

	 for (auto& b : m_entities.getEntities("bullet"))
	 {
		 b->cTransform->pos.x += b->cTransform->velocity.x;
		 b->cTransform->pos.y += b->cTransform->velocity.y;
	 }

	 for (auto& sm : m_entities.getEntities("smallenemy"))
	 {
		 sm->cTransform->pos.x += sm->cTransform->velocity.x;
		 sm->cTransform->pos.y += sm->cTransform->velocity.y;
	 }

	 for (auto& sb : m_entities.getEntities("specialbullet"))
	 {
		 sb->cTransform->pos.x += sb->cTransform->velocity.x;
		 sb->cTransform->pos.y += sb->cTransform->velocity.y;

	 }

}

void Game::sUserInput()
{
	// TODO: handle user input here
	//       note that you should only be setting the player's input component variables here
	//       you should no implement the player's movement logic here
	//       the movement system will read the variables you set in this function
	
	sf::Event event;
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
				std::cout << "W key pressed \n";
				// TODO: set player's input component "up" to true
				m_player->cInput->up = true;
			break;

			case sf::Keyboard::A:
				m_player->cInput->left = true;
			break;

			case sf::Keyboard::S:
				m_player->cInput->down = true;
			break;

			case sf::Keyboard::D:
				m_player->cInput->right = true;
			break;

			case sf::Keyboard::P:
				setPaused(m_paused);
				//m_paused = !m_paused;
				break;

			case sf::Keyboard::Escape:
				m_window.close();
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
				m_player->cInput->up = false;
			break;

			case sf::Keyboard::A:
				m_player->cInput->left = false;
			break;

			case sf::Keyboard::S:
				m_player->cInput->down = false;
			break;

			case sf::Keyboard::D:
				m_player->cInput->right = false;
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
				// call spawn bullet here
				spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
			break;

			case sf::Mouse::Right:
				spawnSpecialWeapon(m_player);
			break;

			default:
				break;
			}

		}
	}
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	int vertices = entity->cShape->circle.getPointCount();
	Vec2 parentPos = { entity->cTransform->pos.x, entity->cTransform->pos.y };
	float radius = entity->cShape->circle.getRadius();
	float angleIncrements = 360.f / vertices;

	sf::Color parentColor = entity->cShape->circle.getFillColor();
	sf::Color parentOutline = entity->cShape->circle.getOutlineColor();
	float parentThickness = entity->cShape->circle.getOutlineThickness();

	for (size_t i = 0; i <= vertices; i++)
	{
		float angle = i * angleIncrements;
		Vec2 point = polar_to_cartesian(radius, angle, parentPos.x, parentPos.y);
		Vec2 direction = point - parentPos;
		float distance = distanceBtwnVectors(parentPos.x, parentPos.y, point.x, point.y);

		// Normalize the direction vector
		if (distance != 0) {
			direction /= distance;
		}

		auto smallEnemy = m_entities.addEntity("smallenemy");

		smallEnemy->cTransform = std::make_shared<CTransform>(parentPos, Vec2(0,0), 0);
		smallEnemy->cShape = std::make_shared<CShape>(radius * 0.5f, vertices, parentColor, parentOutline, parentThickness);
		smallEnemy->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
		smallEnemy->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
		smallEnemy->cScore = std::make_shared<CScore>((vertices * 100) * 2);

		smallEnemy->cTransform->velocity = direction * m_enemyConfig.SMIN;

	}

}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
	Vec2 origin = entity->cTransform->pos;
	Vec2 direction = target - origin;
	float distance = origin.dist(target);

	// Normalize the direction vector
	if (distance != 0) {
		direction /= distance;
	}

	auto bullet = m_entities.addEntity("bullet");

	bullet->cTransform = std::make_shared<CTransform>(origin, Vec2(0,0), 0);
	bullet->cShape = std::make_shared<CShape>(10, 8, sf::Color(255,255,255),
		sf::Color(255, 0, 0), 2);

	// Set the velocity of the bullet entity based on the normalized direction vector and bullet speed
	bullet->cTransform->velocity = direction * m_bulletConfig.S;

	// Set the bullet lifespam
	bullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);

	// Set collision radius
	bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);

}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> player) 
{
	if (!m_isOnCoolDown)
	{
		m_isOnCoolDown = true;
		m_coolDownTimer = m_coolDownDuration;
		// TODO: implement your own special weapon
		int vertices = player->cShape->circle.getPointCount();
		Vec2 playerPos = { player->cTransform->pos.x, player->cTransform->pos.y };
		float radius = player->cShape->circle.getRadius();
		float angleIncrements = 360.f / vertices;

		sf::Color playerFill = player->cShape->circle.getFillColor();
		sf::Color playerOutline = player->cShape->circle.getOutlineColor();
		float playerOutlineThickness = player->cShape->circle.getOutlineThickness();

		for (size_t i = 0; i <= vertices; i++)
		{
			float angle = i * angleIncrements;
			Vec2 point = polar_to_cartesian(radius, angle, playerPos.x, playerPos.y);
			Vec2 direction = point - playerPos;
			float distance = distanceBtwnVectors(playerPos.x, playerPos.y, point.x, point.y);
			// Normalize the direction vector
			if (distance != 0) {
				direction /= distance;
			}

			auto smallBullet = m_entities.addEntity("specialbullet");

			smallBullet->cTransform = std::make_shared<CTransform>(playerPos, Vec2(0, 0), 0);
			smallBullet->cShape = std::make_shared<CShape>(10, 8, playerFill, playerOutline, playerOutlineThickness);
			smallBullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
			smallBullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);

			smallBullet->cTransform->velocity = direction * m_bulletConfig.S;

		}
		
	}
	else {
		std::cout << "Special weapon is on cooldown" << "\n";
	}

}

bool Game::checkCollision(std::shared_ptr<Entity> bullet, std::shared_ptr<Entity> enemy)
{
	Vec2 bulletPos = bullet->cTransform->pos;
	Vec2 enemyPos = enemy->cTransform->pos;
	Vec2 direction = enemyPos - bulletPos;

	//float distance = bulletPos.dist(enemyPos);

	float r1 = bullet->cCollision->radius;
	float r2 = enemy->cCollision->radius;

	// if distancesq = (r1+r2) * (r1+r2)
	bool collided = ((direction.x * direction.x) + (direction.y * direction.y)) < (r1 + r2) * (r1 + r2);

	// if distance < r1 + r2 collission
	//bool collided = distance < r1 + r2;

	return collided;
}

Vec2 Game::calculatePointsOnCircle(float radius, float angleDegrees)
{
	// Calculate x and y components using cosine and sine
	float angleRadians = angleDegrees * 3.14159265358979323846 / 180.0f;
	float xComponent = radius * std::cos(angleRadians);
	float yComponent = radius * std::sin(angleRadians);

	return { xComponent, yComponent };
}

// Function to calculate coordinates of a point given radius and angle
Vec2 Game::polar_to_cartesian(float radius, float angle_degrees, float center_x, float center_y) 
{
	float angle_radians = angle_degrees * M_PI / 180.0;
	float x = center_x + radius * std::cos(angle_radians);
	float y = center_y + radius * std::sin(angle_radians);
	return { x, y };
}

// Function to calculate distance between two points
float Game::distanceBtwnVectors(float x1, float y1, float x2, float y2)
{
	return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}



