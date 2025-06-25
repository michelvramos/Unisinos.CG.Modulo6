#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"
#include "MyMaterial.h"
#include "AnimationPoint.h"

class SimpleOBJLoader
{
public:
	static void Load(std::string filePath, Mesh& mesh);	
	static void LoadMTL(std::string filePath, MyMaterial& material);
	static void LoadAnimation(std::string filePath, std::vector<AnimationPoint>& animation);
};

