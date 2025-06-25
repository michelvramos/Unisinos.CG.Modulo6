#include "MyCamera.h"

void MyCamera::AddYaw(float increment)
{
	m_yaw += increment;
}

void MyCamera::AddPitch(float increment)
{
	m_pitch = glm::clamp(m_pitch + increment, -89.0f, 89.0f);
}

void MyCamera::AddRoll(float increment)
{
	m_roll += increment;
}

MyCamera::MyCamera(float fovy, float aspect, float f_near, float f_far, glm::vec3 position, float yaw, float pitch, float roll, glm::vec3 up)
{
	m_original_yaw = m_yaw = yaw;
	m_original_pitch= m_pitch = glm::clamp(pitch,-89.0f, 89.0f);
	m_original_roll= m_roll = roll;

	// Matriz de projeção (perspectiva)
	m_projectionMatrix = glm::perspective(glm::radians(fovy),aspect, f_near, f_far);
	m_originalPosition = m_position = position;	
	m_originalUp= m_up = up;

	ApplyTransform();	
}

void MyCamera::LookAt(const glm::vec3& target)
{
	// Direção desejada
	glm::vec3 dir = glm::normalize(target - m_position);

	// Calcula pitch (ângulo em relação ao eixo Y)
	m_pitch = glm::degrees(asin(dir.y));

	// Calcula yaw (ângulo no plano XZ)
	m_yaw = glm::degrees(atan2(dir.z, dir.x));

	// Zera roll ao fazer LookAt
	m_roll = 0.0f;

	ApplyTransform();
}


void MyCamera::AddPosition(glm::vec3 position)
{
	m_position += position;
}

void MyCamera::ApplyTransform()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front = glm::normalize(front);

	//permite o roll
	glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
	glm::vec3 m_up = glm::normalize(glm::cross(right, front));
	m_up = glm::rotate(m_up, glm::radians(m_roll), front);

	m_viewMatrix = glm::lookAt(
		m_position,          //position
		m_position + front,  //look at
		m_up                 //up direction
	);
}

void MyCamera::ResetCamera()
{
	m_yaw = m_original_yaw;
	m_pitch = m_original_pitch;
	m_roll = m_original_roll;
	m_position = m_originalPosition;
	m_up = m_originalUp;
}

