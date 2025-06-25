#pragma once


struct AnimationPoint
{
	enum class PositionType
	{
		Absolute,
		Offset
	};

	float offsetX, offsetY, offsetZ, offsetYaw, offsetPitch, offsetRoll, timeMs;
};