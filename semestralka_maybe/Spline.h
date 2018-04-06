#ifndef __SPLINE_H_
#define __SPLINE_H_
#include "pgr.h"
#include "Data.h"

/**
* Takes new position to an account (basicly rotates the object)
*/
glm::mat4 adjustObjectPosition(glm::vec3& position, const glm::vec3& front, const glm::vec3& up);

/**
* Evaluates a position on Catmull-Rom curve segment
*/
glm::vec3 evaluateCurveSegment(glm::vec3& P0, glm::vec3& P1, glm::vec3& P2, glm::vec3& P3, const float t);

/**
* Evaluates a first derivative of Catmull-Rom curve segment
*/
glm::vec3 evaluateCurveSegmentDerivative(glm::vec3& P0, glm::vec3& P1, glm::vec3& P2, glm::vec3& P3, const float t);

/**
* Evaluates a position on a closed curve composed of Catmull-Rom segments
*/
glm::vec3 evaluateClosedCurve(glm::vec3 points[], const size_t count, const float t);

/**
* Evaluates a first derivative of a closed curve composed of Catmull-Rom segments
*/
glm::vec3 evaluateClosedCurveDerivative(glm::vec3 points[], const size_t count, const float t);
//**************************************************************************************************
/// Clamping of a value.
/**
Makes sure that value is not outside the internal [\a minBound, \a maxBound].
If \a value is outside the interval it treated as periodic value with period equal to the size
of the interval. A necessary number of periods are added/subtracted to fit the value to the interval.
*/
float clamp(const float value, const float minBound, const float maxBound);

#endif