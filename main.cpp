/*
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada s
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;


//variables para animación


//VARIABLES PARA CICLO DIA/NOCHE
float solAng = 0.0f; //angulo del soll   
const float VelSol = 0.25f;// Velocidad de avance del día en grados por segundo
glm::vec3 DirIni = glm::vec3(0.0f, -1.0f, 0.0f); // Dirección inicial del sol, mediodia

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

/*DECLARACIÓN DE CÁMARAS :
camara: la camara de editor que permite recorrer el mapa, se debe eliminar en la entrega final
camara1: la camara que sigue al avatar
camara2: camara que se posiciona sobre cada atracción
camara3: camara que muestra el mapa completo

*/
Camera camara1, camara2, camara3;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;

///----------AÑADIR MODELOS EN ESTA SECCIÓN---------------------

//exclusivo añadir modelos

//MARIO BROS
Model mario_torso;
Model mario_cabeza;
Model mario_b_izq;
Model mario_b_der;
Model mario_p_izq;
Model mario_p_izq_zap;
Model mario_p_der_1;
Model mario_p_der_2;
Model mario_p_der_zap;

// SKYBOX PARA DÍA (SKYBOX), NOCHE(SKYBOX_N) Y TARDE (SKYBOX_T)
Skybox skybox, skybox_n, skybox_t;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);

//DECLARACIONES PARA VARIABLES DE TIEMPO

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

