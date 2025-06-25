/*
Adicione trajetórias para os objetos lidos em sua cena.
Cada objeto da cena poderá ter uma trajetória em que irá seguir repetidamente (preferencialmente cíclica).
Para isso, você deve permitir a adição de alguns pontos no espaço e armazenar suas coordenadas espaciais
na lista de pontos de controle da trajetória do objeto selecionado.  Neste exercício, você precisa criar
o mecanismo de adicionar pontos (pode ser por controle via teclado ou mouse ou mesmo a partir de arquivo
de configuração) e salvá-los. Além disso, deve realizar a translação do objeto por estes pontos de maneira
cíclica (ao chegar no último ponto, o objeto volta para a primeira posição). Ainda não é necessário
implementar uma interpolação por curva cúbica, pois faremos isso durante a Atividade Vivencial.

* ************************************************************************************
*                                    COMANDOS DE TECLA
* ************************************************************************************
*
* 8, 9 e 0: desliga/liga as luzes principa, secundária e de fundo.
*/
#pragma once
#include <iostream>
#include <string>
#include <assert.h>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SimpleOBJLoader.h"
#include "MyTextureLoader.h"
#include "My3DObject.h"
#include "MyCamera.h"
#include "Helper.h"
#include <irrKlang.h>

using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
int setupShader(GLint& success);
std::string LoadShaderSource(const std::string& filePath);
void EnableLight(GLuint uniLocation, glm::vec3 color);

bool enableLight1 = true, enableLight2 = true, enableLight3 = true;

