// ===============================================
// @file   update.cpp
// @author atandon
// @practical defender
// @brief  Update file for the defender game
// ===============================================
#include "defender.h"

void Ship::update(float dt)  {
	
	// clamp the x
	if(position.x < (level->position.x + 2.0/3.0) && position.x > (level->position.x)){
		position.x += dt*velocity.x;
	}
	else if(position.x > (level->position.x + 2.0/3.0) ){
		position.x = (level->position.x + 2.0/3.0);
	}
	if(position.x < level->position.x){
		position.x = (level->position.x+0.02);
	}

	// clamp the y
	if(position.y < 0.9 && position.y > 0 ){
		position.y += dt*velocity.y;
	}
	else if(position.y > 0.9){
		position.y = 0.9;
	}
	if(position.y < 0) {
		position.y = 0.02;
	}
	
	// game over
	if(health < 5) gameState = GAME_QUIT;
	
	// ground and ceiling collision
	pointSceneryCollision(Vector2f(position.x, position.y+0.1), false, isAutoPilot);
	pointSceneryCollision(position, true, isAutoPilot);
	playerHealthCollision(Vector2f(position.x+0.1, position.y+0.05));
	
}
void Bullet::update(float dt)  {
	if (currentTime-timeToDie>1.5) {
			state = DEAD;
	}
	if(state == AWAKE){
		position += dt*velocity;
	}
}
void Bomb::update(float dt)  {
	if (position.y < 0.0) {
			state = DEAD;
	}
	if(state == AWAKE){
		position += dt*velocity;
		if(isPointInsideCircle(position, true, 95)) state = DEAD;
	}
}
void Enemy::update(float dt)  {
	position += dt*velocity;
	// limit firing of bullets to 5 a second
    if (currentTime-previousBulletTime>0.2) {
		Bullet & bullet = enemyBullets.allocate();
       	bullet.state = Entity::AWAKE;
       	bullet.position = position - Vector2f(0.03,0.0);
       	bullet.velocity.x = -1;
       	bullet.timeToDie = currentTime;
       	previousBulletTime = currentTime;
	}
	switch(type){
		case (SIMPLE):
			if(position.y >= 0.8){
				position.y = 0.78;
				velocity.y = -velocity.y;
			}else if(position.y <= 0.2){
				position.y = 0.22;
				velocity.y = -velocity.y;
			}
			break;
		case (TRACKING):
			double angle = atan2(ship.position.y-position.y, ship.position.x-position.x);
			double speed = 0.3;
			velocity.x = speed*cos(angle);
			velocity.y = 2*speed*sin(angle);
	}
}

// ===============================================
// Main Update
// ===============================================
void update(){

// Firing Ship Bullets
    if (fire && currentTime-previousBulletTime>0.2) {			// limit firing of bullets to 5 a second
		Bullet & bullet = shipBullets.allocate();
        bullet.state = Entity::AWAKE;
        bullet.position = ship.position + Vector2f(0.03,0.04);
        bullet.timeToDie = currentTime;
        fire = false;
        previousBulletTime = currentTime;
	}
	
// Firing Ship Bombs
    if (fireBomb && currentTime-previousBombTime>0.5) {			// limit firing of bombs to 2 a second
		Bomb & bomb = shipBombs.allocate();
        bomb.state = Entity::AWAKE;
        bomb.position = ship.position + Vector2f(0.03,0.035);
        fireBomb = false;
        previousBombTime = currentTime;
	}
	
// Level, Ship and Enemy updates
	level->update(dt);											// level update
	ship.update(dt);											// ship update
	
	for (int k=0; k<level->enemyLength; ++k) {
		if(fabs(level->enemies[k].position.x - ship.position.x) < 1.5 && level->enemies[k].health > 5){
			level->enemies[k].state = Entity::AWAKE;
		}else level->enemies[k].state = Entity::ASLEEP;
		
		if(level->enemies[k].state == Entity::AWAKE){
			level->enemies[k].update(dt);						// enemy update
			playerEnemyCollision(level->enemies[k].position);
		}
	}
	
// Ship Bombs
	for (int k=0; k < shipBombs.size();++k) {
		if(shipBombs[k].state == Entity::DEAD){
			shipBombs.free(k);
		} else
			shipBombs[k].update(dt);
	}
	
// Ship Bullets
	for (int k=0; k < shipBullets.size();++k) {			
		if(shipBullets[k].state == Entity::DEAD){
			shipBullets.free(k);
		} else{
			shipBullets[k].update(dt);
			if(isPointInsideCircle(shipBullets[k].position, true, 20)){
				shipBullets[k].state = Entity::DEAD;
			}
		}
	}
	
// Enemy Bullets
	for (int k=0; k < enemyBullets.size();++k) {			
		if(enemyBullets[k].state == Entity::DEAD){
			enemyBullets.free(k);
		} else{
			enemyBullets[k].update(dt);
			if(isPointInsideCircle(enemyBullets[k].position, false, 20)) {
				enemyBullets[k].state = Entity::DEAD;
			}
		}
	}
}

