// ===============================================
// @file   math.cpp
// @author kmurphy
// @brief  Collection of math functions useful for game development
// ===============================================

#include "math.h"

const float E = 2.71828182845904523536f;
const float PI = 3.14159265358979323846f; 
const float PI_OVER_2 = 1.57079632679489661923f;
const float PI_OVER_4 = 0.78539816339744830962f;

extern const float DEG_TO_RAD = PI/180.0f;
extern const float RAD_TO_DEG = 180.0f/PI;

const float MAX_FLOAT = 3.402823E+38f;
const float MIN_FLOAT = -3.402823E+38f;

const float EPSILON = 0.1E-4f;


// ===============================================
// Power like functions
// ===============================================

float sqr(float value) { return value*value; }
float cubed(float value) { return value*value*value; }

// ===============================================
// Semi-standard functions
// ===============================================

// Clamp a value to be within a specified interval
float clamp(float value, float low, float high) {
	ASSERT (low<=high, "Expected non-empty interval in Clamp");  
	return value<low ? low : (value>high ? high : value) ; 
}

// Clamps the given value down to zero if it is within epsilon of zero.
float zeroClamp (float value, float eps) {
	return fabs(value)<eps ? 0.0f : value;
}

float max(float value1, float value2) { 
	return (value1 > value2 ? value1 : value2); 
}

float max(float value1, float value2, float value3) { 
	return max(max(value1,value2), value3); 
}

float max(float value1, float value2, float value3, float value4) { 
	return max(max(value1,value2), max(value3,value4)); 
}   

float min(float value1, float value2) { 
	return (value1 < value2 ? value1 : value2); 
}

float min(float value1, float value2, float value3) { 
	return min(min(value1,value2), value3); 
}

float min(float value1, float value2, float value3, float value4) { 
	return min(min(value1,value2), min(value3,value4)); 
}   

// return +1,0, or -1 for positive, zero or negative input
int sgn(float value) { return (value > 0.0f ? 1 : (value < 0.0f ? -1 : 0)); }    

// return 0 for input less than the cutoff value or 1 otherwise.
int step(float value, float cutoff) { 
	return (value < cutoff ? 0 : 1); 
} 


// ===============================================
// Degree <-> Radian functions
// ===============================================

float degToRad(float value) { return value * DEG_TO_RAD; }
float radToDeg(float value) { return value * RAD_TO_DEG; }


// ===============================================
// Comparison functions
// ===============================================

// compare two floats up to given tolerance
bool isEqual(const float a, const float b, const float eps) {
	ASSERT (eps >= 0, "Expect a nonnegative tolerance for comparision.");
	return fabs(a-b) <= eps;
}

// ===============================================
// Random number generation functions
// ===============================================

void randomize(unsigned int seed) {
	srand(seed==0 ? (unsigned int)time(NULL) : seed);
}

int randomInt(int max) {
	ASSERT (max>0, "Interval for generating int values is empty");
	return (rand() % max);
}

int randomInt(int min, int max) {
	ASSERT (min<max, "Interval for generating int values is empty");
	return randomInt(max-min) + min;
}


int randomIntWithError(int target, int error) { 
	ASSERT (error>0, "Interval for generating int values is empty");
	return randomInt(target-error+1, target+error);
}

float randomFloat() {
	return float(rand())/float(RAND_MAX);
}

float randomFloat(float max) {
	return randomFloat() * max;
}

float randomFloat(float min, float max) {
	ASSERT (min<max, "Interval for generating float values is empty");
	return randomFloat(max-min) + min;
}

float randomFloatWithError(float target, float error) {
	ASSERT (error>0, "Interval for generating float values is empty");
	return randomFloat(target-error, target+error);
}

bool randomBool() {
	return randomInt(2) > 0;
}


