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
	static void InterpolateHermite(const AnimationPoint& p0, const glm::vec3& t0, const glm::vec3& rt0, const AnimationPoint& p1, const glm::vec3& t1, const glm::vec3& rt1, AnimationPoint& result, const float t);
	static glm::vec3 ComputeTangent(const std::vector<glm::vec3>& points, const int i);
	static glm::vec3 FromBlender(const glm::vec3& pos);
};

