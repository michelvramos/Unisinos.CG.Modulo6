#pragma once

#include <vector>
#include "AnimationPoint.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static class Helper
{
public:
	static void InterpolateLinear(const AnimationPoint& p1, const AnimationPoint& p2, AnimationPoint& result, const float t);
	static void InterpolateHermite(const glm::vec3& p0, const glm::vec3& r0, const glm::vec3& t0, const glm::vec3& rt0, const glm::vec3& p1, const glm::vec3& r1, const glm::vec3& t1, const glm::vec3& rt1, glm::vec3& resultPos, glm::vec3& resultRot, const float t);
	static glm::vec3 ComputeTangent(const std::vector<glm::vec3>& points, const int i);
	static glm::vec3 FromBlender(const glm::vec3& pos);
};

