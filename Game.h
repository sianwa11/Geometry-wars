#pragma once

#include "Entity.h"
#include "EntityManager.h"

#include <SFML/Graphics.hpp>
#include <memory>



struct WindowConfig { int W, H, FL, FS; };
struct FontConfig { int S, R, G, B; std::string F;};
struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SP; float SMIN,SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };

class Game
{
	sf::RenderWindow m_window;         // the window we will draw to
	EntityManager    m_entities;      // vector of entities to maintain
	sf::Font         m_font;          // the font we will use to draw
	sf::Text         m_text;         // the score text to be drawn to the screen
	WindowConfig     m_windowConfig;
	FontConfig		 m_fontConfig;
	PlayerConfig     m_playerConfig;
	EnemyConfig      m_enemyConfig;
	BulletConfig     m_bulletConfig;
	int              m_score = 0;
	int              m_currentFrame = 0;
	int              m_lastEnemySpawnTime = 0;

	int              m_coolDownDuration = 10000;
	bool			 m_isOnCoolDown = false;
	int				 m_coolDownTimer = 0;

	bool             m_paused = false;        // whether we update game logic
	bool             m_running = true; // whether the game is running
	bool             m_outOfBounds = false;

	std::shared_ptr<Entity> m_player;

	void init(const std::string& config); // initialize the Gamestate with a config file path
	void setPaused(bool paused);          // pause the game

	void sMovement();                     // System: Entity position / movement update
	void sUserInput();                    // System: User Input
	void sLifespan();                     // System: Lifespan
	void sRender();                       // System: Render / Drawing
	void sEnemySpawner();                 // System: Spawns Enemies
	void sCollision();                    // System: Collisions

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

	void entityOutOfBounds(const std::shared_ptr<Entity>& e, float windowHeight, float windowWidth);
	bool checkCollision(std::shared_ptr<Entity> bullet, std::shared_ptr<Entity> enemy);
	Vec2 calculatePointsOnCircle(float radius, float angleDegrees);
	Vec2 polar_to_cartesian(float radius, float angle_degrees, float center_x, float center_y);
	float distanceBtwnVectors(float x1, float y1, float x2, float y2);



public:
	Game(const std::string& config); // constrcutor, takes in game config

	void run();
};