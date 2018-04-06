#include "Spline.h"

float clamp(const float value, const float minBound, const float maxBound) {

	float amp = maxBound - minBound;
	float val = fmod(value - minBound, amp);

	if (val < 0)
		val += amp;

	return val + minBound;
}

glm::mat4 adjustObjectPosition(glm::vec3& position, const glm::vec3& front, const glm::vec3& up) {

	glm::vec3 z = glm::normalize(front);
	glm::vec3 x = glm::normalize(glm::cross(up, z));
	glm::vec3 y = glm::cross(z, x);

	glm::mat4 matrix = glm::mat4(
		x.x, x.y, x.z, 0.0,
		y.x, y.y, y.z, 0.0,
		z.x, z.y, z.z, 0.0,
		position.x, position.y, position.z, 1.0
		);

	return matrix;
}

glm::vec3 evaluateCurveSegment(glm::vec3&  P0, glm::vec3&  P1, glm::vec3&  P2, glm::vec3&  P3, const float t) {

	glm::vec3 result(0.0, 0.0, 0.0);

	const float t2 = t*t;
	const float t3 = t*t*t;

	result = 0.5f * (
		P0 * (-1 * t3 + 2 * t2 - 1 * t + 0.0f)
		+ P1 * (3 * t3 - 5 * t2 + 0 * t + 2.0f)
		+ P2 * (-3 * t3 + 4 * t2 + 1 * t + 0.0f)
		+ P3 * (1 * t3 - 1 * t2 + 0 * t + 0.0f)
		);
	return result;
}

glm::vec3 evaluateCurveSegmentDerivative(glm::vec3&  P0, glm::vec3&  P1, glm::vec3&  P2, glm::vec3&  P3, const float t) {

	glm::vec3 result(1.0, 0.0, 0.0);

	const float t2 = t*t;

	result = 0.5f * (
		P0 * (3 * -1 * t2 + 2 * 2 * t - 1.0f)
		+ P1 * (3 * 3 * t2 - 2 * 5 * t + 0.0f)
		+ P2 * (3 * -3 * t2 + 2 * 4 * t + 1.0f)
		+ P3 * (3 * 1 * t2 - 2 * 1 * t + 0.0f)
		);

	return result;
}

glm::vec3 evaluateClosedCurve(glm::vec3 points[], const size_t count, const float t) {
	glm::vec3 result(0.0, 0.0, 0.0);

	float param = clamp(t, 0.0f, float(count));
	size_t i = float(param);

	result = evaluateCurveSegment(
		points[(i - 1 + count) % count],
		points[(i) % count],
		points[(i + 1) % count],
		points[(i + 2) % count],
		param - floor(param)
		);

	return result;
}

glm::vec3 evaluateClosedCurveDerivative(glm::vec3 points[], const size_t count, const float  t) {
	glm::vec3 result(1.0, 0.0, 0.0);

	float param = clamp(t, 0.0f, float(count));
	size_t i = float(param);

	result = evaluateCurveSegmentDerivative(
		points[(i - 1 + count) % count],
		points[(i) % count],
		points[(i + 1) % count],
		points[(i + 2) % count],
		param - floor(param)
		);

	return result;
}