//modo janela
//const GLuint WIDTH = (GLuint)(720 * 1.7), HEIGHT = 720;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	const GLuint WIDTH = (GLuint)mode->width, HEIGHT = mode->height;

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, u8"Computação Gráfica - Módulo 06", monitor, nullptr);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros de funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	GLint setupShaderSuccess;
	GLuint texturedAOShaderID = setupShader(setupShaderSuccess);

	if (!setupShaderSuccess)
	{
		return 0;
	}

	glUseProgram(texturedAOShaderID);

	//sound

	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
	if (!engine)
	{
		return 0; // error starting up the engine
	}

	
	
	My3DObject arwing("Assets/arwing.obj", "Assets/arwing.mtl");
	arwing.colorTextureId = MyTextureLoader::LoadTexture("Assets/baked_color.png", true);
	arwing.aoTextureId = MyTextureLoader::LoadTexture("Assets/baked_ao.png", true);//white png

	My3DObject tunel("Assets/Tunel.obj", "Assets/Tunel.mtl");
	tunel.colorTextureId = MyTextureLoader::LoadTexture("Assets/TunelColor.png", true);
	tunel.aoTextureId = arwing.aoTextureId;

	My3DObject torre("Assets/Torre.obj", "Assets/Torre.mtl");
	torre.colorTextureId = MyTextureLoader::LoadTexture("Assets/TorreColor.png", true);
	torre.aoTextureId = arwing.aoTextureId;

	My3DObject ground("Assets/Ground.obj", "Assets/Ground.mtl");
	ground.colorTextureId = MyTextureLoader::LoadTexture("Assets/GroundColor.png", true);
	ground.aoTextureId = arwing.aoTextureId;

	My3DObject arc("Assets/Arco.obj", "Assets/Arco.mtl");
	arc.colorTextureId = MyTextureLoader::LoadTexture("Assets/ArcoColor.png", true);
	arc.aoTextureId = arwing.aoTextureId;

	My3DObject launchingPad("Assets/LaunchingPad.obj", "Assets/LaunchingPad.mtl");
	launchingPad.colorTextureId = MyTextureLoader::LoadTexture("Assets/LaunchingPadColor.png", true);
	launchingPad.aoTextureId = arwing.aoTextureId;
	
	//from blender -> invert X and swap Y <> Z
	torre.SetStartPosition(glm::vec3(0));
	tunel.SetStartPositionFromBlender(glm::vec3(-20.0840f, -1146.8588f, 0.0000f));//posição do túnel no Blender
	ground.SetStartPositionFromBlender(glm::vec3(0.0000, -869.3160, 0.0000));
	launchingPad.SetStartPositionFromBlender(glm::vec3(-20, 50, 0));
	launchingPad.SetRotation(glm::vec3(0, 180, 0));
	launchingPad.Transform();

	//posição das torres no Blender
	std::vector<glm::vec3> towersPositions;
	towersPositions.push_back(Helper::FromBlender(glm::vec3(67.5957, -58.3908, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(-64.4169, -58.3908, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(67.5957, -135.2515, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(-64.4169, -135.2515, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(67.5957, -210.3521, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(-64.4169, -210.3521, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(67.5957, -289.5596, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(-64.4169, -289.5596, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(67.5957, -367.0070, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(-64.4169, -367.0070, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(67.5957, -455.6021, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(-64.4169, -455.6021, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(67.5957, -540.0902, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(-64.4169, -540.0902, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(67.5957, -635.7260, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(-64.4169, -635.7260, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(67.5957, -726.0813, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(-64.4169, -726.0813, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(67.5957, -817.6100, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(-64.4169, -817.6100, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(67.5957, -1088.9694, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(-64.4169, -1088.9694, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(67.5957, -1271.3423, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(-64.4169, -1271.3423, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(67.5957, -1471.3423, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(-64.4169, -1471.3423, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(67.5957, -1671.3423, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(-64.4169, -1671.3423, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(67.5957, -1871.3423, 0.0000)));
	towersPositions.push_back(Helper::FromBlender(glm::vec3(-64.4169, -1871.3423, 0.0000)));

	//posição dos Arcos no Blender
	std::vector<glm::vec3> arcPositions;
	arcPositions.push_back(Helper::FromBlender(glm::vec3(-17.4438, -692.8109, 0.0000)));
	arcPositions.push_back(Helper::FromBlender(glm::vec3(-30.3517, -784.3397, 0.0000)));
	arcPositions.push_back(Helper::FromBlender(glm::vec3(-5.4160, -876.7485, 0.0000)));
	arcPositions.push_back(Helper::FromBlender(glm::vec3(-17.4438, -1042.3998, 0.0000)));

	std::vector<AnimationPoint> animationPoints;
	SimpleOBJLoader::LoadAnimation("Assets/arwing.ani", animationPoints);

	//primeira e última posição de animação não é utilizada na animação
	//é utilizada apenas para a geração da tangente.
	std::vector<glm::vec3> pTangents,rTangents;
	
	pTangents.push_back(glm::vec3(0));
	rTangents.push_back(glm::vec3(0));

	glm::vec3 p0, p2;

	for (int i = 1; i < animationPoints.size() - 2; i++)
	{
		p0 = glm::vec3(animationPoints[i - 1].offsetX, animationPoints[i - 1].offsetY, animationPoints[i - 1].offsetZ);
		p2 = glm::vec3(animationPoints[i + 1].offsetX, animationPoints[i + 1].offsetY, animationPoints[i + 1].offsetZ);
		pTangents.push_back((p2-p0)/2.0f);

		p0 = glm::vec3(animationPoints[i - 1].offsetPitch, animationPoints[i - 1].offsetYaw, animationPoints[i - 1].offsetRoll);
		p2 = glm::vec3(animationPoints[i + 1].offsetPitch, animationPoints[i + 1].offsetYaw, animationPoints[i + 1].offsetRoll);
		rTangents.push_back((p2 - p0) / 2.0f);
	}
	pTangents.push_back(glm::vec3(0));
	rTangents.push_back(glm::vec3(0));


	arwing.SetStartPosition(glm::vec3(animationPoints[0].offsetX, animationPoints[0].offsetY, animationPoints[0].offsetZ));
	arwing.SetRotation(glm::vec3(0, 180, 0));
	arwing.Transform();	

	//índice dos parâmetros no vertex shader
	GLint modelUniLocation = glGetUniformLocation(texturedAOShaderID, "model");
	GLint viewUniLocation = glGetUniformLocation(texturedAOShaderID, "viewMatrix");
	GLint projectionUniLocation = glGetUniformLocation(texturedAOShaderID, "projectionMatrix");

	//índice dos parâmetros no fragment shader
	GLint textureUniLocation = glGetUniformLocation(texturedAOShaderID, "colorTexture");
	GLint aoTextureUniLocation = glGetUniformLocation(texturedAOShaderID, "aoMap");
	GLint kaUniLocation = glGetUniformLocation(texturedAOShaderID, "ka");
	GLint kdUniLocation = glGetUniformLocation(texturedAOShaderID, "kd");
	GLint ksUniLocation = glGetUniformLocation(texturedAOShaderID, "ks");
	GLint mainLightUniLocation = glGetUniformLocation(texturedAOShaderID, "mainLightPosition");
	GLint fillLightUniLocation = glGetUniformLocation(texturedAOShaderID, "fillLightPosition");
	GLint backLightUniLocation = glGetUniformLocation(texturedAOShaderID, "backLightPosition");
	GLint shininessUniLocation = glGetUniformLocation(texturedAOShaderID, "shininess");
	GLint mainLightColorUniLocation = glGetUniformLocation(texturedAOShaderID, "mainLightColor");
	GLint fillLightColorUniLocation = glGetUniformLocation(texturedAOShaderID, "fillLightColor");
	GLint backLightColorUniLocation = glGetUniformLocation(texturedAOShaderID, "backLightColor");
	
	glm::vec3 cameraStartPosition(-40.0f, 25.0f, -100.0f);

	MyCamera camera(
		30.0f, //fov y
		(float)WIDTH / (float)HEIGHT, //aspect ratio
		0.1f,    //near
		1000.0f, //far
		cameraStartPosition, //position
		90.0f, //yaw
		2.0f,  //pitch
		0.0f,  //roll
		glm::vec3(0.0f, 1.0f, 0.0f)//up
	);

	//posição das luzes
	glm::vec3 mainLightTransform = launchingPad.GetPosition() + glm::vec3(-10.0f, 1000.0f, -100.0f);
	glm::vec3 fillLightTransform = mainLightTransform+ glm::vec3(-1.0f, 0.8f, 2.0f); // Luz secundária (fill light)
	glm::vec3 backLightTransform = mainLightTransform+ glm::vec3(0.0f, 1.5f, -5.5f); // Luz de recorte (back light)

	glm::vec3 mainLightColor = glm::vec3(1.0f, 0.957f, 0.898f); // main light color (warm)
	glm::vec3 fillLightColor = glm::vec3(0.7f, 0.7f, 0.8f); // fill light (cooler, bluish)
	glm::vec3 backLightColor = glm::vec3(1.0f, 1.0f, 1.0f); // back light (neutral white)
	glm::vec3 blackColor = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 sunDir = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f)); // direção do sol
	glUniform3fv(glGetUniformLocation(texturedAOShaderID, "sunDirection"), 1, glm::value_ptr(sunDir));
	glUniform3f(glGetUniformLocation(texturedAOShaderID, "sunColor"), 1.0f, 1.0f, 0.95f); // branco amarelado

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Descarta apenas faces traseiras
	glFrontFace(GL_CCW); // Define sentido anti-horário como frente		

	float angleIncrement = 180.0f;
	float modelSpeedFactor = 2.5f;
	float cameraSpeedFactor = 200.0f;
	float cameraRotationSpeedFactor = 50.0f;
	float scaleFactor = 1.0f;
	float currentFrameTime = 0.0f;
	float lastFrameTime = glfwGetTime();
	float deltaTime = 0.0f;

	bool animationEnabled = true;
	int currentAnimationSegment = -1;
	int animationSegmentsSize = animationPoints.size();
	float animationStartTime = glfwGetTime() * 1000;
	AnimationPoint startAnimation{ 0,0,0,0,0,0,0 };
	float lastOffX, lastOffY, lastOffZ, lastOffYaw, lastOffRoll, lastOffPitch;
	float fps = 1.0f / 60.0f;

	glUniformMatrix4fv(projectionUniLocation, 1, GL_FALSE, glm::value_ptr(camera.m_projectionMatrix));

	glm::vec3 modelTranslation = glm::vec3(0.0f);
	glm::vec3 cameraTranslation = glm::vec3(0.0f);
	glm::vec3 cameraRotation = glm::vec3(0.0f);
	glm::vec3 modelRotation = glm::vec3(0.0f);
	float modelScaleFactor = 1.0f;
		
	engine->play2D("Assets/Star Fox - Corneria.mp3", true);
		
	while (!glfwWindowShouldClose(window))
	{
		currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;

		if (deltaTime < fps)
		{
			continue;
		}

		lastFrameTime = currentFrameTime;

		glfwPollEvents();
		glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniform3fv(mainLightUniLocation, 1, glm::value_ptr(mainLightTransform));
		glUniform3fv(fillLightUniLocation, 1, glm::value_ptr(fillLightTransform));
		glUniform3fv(backLightUniLocation, 1, glm::value_ptr(backLightTransform));		

		EnableLight(mainLightColorUniLocation, enableLight1 ? mainLightColor : blackColor);
		EnableLight(fillLightColorUniLocation, enableLight2 ? fillLightColor : blackColor);
		EnableLight(backLightColorUniLocation, enableLight3 ? backLightColor : blackColor);

		modelTranslation = glm::vec3(0.0f);
		cameraTranslation = glm::vec3(0.0f);
		cameraRotation = glm::vec3(0.0f);
		modelRotation = glm::vec3(0.0f);
		modelScaleFactor = 1.0f;		

		//animação da nave
		if (animationEnabled)
		{
			if (currentAnimationSegment < 1)
			{
				currentAnimationSegment = 1;
				animationStartTime = glfwGetTime() * 1000.0f;
				lastOffX = lastOffY = lastOffZ = lastOffPitch = lastOffRoll = lastOffYaw = 0.0f;
				startAnimation.offsetX = 0;
				startAnimation.offsetY = 0;
				startAnimation.offsetZ = 0;
				startAnimation.offsetYaw = 0;
				startAnimation.offsetPitch = 0;
				startAnimation.offsetRoll = 0;
			}

			float actualTimeMs = glfwGetTime() * 1000.0f;
			float t = actualTimeMs - animationStartTime;

			if (t > animationPoints[currentAnimationSegment].timeMs)
			{
				currentAnimationSegment++;
				animationStartTime = glfwGetTime() * 1000.0f;

				if (currentAnimationSegment >= animationPoints.size()-2)
				{
					currentAnimationSegment = 1;
					startAnimation.offsetX = 0;
					startAnimation.offsetY = 0;
					startAnimation.offsetZ = 0;
					startAnimation.offsetYaw = 0;
					startAnimation.offsetPitch = 0;
					startAnimation.offsetRoll = 0;
					//animationEnabled = false;
				}
				else
				{
					startAnimation.offsetX = animationPoints[currentAnimationSegment - 1].offsetX;
					startAnimation.offsetY = animationPoints[currentAnimationSegment - 1].offsetY;
					startAnimation.offsetZ = animationPoints[currentAnimationSegment - 1].offsetZ;
					startAnimation.offsetYaw = animationPoints[currentAnimationSegment - 1].offsetYaw;
					startAnimation.offsetPitch = animationPoints[currentAnimationSegment - 1].offsetPitch;
					startAnimation.offsetRoll = animationPoints[currentAnimationSegment - 1].offsetRoll;
				}
			}

			if (animationEnabled)
			{
				AnimationPoint currentAnimation = animationPoints[currentAnimationSegment];
				float percent = glm::clamp((actualTimeMs - animationStartTime) / currentAnimation.timeMs, 0.0f, 1.0f);
				AnimationPoint currOffset;
				
				Helper::InterpolateHermite(currentAnimation,
					pTangents[currentAnimationSegment],
					rTangents[currentAnimationSegment],
					animationPoints[currentAnimationSegment + 1],
					pTangents[currentAnimationSegment + 1],
					rTangents[currentAnimationSegment+1],
					currOffset,
					percent);

				modelTranslation.x = currOffset.offsetX - lastOffX;
				modelTranslation.y = currOffset.offsetY - lastOffY;
				modelTranslation.z = currOffset.offsetZ - lastOffZ;

				modelRotation.x = currOffset.offsetPitch - lastOffPitch;
				modelRotation.y = currOffset.offsetYaw - lastOffYaw;
				modelRotation.z = currOffset.offsetRoll - lastOffRoll;

				lastOffYaw = currOffset.offsetYaw;
				lastOffPitch = currOffset.offsetPitch;
				lastOffRoll = currOffset.offsetRoll;

				lastOffX = currOffset.offsetX;
				lastOffY = currOffset.offsetY;
				lastOffZ = currOffset.offsetZ;
			}
		}
		
		arwing.SetScale(glm::vec3(modelScaleFactor));		
		arwing.SetTranslation(modelTranslation);		
		arwing.SetRotation(modelRotation);		
		arwing.Transform();

		//animação da câmera
		if (currentAnimationSegment > 4 && currentAnimationSegment < 48)
		{
			//calcular o vetor frente/trás
			glm::vec3 backSide = -arwing.GetForward();
			
			float camDistance = 5.0f;
			float camHeight = 4.0f;

			//faz a câmera ir para frente ou trás da nave
			glm::vec3 desiredCameraPos = arwing.GetPosition() + backSide * camDistance + glm::vec3(0, camHeight, 0);
			camera.m_position = glm::mix(camera.m_position, desiredCameraPos, 0.05f); // 0.1f = suavidade

			//aplica a transformação, movendo a câmera.
			camera.ApplyTransform();			
		}
		else
		{
			camera.m_position = cameraStartPosition;
		}

		//faz a câmera apontar para a nave
		camera.LookAt(arwing.GetPosition());		
		camera.AddRoll(modelRotation.z*0.25f);
		camera.ApplyTransform();
		glUniformMatrix4fv(viewUniLocation, 1, GL_FALSE, glm::value_ptr(camera.m_viewMatrix));

		//desenhar os objetos
		ground.Draw(modelUniLocation, textureUniLocation, aoTextureUniLocation, kaUniLocation, kdUniLocation, ksUniLocation, shininessUniLocation);		
		arwing.Draw(modelUniLocation, textureUniLocation, aoTextureUniLocation, kaUniLocation, kdUniLocation, ksUniLocation, shininessUniLocation);
		tunel.Draw(modelUniLocation, textureUniLocation, aoTextureUniLocation, kaUniLocation, kdUniLocation, ksUniLocation, shininessUniLocation);

		for (int i = 0; i < towersPositions.size(); i++)
		{
			torre.ApplyTranslation(towersPositions[i]);
			torre.Draw(modelUniLocation, textureUniLocation, aoTextureUniLocation, kaUniLocation, kdUniLocation, ksUniLocation, shininessUniLocation);
		}

		for (int i = 0; i < arcPositions.size(); i++)
		{
			arc.ApplyTranslation(arcPositions[i]);
			arc.Draw(modelUniLocation, textureUniLocation, aoTextureUniLocation, kaUniLocation, kdUniLocation, ksUniLocation, shininessUniLocation);
		}

		launchingPad.Draw(modelUniLocation, textureUniLocation, aoTextureUniLocation, kaUniLocation, kdUniLocation, ksUniLocation, shininessUniLocation);
		glfwSwapBuffers(window);
	}

	try
	{
		engine->drop(); // delete engine
	}
	catch (const char* ex)
	{

	}

	// Pede pra OpenGL desalocar os buffers	
	arwing.DeallocateOpenGL();
	tunel.DeallocateOpenGL();
	torre.DeallocateOpenGL();
	arc.DeallocateOpenGL();
	launchingPad.DeallocateOpenGL();

	glDeleteProgram(texturedAOShaderID);

	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	enableLight1 = (key == GLFW_KEY_8) && (action != GLFW_RELEASE) ? !enableLight1 : enableLight1;
	enableLight2 = (key == GLFW_KEY_9) && (action != GLFW_RELEASE) ? !enableLight2 : enableLight2;
	enableLight3 = (key == GLFW_KEY_0) && (action != GLFW_RELEASE) ? !enableLight3 : enableLight3;
	
}

//Esta função está bastante hardcoded - objetivo é compilar e "buildar" um programa de
// shader simples e único neste exemplo de código
// O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
// fragmentShader source no iniçio deste arquivo
// A função retorna o identificador do programa de shader
int setupShader(GLint& success)
{
	std::string vertexCode = LoadShaderSource("Shaders/cube.vert");
	std::string fragmentCode = LoadShaderSource("Shaders/cube.frag");

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Checando erros de compilação (exibição via log no terminal)
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}

	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}

	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return 0;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

std::string LoadShaderSource(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Erro ao abrir o arquivo de shader: " << filePath << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

void EnableLight(GLuint uniLocation, glm::vec3 color)
{
	glUniform3fv(uniLocation, 1, glm::value_ptr(color));
}