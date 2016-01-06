// ===============================================
// @file   defender.h
// @author atandon
// @practical defender
// @brief  Header file for PSP+GLFW(OpenGL) dfender game
// ===============================================

#include "defender.h"

// state for main game loop
GameState gameState = GAME_INTRO;

// frame timing information
double startTime, currentTime, previousTime, dt;
double previousBulletTime, previousBombTime;

Level * level;
Ship ship;
bool fire, fireBomb;
BulletPool shipBullets(1000);
BulletPool enemyBullets(1000);
BombPool shipBombs(1000);

int main() {
	
	initGraphics();

	bool done = false;
		
	/*level = new Level("test");
	cout << *level;
	cout << *level;
	cout << *level;
	if (level != NULL) { delete level; level = NULL; }*/

//	return 0;
	
	while (!done) {
		
		switch (gameState) {

			// state - GAME_INTRO
			// Display game intor screen etc
			// not implemented - just switches to state START_GAME 
			case(GAME_INTRO): {
				printf ("Game state changed from GAME_INTRO to GAME_START\n");
				gameState = GAME_START;
				break;
			}
			
			// state - GAME_START
			// initialse seting for start of game and start first level
			case(GAME_START): {
				printf ("Game state changed from GAME_START to LEVEL_START\n");
				gameState = LEVEL_START;
				break;
			}

			// state - LEVEL_START
			// Create level using data in file and start playing level
			case(LEVEL_START): {
				if (level != NULL) {  delete level; level = NULL; }
				level = new Level("test");
				cout << *level;
				printf ("Game state changed from LEVEL_START to LEVEL_PLAY\n");
				gameState = LEVEL_PLAY;
				break;
			}

			// state - LEVEL_PLAY
			case(LEVEL_PLAY): {
				update();
				render();
				getInput();
				if (ship.state==Entity::DEAD) gameState = LEVEL_OVER;
				break;
			}

			// state - LEVEL_OVER
			case(LEVEL_OVER): {
				gameState = GAME_OVER;
				break;
			}

			// state - GAME_OVER
			case(GAME_OVER): {
				render();
				getInput();
				break;
			}

			// state - GAME_QUIT
			case(GAME_QUIT): {
				done = true;
				break;
			}
			
			default: {
				printf ("Game state %d is not valid.\n", gameState);				
			}
		}
		
		done = done || (currentTime-startTime>40);
				
	}

	// free remaining resources
//	if (level != NULL) delete level;

	return 0;
}


