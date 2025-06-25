#include "Helper.h"


void Helper::InterpolateLinear(const AnimationPoint& p1, const AnimationPoint& p2, AnimationPoint& result, const float t)
{
	result.offsetX = p1.offsetX + (p2.offsetX - p1.offsetX) * t;
	result.offsetY = p1.offsetY + (p2.offsetY - p1.offsetY) * t;
	result.offsetZ = p1.offsetZ + (p2.offsetZ - p1.offsetZ) * t;
	result.offsetYaw = p1.offsetYaw + (p2.offsetYaw - p1.offsetYaw) * t;
	result.offsetPitch = p1.offsetPitch + (p2.offsetPitch - p1.offsetPitch) * t;
	result.offsetRoll = p1.offsetRoll + (p2.offsetRoll - p1.offsetRoll) * t;
}

void Helper::InterpolateHermite(const AnimationPoint& p0, const glm::vec3& tan0, const glm::vec3& rt0, const AnimationPoint& p1, const glm::vec3& tan1, const glm::vec3& rt1,AnimationPoint& result, const float t)
{	
	glm::vec3 pp1(p0.offsetX, p0.offsetY, p0.offsetZ);
	glm::vec3 pp2(p1.offsetX, p1.offsetY, p1.offsetZ);

	float t2 = t * t;
	float t3 = t2 * t;

	float h00 = 2 * t3 - 3 * t2 + 1;
	float h10 = t3 - 2 * t2 + t;
	float h01 = -2 * t3 + 3 * t2;
	float h11 = t3 - t2;

	glm::vec3 pos =
		h00 * pp1 +
		h10 * tan0 +
		h01 * pp2 +
		h11 * tan1;

	result.offsetX = pos.x;
	result.offsetY = pos.y;
	result.offsetZ = pos.z;

	result.offsetYaw = h00 * p0.offsetYaw + h10 * rt0.x + h01 * p1.offsetYaw + h11 * rt1.x;
	result.offsetPitch = h00 * p0.offsetPitch + h10 * rt0.y + h01 * p1.offsetPitch + h11 * rt1.y;
	result.offsetRoll = h00 * p0.offsetRoll + h10 * rt0.z + h01 * p1.offsetRoll + h11 * rt1.z;
		
}

glm::vec3 Helper::ComputeTangent(const std::vector<glm::vec3>& points, const int i)
{
	int n = points.size();
	float splineTension = 0.5f;
	// Para o primeiro ponto
	if (i == 0)
	{
		// Se parado no início (primeiro e segundo são iguais)
		if (n > 1 && points[0] == points[1])
			return glm::vec3(0.0f);
		else
			return splineTension * (points[1] - points[0]);
	}
	// Para o último ponto
	else if (i == n - 1)
	{
		// Se parado no fim (último e penúltimo são iguais)
		if (points[n - 2] == points[n - 1])
			return glm::vec3(0.0f);
		else
			return splineTension * (points[n - 1] - points[n - 2]);
	}
	// Para os pontos do meio
	else
	{
		// Se estamos em ponto de giro parado (atual igual ao anterior OU igual ao próximo)
		if (points[i] == points[i - 1] || points[i] == points[i + 1])
			return glm::vec3(0.0f);
		else
			return splineTension * (points[i + 1] - points[i - 1]);
	}
}

glm::vec3 Helper::FromBlender(const glm::vec3& position)
{
	glm::vec3 pos(-position.x, position.z, position.y);
	return pos;
}
