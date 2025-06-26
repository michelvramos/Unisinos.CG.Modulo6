#include "SimpleOBJLoader.h"
constexpr float PI = 3.14159265358979323846f;

void SimpleOBJLoader::Load(std::string filePath, Mesh& mesh)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<GLfloat> vBuffer;

	std::ifstream fileStream(filePath.c_str());
	if (!fileStream.is_open())
	{
		std::cerr << "Erro ao tentar ler o arquivo " << filePath << std::endl;
		return;
	}

	std::string line;
	while (std::getline(fileStream, line))
	{
		std::istringstream lineStream(line);
		std::string word;
		lineStream >> word;

		if (word == "v")
		{
			glm::vec3 vertice;
			lineStream >> vertice.x >> vertice.y >> vertice.z;
			vertices.push_back(vertice);
		}
		else if (word == "vt")
		{
			glm::vec2 vt;
			lineStream >> vt.s >> vt.t;
			texCoords.push_back(vt);
		}
		else if (word == "vn")
		{
			glm::vec3 normal;
			lineStream >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (word == "f")
		{
			while (lineStream >> word)
			{
				int vi = 0, ti = 0, ni = 0;
				std::istringstream ss(word);
				std::string index;

				if (std::getline(ss, index, '/')) vi = !index.empty() ? std::stoi(index) - 1 : 0;
				if (std::getline(ss, index, '/')) ti = !index.empty() ? std::stoi(index) - 1 : 0;
				if (std::getline(ss, index)) ni = !index.empty() ? std::stoi(index) - 1 : 0;

				vBuffer.push_back(vertices[vi].x);
				vBuffer.push_back(vertices[vi].y);
				vBuffer.push_back(vertices[vi].z);
				vBuffer.push_back(texCoords[ti].s);
				vBuffer.push_back(texCoords[ti].t);
				vBuffer.push_back(normals[ni].x);
				vBuffer.push_back(normals[ni].y);
				vBuffer.push_back(normals[ni].z);
			}
		}
	}

	fileStream.close();

	std::cout << "Gerando o buffer de geometria..." << std::endl;
	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vBuffer.size() * sizeof(GLfloat), vBuffer.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// layout: vx, vy, vz, s, t, ny, nx, nz
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	mesh.vertexCount = vBuffer.size() / 8;
	mesh.VAO = VAO;
}

void SimpleOBJLoader::LoadMTL(std::string filePath, MyMaterial& material)
{
	std::ifstream fileStream(filePath.c_str());
	if (!fileStream.is_open())
	{
		std::cerr << "Erro ao tentar ler o arquivo " << filePath << std::endl;
		return;
	}

	std::string line;
	while (std::getline(fileStream, line))
	{
		std::istringstream lineStream(line);
		std::string word;
		lineStream >> word;

		if (word == "Ns")
		{
			lineStream >> material.ns;
		}
		else if (word == "Ka")
		{
			lineStream >> material.ka.x >> material.ka.y >> material.ka.z;
		}
		else if (word == "Kd")
		{
			lineStream >> material.kd.x >> material.kd.y >> material.kd.z;
		}
		else if (word == "Ks")
		{
			lineStream >> material.ks.x >> material.ks.y >> material.ks.z;
		}
		else if (word == "Ke")
		{
			lineStream >> material.ke.x >> material.ke.y >> material.ke.z;
		}
		else if (word == "Ni")
		{
			lineStream >> material.ni;
		}
		else if (word == "d")
		{
			lineStream >> material.d;
		}
		else if (word == "illum")
		{
			lineStream >> material.illum;
		}
		else if (word == "map_Kd")
		{
			lineStream >> material.map_Kd;
		}
	}

	fileStream.close();
}

