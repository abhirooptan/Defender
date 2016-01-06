// ===============================================
// @file   Level.h
// @author atandon
// @brief  Specification of class to represent a Level in the defender game
//
// A level contains all information relevent to a single level
// ie, the length of the level, ground position, ceiling position,
// position and type of all enemies.
// ===============================================

#ifndef LEVEL_H
#define LEVEL_H

#include "Vector2f.h"
#include "defender.h"


class Level : public Entity {

public:
	float length;			// horizontal length of level
	int groundLength;		// number of vectors to specift the ground
	int ceilingLength;		// number of vectors to specift the ceiling
	int healthLength;		// number of health objects
 	int enemyLength;		// number of enemy ship in level
	Vector2f * ground;		
	Vector2f * ceiling;
	Vector2f * health;
	Enemy * enemies;
	
	Level (const std::string filename);
	
	void render() const;
	
	void update (float dt) {
		position += dt*velocity;
	}
	
	~Level ();

	std::string toString() const;

	friend std::ostream& operator <<(std::ostream& outputStream, const Level& level);
};
extern Level * level;

#endif