// ===============================================
// Methods for Collision Detection
// ===============================================

bool pointLineCollision(Vector2f p, Vector2f a, Vector2f b, bool isGround, bool isAutoPilot)
{
	Vector2f c = b-a;
	Vector2f normal = c.normal();
	Vector2f other = p-a;
	
	// for autoPilot
	Vector2f otherGround = Vector2f(p.x+0.03,p.y-0.1)-a;	// the bottom-right (tip) of the ship
	Vector2f otherCeiling = Vector2f(p.x,p.y+0.1)-a;		// the top-left of the ship
		
	if(isAutoPilot){
		if (isGround) {
			if (dot(otherGround,normal)< 0){
				// a cheat auto-pilot
				ship.position.y += 0.1;//a.x*tan(fabs((b.y-a.y)/(b.x-a.x))); // trying to find slope
			}
		}
		else {
			if (dot(otherCeiling,normal)> 0){
				// a cheat auto-pilot
				ship.position.y -= 0.1;//a.x*tan(fabs((b.y-a.y)/(b.x-a.x)));
			}
		}
	}
	if (isGround) return (dot(other,normal)< 0);
	else return (dot(other,normal)> 0);
}

void pointSceneryCollision(Vector2f p, bool isGround, bool isAutoPilot)
{
	int length = level-> groundLength;
	Vector2f* points = level->ground;
	if(!isGround){
		length = level->ceilingLength;
		points = level->ceiling;
	}

	int k=0;
	while(points[k+1].x<p.x)
		k=k+1;
		 
	// decrease player health when touching the ground or ceiling
	if(pointLineCollision(p, points[k], points[k+1], isGround, isAutoPilot)) {
		ship.health -= 0.1;
	}
}

bool isPointInsideRectangle(Vector2f p, Vector2f box, float size) {

	return (p.x>=box.x-size && p.y>=box.y-size && p.x<=box.x+size && p.y<=box.y+size);
}

bool isPointInsideCircle(Vector2f p, bool isEnemy, int damage){
	if(isEnemy)	{
		for (int k=0; k<level->enemyLength; ++k) {
			if(level->enemies[k].state == Entity::AWAKE && 
				p.x > level->enemies[k].position.x - 2 && p.x < level->enemies[k].position.x + 2)
				if (isPointInsideRectangle(p, level->enemies[k].position, 0.05)){
					level->enemies[k].health -= damage;
					return true;
				}
				else return false;
		}
	}
	else if (isPointInsideRectangle(p, Vector2f(ship.position.x+0.05, ship.position.y+0.05), 0.05)){
		ship.health -= 10;
		return true;
	}
	else false;
}

void playerEnemyCollision(Vector2f p) {
	if(isPointInsideRectangle(Vector2f(ship.position.x+0.1, ship.position.y+0.05), p, 0.05)) {
		ship.health = 2;
	}
}

void playerHealthCollision(Vector2f p) {
	int length = level-> healthLength;
	Vector2f* healths = level->health;

	int k=0;
	while(healths[k].x < p.x)
		k=k+1;
		 
	// decrease player health when touching the ground or ceiling
	if(isPointInsideRectangle(p, healths[k], 0.05) ) {
		ship.health = 100;
	}
}