void SimpleOBJLoader::LoadAnimation(std::string filePath, std::vector<AnimationPoint>& animation)
{
	std::ifstream fileStream(filePath.c_str());
	if (!fileStream.is_open())
	{
		std::cerr << "Erro ao tentar ler o arquivo " << filePath << std::endl;
		return;
	}

	//'o'penGl or 'b'lender
	char type = 'o';
	float x, y, z, rx, ry, rz;

	std::string line;
	while (std::getline(fileStream, line))
	{
		std::istringstream lineStream(line);
		std::string word;
		lineStream >> word;

		if (word == "#" || word.empty())
		{
			continue;
		}

		if (word == "p")
		{
			AnimationPoint point;
			lineStream >> x>> y >>
				z >> ry >> rx
				>> rz >> point.timeMs;
			
			point.position = glm::vec3(x, y, z);
			point.rotation = glm::vec3(rx, ry, rz);

			animation.push_back(point);
		}
		else if (word == "t")
		{
			lineStream >> type;
		}
	}

	fileStream.close();
	float speed = 120.0f;
	
	//calcula tempo de animaçao em velocidade constante
	for (size_t i = 1; i < animation.size(); ++i)
	{
		if (animation[i].timeMs > -1.0f)
		{
			continue;
		}

		float dist = glm::distance(animation[i].position, animation[i-1].position);
		float tempo_ms = (dist / speed) * 1000.0f;
		
		animation[i].timeMs = tempo_ms;
	}


	if (type != 'b')
	{
		return;
	}

	x=0, y=0, z=0;
	
	//ajustar os eixos do vindos do Blender
	for (int i = 0; i < animation.size(); i++)
	{
		x = -animation[i].position.x;
		y = animation[i].position.z;
		z = animation[i].position.y;

		animation[i].position.x = x;
		animation[i].position.y = y;
		animation[i].position.z = z;
	}

	//transforma absolutos em offsets
	AnimationPoint p1 = animation[0];
	for (int i = 1; i < animation.size(); i++)
	{
		animation[i].position.x -= p1.position.x;
		animation[i].position.y-= p1.position.y;
		animation[i].position.z -= p1.position.z;
	}

	// ---- Cálculo dos ângulos acumulativos, pois Blender não ajuda!!!! ----
	 // Primeiro segmento define yaw inicial
	float dx0 = animation[1].position.x - animation[0].position.x;
	float dz0 = animation[1].position.z - animation[0].position.z;
	float yaw0 = std::atan2(dx0, -dz0) * (180.0f / PI);
	float yaw = yaw0;
	animation[0].rotation.y = yaw0;

	float prev_dx = dx0, prev_dz = dz0;
	float prev_len = std::sqrt(prev_dx * prev_dx + prev_dz * prev_dz);
	if (prev_len != 0.0f)
	{
		prev_dx /= prev_len;
		prev_dz /= prev_len;
	}

	for (int i = 1; i < animation.size(); i++)
	{
		float curr_dx = animation[i].position.x - animation[i - 1].position.x;
		float curr_dz = animation[i].position.z - animation[i - 1].position.z;
		float curr_len = std::sqrt(curr_dx * curr_dx + curr_dz * curr_dz);

		if (curr_len != 0.0f)
		{
			curr_dx /= curr_len;
			curr_dz /= curr_len;
		}

		// Produto vetorial e escalar 2D para delta angular no plano XZ
		float cross = prev_dx * curr_dz - prev_dz * curr_dx;
		float dot = prev_dx * curr_dx + prev_dz * curr_dz;

		float deltaYawRad = std::atan2(cross, dot);
		float deltaYawDeg = deltaYawRad * (180.0f / PI);
		yaw += deltaYawDeg;
		animation[i].rotation.y = yaw;

		// Pitch: subida/descida do segmento
		float dy = animation[i].position.y - animation[i - 1].position.y;
		float pitch = 0.0f;
		if (curr_len > 0.0f)
			pitch = std::atan2(dy, curr_len) * (180.0f / PI);
		animation[i].rotation.x = pitch;

		// ---- Acrescenta roll proporcional à curva ----
		// Ajuste "rollStrength" para controlar quanto o objeto inclina nas curvas.
		float rollStrength = 90.0f; // graus máximos para curvas muito fechadas (ajuste a gosto)
		float roll = glm::clamp(deltaYawRad * rollStrength, -rollStrength, rollStrength);
		animation[i].rotation.z = roll;

		// Atualiza vetor anterior para próximo passo
		prev_dx = curr_dx;
		prev_dz = curr_dz;
	}

	for (int i = 0; i < animation.size(); i++)
	{
		printf("p %.4f %.4f %.4f %.4f %.4f %.4f %.0f\n", animation[i].position.x, animation[i].position.y, animation[i].position.z, animation[i].rotation.y, animation[i].rotation.x, animation[i].rotation.z, animation[i].timeMs);
	}
}
