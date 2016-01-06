// ===============================================
// @file   Level.h
// @author atandon
// @brief  Implementation of class to represent a Level in the defender game
// ===============================================

#include <iostream>
#include <fstream>

#include "Level.h"

Level::Level (const std::string filename) {
	
	groundLength = ceilingLength = enemyLength = 0;
	ground = ceiling = NULL;
	enemies = NULL;
	
	// open filename (append ".dat" extenion)
	std::string fullname = filename + ".dat";
	std::ifstream file(fullname.c_str());
	if (!file.is_open()) {
		std::cerr <<"Cannot open level file (" <<filename <<".dat). Exiting...\n";
		exit(1);
	}
	
	std::string line;
	enum State {LENGTH, SPEED, GROUND_LENGTH, GROUND, CEILING_LENGTH, CEILING, ENEMY_LENGTH, ENEMY, HEALTH_LENGTH, HEALTH,END};
	int index;
	State state = LENGTH;
	 
	while(std::getline(file, line))	{	// read line by line

		// strip end of line comments (indicated by #)
		line = line.substr (0, line.find("#"));
		
		// trim spaces
		line.erase(line.find_last_not_of(" \n\r\t")+1);

		// skip empty lines
		if (line.length()==0) continue;
		
		// read in level data (based on simple state variable)
		switch (state) {
			
			case (LENGTH) : {
				length = atoi(line.c_str());
				state = SPEED;
				break;

			} case (SPEED) : {
				position = Vector2f::ZERO;
				velocity = Vector2f(atof(line.c_str()), 0.0f);
				state = GROUND_LENGTH;
				break;
				
			} case (GROUND_LENGTH) : {
				groundLength = atoi(line.c_str());
				if (groundLength==0) {
					state = ENEMY_LENGTH;
				} else {
					ground = new Vector2f[groundLength];
					state = GROUND;
					index = 0;
				}
				break;

			} case (GROUND) : {
				std::stringstream stream(line);
				stream >>ground[index++];
				if (index==groundLength) {
					// check if ground data starts at zero x-coordinate
					// and end at level length.
					if (!isEqual(ground[0].x,0)) {
						std::cerr << "Error : First ground should have zero x-coordinate. \n";
						exit(1);
					} else if (!isEqual(ground[groundLength-1].x, length)) {
						std::cerr << "Error : Last ground should have x-coordinate equal to level length. \n";
						exit(1);
					}
					state = CEILING_LENGTH;
				}
				break;

			} case (CEILING_LENGTH) : {
				ceilingLength = atoi(line.c_str());
				if (ceilingLength==0) {
					state = ENEMY_LENGTH;
				} else {
					ceiling = new Vector2f[ceilingLength];
					state = CEILING;
					index = 0;
				}
				state = (ceilingLength>0 ? CEILING : ENEMY_LENGTH);
				index = 0;
				break;

			} case (CEILING) : {
				std::stringstream stream(line);
				stream >>ceiling[index++];
				if (index==ceilingLength) {
					// check if ceiling data starts at zero x-coordinate
					// and end at level length.
					std::cout <<ceiling[ceilingLength-1] <<std::endl;
					if (!isEqual(ceiling[0].x,0)) {
						std::cerr << "Error : First ceiling should have zero x-coordinate." <<std::endl;
						exit(1);
					} else if (!isEqual(ceiling[ceilingLength-1].x, length)) {
						exit(1);
						std::cerr << "Error : Last ceiling should have x-coordinate equal to level length."  <<std::endl;
					}
					state = ENEMY_LENGTH;
				}
				break;

			} case (ENEMY_LENGTH) : {
				enemyLength = atoi(line.c_str());
				if (enemyLength==0) {
					state = HEALTH_LENGTH;
				} else {
					enemies = new Enemy[enemyLength];
					state = ENEMY;
					index = 0;
				}
				break;

			} case (ENEMY) : {
				int type=0;
				std::cout <<"here " <<index <<std::endl;
				std::stringstream stream(line);
				stream >>type >>enemies[index].position >>enemies[index].velocity;
				enemies[index].type = (type==1 ? Enemy::SIMPLE : Enemy::TRACKING);
				if (++index==enemyLength) state = HEALTH_LENGTH;
				break;

			} case (HEALTH_LENGTH) : {
				healthLength = atoi(line.c_str());
				if (healthLength==0) {
					state = END;
				} else {
					health = new Vector2f[healthLength];
					state = HEALTH;
					index = 0;
				}
				state = (healthLength>0 ? HEALTH : END);
				index = 0;
				break;

			} case (HEALTH) : {
				std::stringstream stream(line);
				stream >>health[index++];
				if (index==healthLength) {
					std::cout <<health[healthLength-1] <<std::endl;
					state = END;
				}
				break;

			}case (END) : {
				break;
				
			} default : {
			std::cerr <<"Invalid state (" <<state <<") while reading line. Exiting...\n";
			//exit(1);
			}
		}
	}
	
	file.close();	
}


Level::~Level () {
	if (ground != NULL) { delete [] ground; ground=NULL; }
	if (ceiling != NULL) { delete [] ceiling; ceiling=NULL; }
	if (enemies != NULL) { delete [] enemies; enemies=NULL; }
}


std::string Level::toString() const {
	std::ostringstream outs;
	outs <<"Length = " <<length <<"\n";
	outs <<"Speed = " <<position <<"\n";

	outs <<"Ground Vertices (" <<groundLength <<")\n";
	for(int k=0; k<groundLength; ++k)
		outs << ground[k] <<"\n";

	outs <<"Ceiling Vertices (" <<ceilingLength <<")\n";
	for(int k=0; k<ceilingLength; ++k)
		outs << ceiling[k] <<"\n";

	outs <<"Enemies (" <<enemyLength <<")\n";
	for(int k=0; k<enemyLength; ++k)
		outs << enemies[k] <<"\n";
		
	return outs.str();
	
}


std::ostream& operator <<(std::ostream& outputStream, const Level& level) {
	outputStream <<level.toString();
	return outputStream;
}



