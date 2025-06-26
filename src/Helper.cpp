#include "Helper.h"


void Helper::InterpolateLinear(const AnimationPoint& p1, const AnimationPoint& p2, AnimationPoint& result, const float t)
{
	result.position.x = p1.position.x + (p2.position.x - p1.position.x) * t;
	result.position.y = p1.position.y + (p2.position.y - p1.position.y) * t;
	result.position.z = p1.position.z + (p2.position.z - p1.position.z) * t;
	result.rotation.y = p1.rotation.y + (p2.rotation.y - p1.rotation.y) * t;
	result.rotation.x = p1.rotation.x + (p2.rotation.x - p1.rotation.x) * t;
	result.rotation.z = p1.rotation.z + (p2.rotation.z - p1.rotation.z) * t;
}

void Helper::InterpolateHermite(const glm::vec3& p0, const glm::vec3& r0, const glm::vec3& tan0, const glm::vec3& rt0, const const glm::vec3& p1, const glm::vec3& r1, const glm::vec3& tan1, const glm::vec3& rt1, glm::vec3& resultPos, glm::vec3& resultRot, const float t)
{	
	glm::vec3 pp1 = p0;
	glm::vec3 pp2 = p1;

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

	resultPos = pos;

	resultRot.y = h00 * r0.y + h10 * rt0.x + h01 * r1.y + h11 * rt1.x;
	resultRot.x= h00 * r0.x + h10 * rt0.y + h01 * r1.x + h11 * rt1.y;
	resultRot.z = h00 * r0.z + h10 * rt0.z + h01 * r1.z + h11 * rt1.z;
		
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
