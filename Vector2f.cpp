// ===============================================
// @file   Vector2f.cpp
// @author kmurphy
// @brief  Declatation of static members of 2D (float) vectors
// ===============================================

#include <iostream>
#include "Vector2f.h"

// -----------------------------------------------
// static members
// -----------------------------------------------

const float Vector2f::EPS = EPSILON;

const Vector2f Vector2f::X = Vector2f(1,0);
const Vector2f Vector2f::Y = Vector2f(0,1);;
const Vector2f Vector2f::ZERO = Vector2f(0,0);;


// -----------------------------------------------
// Friend functions
// -----------------------------------------------

Vector2f operator * (float scalar, const Vector2f& v)  { 
	return Vector2f(v.x*scalar, v.y *scalar); 
}

float dot(const Vector2f& u, const Vector2f& v) {
	return u.x*v.x + u.y*v.y;
}

std::ostream& operator <<(std::ostream& outputStream, const Vector2f& v) {
	outputStream <<v.toString();
	return outputStream;
}	

std::istream& operator >>(std::istream& inputStream, Vector2f& v) {
	inputStream >>v.x >>v.y;
	return inputStream;
}


