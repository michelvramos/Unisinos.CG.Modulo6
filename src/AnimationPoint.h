#pragma once
#include <glm/glm.hpp>

struct AnimationPoint
{
	enum class PositionType
	{
		Absolute,
		Offset
	};

	glm::vec3 position;
	glm::vec3 rotation;
	float timeMs;
};