//CREACIÓN DE OBJETOS, LOS ACTUALES SIRVEN SOLO
void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};


	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	//inicialización de cámaras
	camara1 = Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.5f, 0.5f);
	camara2 = Camera(glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), -90.0f, -89.9f, 0.5f, 0.5f);
	camara3 = Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	mario_torso = Model(); // Creamos modelo
	mario_torso.LoadModel("Models/mario_torso.obj");

	mario_cabeza = Model(); // Creamos modelo
	mario_cabeza.LoadModel("Models/mario_cabeza.obj");

	mario_b_izq = Model(); // Creamos modelo
	mario_b_izq.LoadModel("Models/mario_b_izq.obj");

	mario_b_der = Model(); // Creamos modelo
	mario_b_der.LoadModel("Models/mario_b_der.obj");

	mario_p_izq = Model(); // Creamos modelo
	mario_p_izq.LoadModel("Models/mario_p_izq.obj");

	mario_p_izq_zap = Model(); // Creamos modelo
	mario_p_izq_zap.LoadModel("Models/mario_p_izq_zap.obj");

	mario_p_der_1 = Model(); // Creamos modelo
	mario_p_der_1.LoadModel("Models/mario_p_der_1.obj");

	mario_p_der_2 = Model(); // Creamos modelo
	mario_p_der_2.LoadModel("Models/mario_p_der_2.obj");

	mario_p_der_zap = Model(); // Creamos modelo
	mario_p_der_zap.LoadModel("Models/mario_p_der_zap.obj");

	//SKYBOX DIA
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/izq.jpg");
	skyboxFaces.push_back("Textures/Skybox/der.jpg");
	skyboxFaces.push_back("Textures/Skybox/abajo.jpg");
	skyboxFaces.push_back("Textures/Skybox/arriba.jpg");
	skyboxFaces.push_back("Textures/Skybox/atras.jpg");
	skyboxFaces.push_back("Textures/Skybox/frente.jpg");

	//SKYBOX TARDE
	std::vector<std::string> skyboxFaces_t;
	skyboxFaces_t.push_back("Textures/Skybox/izq_t.png");
	skyboxFaces_t.push_back("Textures/Skybox/der_t.png");
	skyboxFaces_t.push_back("Textures/Skybox/abajo_t.png");
	skyboxFaces_t.push_back("Textures/Skybox/arriba_t.png");
	skyboxFaces_t.push_back("Textures/Skybox/atras_t.png");
	skyboxFaces_t.push_back("Textures/Skybox/frente_t.png");

	//SKYBOX NOCHE
	std::vector<std::string> skyboxFaces_n;
	skyboxFaces_n.push_back("Textures/Skybox/izq_n.png");
	skyboxFaces_n.push_back("Textures/Skybox/der_n.png");
	skyboxFaces_n.push_back("Textures/Skybox/abajo_n.png");
	skyboxFaces_n.push_back("Textures/Skybox/arriba_n.png");
	skyboxFaces_n.push_back("Textures/Skybox/atras_n.png");
	skyboxFaces_n.push_back("Textures/Skybox/frente_n.png");

	skybox = Skybox(skyboxFaces);
	skybox_t = Skybox(skyboxFaces_t);
	skybox_n = Skybox(skyboxFaces_n);

	//CREACIÓN DE MATERIALES, AÑADIR MÁS PARA LA FERIA
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//luz direccional, sólo 1 y siempre debe de existir

	//VARIABLE solAng para modificar el angulo en tiempo de ejecución.
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, -1.0f, 0.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	//algunas variables para avatar y cámara
	glm::mat4 view;
	glm::vec3 avatarPos; //posición del avatar
	float rotavatar, rotavatarY; //rotación del avatar
	//variables para cámara de cada juego:
	float minX, maxX, minZ, maxZ;

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//ACA SE REALIZA EL CAMBIO DE SKYBOX DEPENDIENDO LA LUZ	y la cámara
		if (solAng >= 80.0f && solAng < 250.0f) { //noche
			if (mainWindow.getcamtype() == 0) { //camara 3ra persona
				skybox_n.DrawSkybox(camara1.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 1) { //camara desde arriba
				skybox_n.DrawSkybox(camara2.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 2) { //camara viendo juego
				skybox_n.DrawSkybox(camara3.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 3) { //camara editor, se debe remover una vez se termine el juego.
				skybox_n.DrawSkybox(camara3.calculateViewMatrix(), projection);
			}
		}
		else if (solAng >= 250.0f && solAng < 300) { //amanecer/atardecer
			if (mainWindow.getcamtype() == 0) { //camara 3ra persona
				skybox_t.DrawSkybox(camara1.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 1) { //camara desde arriba
				skybox_t.DrawSkybox(camara2.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 2) { //camara viendo juego
				skybox_t.DrawSkybox(camara3.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 3) { //camara editor, se debe remover una vez se termine el juego.
				skybox_t.DrawSkybox(camara3.calculateViewMatrix(), projection);
			}
		}
		else if (solAng >= 300.0f && solAng <= 360) { //Día completo
			if (mainWindow.getcamtype() == 0) { //camara 3ra persona
				skybox.DrawSkybox(camara1.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 1) { //camara desde arriba
				skybox.DrawSkybox(camara2.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 2) { //camara viendo juego
				skybox.DrawSkybox(camara3.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 3) { //camara editor, se debe remover una vez se termine el juego.
				skybox.DrawSkybox(camara3.calculateViewMatrix(), projection);
			}
		}
		else if (solAng >= 0.0f && solAng < 30.0f) { //Día completo
			if (mainWindow.getcamtype() == 0) { //camara 3ra persona
				skybox.DrawSkybox(camara1.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 1) { //camara desde arriba
				skybox.DrawSkybox(camara2.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 2) { //camara viendo juego
				skybox.DrawSkybox(camara3.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 3) { //camara editor, se debe remover una vez se termine el juego.
				skybox.DrawSkybox(camara3.calculateViewMatrix(), projection);
			}
		}
		else if (solAng >= 30.0f && solAng < 80) { //amanecer/atardecer
			if (mainWindow.getcamtype() == 0) { //camara 3ra persona
				skybox_t.DrawSkybox(camara1.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 1) { //camara desde arriba
				skybox.DrawSkybox(camara2.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 2) { //camara viendo juego
				skybox.DrawSkybox(camara3.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 3) { //camara editor, se debe remover una vez se termine el juego.
				skybox_t.DrawSkybox(camara3.calculateViewMatrix(), projection);
			}
		}

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		//posición del avatar
		avatarPos = glm::vec3(
			0.0f - mainWindow.getposlat(),
			3.0f,
			0.0f - mainWindow.getposfron());
		//rotación del avatar
		rotavatar = mainWindow.getrotavatar();
		rotavatarY = mainWindow.getrotavatarY();
		//CAMARAS	
		if (mainWindow.getcamtype() == 0) { //vista tercera persona
			// Offset detrás y arriba del personaje
			glm::vec3 camOffset = glm::vec3(0.0f, 15.0f, 40.0f);
			camara1.followObject(avatarPos, camOffset, 10.0f, deltaTime, rotavatar, rotavatarY);
			// Calcular view matrix
			glm::mat4 view = camara1.calculateViewMatrix();
			// Enviar a los shaders
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
			glUniform3f(uniformEyePosition, camara1.getCameraPosition().x, camara1.getCameraPosition().y, camara1.getCameraPosition().z);

		}
		else if (mainWindow.getcamtype() == 1) { //vista aerea
			camara2.lookAtTarget(glm::vec3(0.0f, 550.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			//Se inicia cámara
			glm::mat4 view = camara2.calculateViewMatrix();
			//se mandan al shader
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
			glUniform3f(uniformEyePosition, camara2.getCameraPosition().x, camara2.getCameraPosition().y, camara2.getCameraPosition().z);

		}
		else if (mainWindow.getcamtype() == 2) { //vista sobre juego

			//PARA LA FUNCIÓN lookAtTarget el primer vector es la posición de la cámara y el segundo vector es la dirección a la que apunta
			camara3.lookAtTarget(glm::vec3(-50.0f, 20.0f, 0.0f), glm::vec3(0.0f, 0.5f, 0.5f));
			//Se inicia cámara
			glm::mat4 view = camara3.calculateViewMatrix();
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
			glUniform3f(uniformEyePosition, camara3.getCameraPosition().x, camara3.getCameraPosition().y, camara3.getCameraPosition().z);
		}
		else if (mainWindow.getcamtype() == 3) { //vista editor
			//para cámara editor se usa la cámara de siempre, se borrará cuando se vaya a entregar
			camara3.keyControl(mainWindow.getsKeys(), deltaTime);
			camara3.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			camara3.keyControl(mainWindow.getsKeys(), deltaTime);
			camara3.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camara3.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, camara3.getCameraPosition().x, camara3.getCameraPosition().y, camara3.getCameraPosition().z);

		}

		shaderList[0].SetDirectionalLight(&mainLight);

		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		//Función para ciclo dia/noche usando solAng
		solAng += VelSol * deltaTime; //se aumenta el angulo del sol con forme al deltatime y la velocidad del sol
		if (solAng > 360.0f) //al llegar a 360 se reinicia el día
			solAng -= 360.0f;

		glm::vec3 baseDirection = glm::vec3(0.0f, -1.0f, 0.0f); //dirección inicial
		glm::mat4 rotadia = glm::rotate(glm::mat4(1.0f), glm::radians(solAng), glm::vec3(1.0f, 0.0f, 0.0f)); //rotación de la dirección
		glm::vec3 newDirection = glm::vec3(rotadia * glm::vec4(baseDirection, 0.0f)); //se rota la dirección base 
		mainLight.direction = glm::normalize(newDirection); // se normaliza para ajustar

		printf("\nangulo del sol: %f", solAng);

		//inicia modelo de suelo
		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelaux2(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//MARIO 
		//torso
		model = glm::mat4(1.0);
		//Posición se ajusta para el avatar, mario es temporal
		model = glm::translate(model, glm::vec3(0.0f - mainWindow.getposlat(), 4.5f, 0.0f - mainWindow.getposfron())); //mov del avatar (cambiar a phineas)
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //ROTACIÖN OBLIGATORIA EN AVATAR
		model = glm::rotate(model, rotavatar * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //rotación del avatar (cambiar a phineas)

		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		modelaux = model; //se guarda traslación
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_torso.RenderModel();
		//cabeza
		model = glm::translate(model, glm::vec3(0.0f, 0.675f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_cabeza.RenderModel();
		//brazo izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.46f, 0.478f, -0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_b_izq.RenderModel();
		//brazo der
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.46f, 0.45f, -0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_b_der.RenderModel();
		//pierna izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.28f, -0.48f, -0.025f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_p_izq.RenderModel();
		//zapato izq
		model = glm::translate(model, glm::vec3(0.0f, -0.85f, -0.08f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_p_izq_zap.RenderModel();
		//pierna der
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.25f, -0.44f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_p_der_1.RenderModel();
		//pierna der 2
		model = glm::translate(model, glm::vec3(-0.01f, -0.449f, -0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_p_der_2.RenderModel();
		//zapato der
		model = glm::translate(model, glm::vec3(0.0f, -0.45f, -0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_p_der_zap.RenderModel();


		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
