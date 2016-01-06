// ===============================================
// @file   math.h
// @author kmurphy
// @brief  Collection of math functions useful for game development
// ===============================================

#ifndef MATH_H
#define MATH_H

// Run time assertion with a message 
#include <cassert>
#define ASSERT(cond,mess) assert ((cond)&&mess);

#include <cstdlib>
#include <ctime>

#include <cmath>
#include <iostream>

extern const float E;                   ///< Euler's number http://en.wikipedia.org/wiki/E_(mathematical_constant)
extern const float PI;                  ///< Pi http://en.wikipedia.org/wiki/Pi
extern const float PI_OVER_2;   		///< PI/2
extern const float PI_OVER_4;   		///< PI/4

extern const float DEG_TO_RAD;			///< conversion factor from degrees to radian
extern const float RAD_TO_DEG;			///< conversion factor from radian to degrees

extern const float MIN_FLOAT;   		///< Minimum value represented using 32-bit floating point
extern const float MAX_FLOAT;   		///< Maximum value represented using 32-bit floating point

extern const float EPSILON;             ///< Limit of precision (when compared to unity)


// ===============================================
// Power like functions
// ===============================================

float sqr(float value);
float cubed(float value);

// ===============================================
// Semi-standard functions
// ===============================================

// Clamp a value to be within a specified interval
float clamp(float value, float low=0.0, float high=1.0);

// Clamps the given value down to zero if it is within epsilon of zero.
float zeroClamp (float value, float eps = EPSILON);

float max(float value1, float value2);
float max(float value1, float value2, float value3);
float max(float value1, float value2, float value3, float value4);

float min(float value1, float value2);
float min(float value1, float value2, float value3);
float min(float value1, float value2, float value3, float value4);

// return +1,0, or -1 for positive, zero or negative input
int sgn(float value);   

// return 0 for input less than the cutoff value or 1 otherwise.
int step(float value, float cutoff=0.0f);

// ===============================================
// Degree <-> Radian functions
// ===============================================

float degToRad(float value);
float radToDeg(float value);

// ===============================================
// Comparison functions
// ===============================================

// compare two floats up to given tolerance
bool isEqual(float a, float b, float eps=EPSILON);

// ===============================================
// Random number generation functions
// ===============================================

void randomize(unsigned int seed=0);
int randomInt(int max);
int randomInt(int min, int max);
int randomIntWithError(int target, int error);
float randomFloat(float max);
float randomFloat(float min, float max);
float randomFloatWithError(float target, float error);
bool randomBool();


#endif 
