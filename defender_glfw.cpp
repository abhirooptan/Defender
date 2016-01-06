// ===============================================
// @file   defender_glfw.cpp
// @author atandon
// @practical defender
// @brief  Main file for GLFW(OpenGL) defender game
// ===============================================
#include "defender.h"
#include "Level.h"

#ifndef PSP

void Level::render() const {
	// draw ground
	glColor3ub(0, 100, 0);
	glBegin(GL_QUAD_STRIP);
	for (int k=0; k<level->groundLength; ++k) {
		glVertex3f(level->ground[k].x, level->ground[k].y, 0.0);
		glVertex3f(level->ground[k].x, 0.0, 0.0);
	}
	glEnd();

	// draw ceiling
	glColor3ub(0, 10, 100);
	glBegin(GL_QUAD_STRIP);
	for (int k=0; k<level->ceilingLength; ++k) {
		glVertex3f(level->ceiling[k].x, level->ceiling[k].y, 0.0);
		glVertex3f(level->ceiling[k].x, 1.0, 0.0);
	}
	glEnd();
	
	// draw health
	glColor3ub(0, 200, 0);
	for (int k=0; k < level->healthLength; ++k) {
		drawCircle(level->health[k].x, level->health[k].y, 0.03);
	}
}
void Ship::render() const {
	
	glPushMatrix();
	glTranslatef(position.x, position.y, 0.0f);
	glScalef(0.1, 0.08, 1);

	if(isAutoPilot) glColor3ub(200,200,200);
	else glColor3ub(0,200, 0);
	glBegin(GL_QUADS);
	glVertex3f(0.0, 0.0, 0.0);
    	glVertex3f(1.0, 0.4, 0.0);
    	glVertex3f(1.0, 0.6, 0.0);
    	glVertex3f(0.0, 1.0, 0.0);

	glColor3ub(0, 0, 200);  
    	glVertex3f(0.0, 0.47, 0.0);
    	glVertex3f(1.1, 0.47, 0.0);
    	glVertex3f(1.1, 0.53, 0.0);
    	glVertex3f(0.0, 0.53, 0.0);

	glEnd();
	glPopMatrix();
}
void Bullet::render() const {
	glColor3ub(200,100,0);
	drawCircle(position.x, position.y, 0.01);
}
void Bomb::render() const {
	glColor3ub(200,0,0);
	drawCircle(position.x, position.y, 0.02);
}
void Enemy::render() const {
	
	drawHealth(Vector2f(position.x-0.1, position.y+0.05), Vector2f(position.x,position.y+0.5), 0.02, health);
	
	if(state==AWAKE)	glColor3ub(100,100,0);
	else 				glColor3ub(100,100,100);
	drawCircle(position.x, position.y, 0.05);
	// tail
	glBegin(GL_TRIANGLES);
	glColor3ub(200, 200, 200);
		glVertex3f(position.x, position.y, 0);
       	glVertex3f(0.05 + position.x, position.y + 0.05, 0);
       	glVertex3f(0.05 + position.x, position.y - 0.05, 0);
	glEnd();
}
int initGraphics() {
	
	glfwInit(); 
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	if ( !glfwOpenWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 0,0,0,0,0,0, GLFW_WINDOW) ) { 
		glfwTerminate(); 
		std::cerr <<"glfwOpenWindow failed" <<endl;
		return 1; 
	}
	
	glfwSetWindowPos(100, 100);
	glfwSetWindowTitle("Defender");

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, ASPECT, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();							// reset the model view matrix

	// timing information
	startTime = currentTime = glfwGetTime();
	previousBulletTime = 0.0;

	return 0;		// success
	
}

int deinitGraphics() {
	glfwTerminate();
	return 0;		// success
}

void drawCircle(float x, float y, float radius)	{
	int triangleAmount = 20; //# of triangles used to draw circle
	float twicePi = 2.0f * 3.14;
	glPushMatrix();
	glBegin(GL_TRIANGLE_FAN);
	for(int i = 0; i <= triangleAmount;i++) { 
		glVertex2f(
	            x + (radius * cos(i * twicePi / triangleAmount)), 
		    y + (radius * sin(i * twicePi / triangleAmount))
		);
	}
	glEnd();
	glPopMatrix();
}

void drawHealth(Vector2f trans, Vector2f pos, float scale, float health) {	
  glPushMatrix();
	glTranslatef(trans.x, trans.y, 0.0f);
	glScalef(scale*0.1, scale, 1);
  glBegin(GL_QUADS);
  glColor3f(90-health, health-30, 0);
    glVertex2f(pos.x, pos.y);
    glVertex2f(pos.x+health, pos.y);
    glVertex2f(pos.x+health, pos.y+1);
    glVertex2f(pos.x, pos.y+1);
  glEnd();
  glPopMatrix();
}

void render() {

	// update frame timer
	previousTime = currentTime;
	currentTime = glfwGetTime();
	dt = currentTime - previousTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(-level->position.x, 0, 0);
	
	// rendering 
	level->render();
	
	for (int k=0; k < shipBullets.size();++k) {
		shipBullets[k].render();
	}
	for (int k=0; k < shipBombs.size();++k) {
		shipBombs[k].render();
	}
	for (int k=0; k < enemyBullets.size();++k) {
		enemyBullets[k].render();
	}
	for (int k=0; k<level->enemyLength; ++k) {
		if(level->enemies[k].state == Entity::AWAKE)
			level->enemies[k].render();
	}
	
	// players health
	drawHealth(Vector2f(level->position.x, level->position.y), Vector2f(5,1), 0.05, ship.health);
	
	ship.render();
	glPopMatrix();
	
	glfwSwapBuffers(); 
}

void getInput() {
	if (glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED)) {
		gameState = GAME_QUIT;
		return;
	}
	ship.velocity = level->velocity.x;
	if (glfwGetKey(GLFW_KEY_UP)) {
	ship.velocity.y = 0.7;
	} else if (glfwGetKey(GLFW_KEY_DOWN)) {
		ship.velocity.y = -0.7;
	}
	if (glfwGetKey(GLFW_KEY_LEFT)) {
		ship.velocity.x = level->velocity.x-0.5;
	} else if (glfwGetKey(GLFW_KEY_RIGHT)) {
		ship.velocity.x = level->velocity.x + 0.5;
	}
	if (glfwGetKey(GLFW_KEY_SPACE)){
		fire = true;
	}
	if (glfwGetKey('B')){
		fireBomb = true;
	}
	if (glfwGetKey('A')){
		ship.isAutoPilot = !ship.isAutoPilot;
	}
}

#endif
