/*

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

//VARIABLES PARA CICLO DIA/NOCHE
float solAng = 0.0f; //angulo del soll   
const float VelSol = 0.25f;// Velocidad de avance del día en grados por segundo
glm::vec3 DirIni = glm::vec3(0.0f, -1.0f, 0.0f); // Dirección inicial del sol, mediodia

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

/*DECLARACIÓN DE CÁMARAS :
camara1: la camara que sigue al avatar
camara2: Camara aerea muestra todo el mapa
camara3: camara que se posiciona para ver juegos
camara4: camara modo editor
*/
Camera camara1, camara2, camara3, camara4;

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

//PHINEAS
Model phineas_cuerpo;
Model phineas_brazoD;
Model phineas_brazoI;
Model phineas_piernaD;
Model phineas_piernaI;

//PIKACHU
Model pikachu;
Model banquito;
Model cola;

//CHARIZARD
Model charizard;
Model ala_der_char;
Model ala_izq_char;

//LUCARIO
Model lucario;
Model brazo_der_luc;
Model brazo_izq_luc;

//ENTONRNO
Model valla;
Model arbol_central;

//PUESTOS DE COMIDA
Model puesto_tacos;
Model trompo_pastor;

//JUEGO DE DADOS
Model dados_mesa;
Model dados_vaso;
Model dados_cubo1;
Model dados_cubo2;

//JUEGO HACHA
Model CabinaHacha;
Model Axe;

//PERRY
Model Perry_Cuerpo;
Model Perry_BrazoD;
Model Perry_BrazoI;
Model Perry_PiernaD;
Model Perry_PiernaI;
Model Perry_Sombrero;

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
	obj1->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj1);

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
	camara4 = Camera(glm::vec3(0.0f, 50.0f, 290.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 1.0f, 0.5f);

	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	//MODELO MARIO
	mario_torso = Model();
	mario_torso.LoadModel("Models/Mario/mario_torso.obj");
	mario_cabeza = Model();
	mario_cabeza.LoadModel("Models/Mario/mario_cabeza.obj");
	mario_b_izq = Model();
	mario_b_izq.LoadModel("Models/Mario/mario_b_izq.obj");
	mario_b_der = Model();
	mario_b_der.LoadModel("Models/Mario/mario_b_der.obj");
	mario_p_izq = Model();
	mario_p_izq.LoadModel("Models/Mario/mario_p_izq.obj");
	mario_p_izq_zap = Model();
	mario_p_izq_zap.LoadModel("Models/Mario/mario_p_izq_zap.obj");
	mario_p_der_1 = Model();
	mario_p_der_1.LoadModel("Models/Mario/mario_p_der_1.obj");
	mario_p_der_2 = Model();
	mario_p_der_2.LoadModel("Models/Mario/mario_p_der_2.obj");
	mario_p_der_zap = Model();
	mario_p_der_zap.LoadModel("Models/Mario/mario_p_der_zap.obj");

	//MODELO PHINEAS
	phineas_cuerpo = Model();
	phineas_cuerpo.LoadModel("Models/Phineas/phineas_cuerpo.obj");
	phineas_brazoD = Model();
	phineas_brazoD.LoadModel("Models/Phineas/phineas_brazoD.obj");
	phineas_brazoI = Model();
	phineas_brazoI.LoadModel("Models/Phineas/phineas_brazoI.obj");
	phineas_piernaD = Model();
	phineas_piernaD.LoadModel("Models/Phineas/phineas_piernaD.obj");
	phineas_piernaI = Model();
	phineas_piernaI.LoadModel("Models/Phineas/phineas_piernaI.obj");

	//MODELOS ENTORNO
	valla = Model();
	valla.LoadModel("Models/Valla.obj");
	arbol_central = Model();
	arbol_central.LoadModel("Models/arbol_central.obj");

	//Modelo Puesto de tacos
	puesto_tacos = Model();
	puesto_tacos.LoadModel("Models/Comida/puesto_tacos.obj");
	trompo_pastor = Model();
	trompo_pastor.LoadModel("Models/Comida/Trompo.obj");

	//Modelo pikachu
	pikachu = Model();
	pikachu.LoadModel("Models/Pikachu/Pikachu.obj");
	banquito = Model();
	banquito.LoadModel("Models/Pikachu/banquito.obj");
	cola = Model();
	cola.LoadModel("Models/Pikachu/Cola.obj");

	//Modelo Charizard
	charizard = Model();
	charizard.LoadModel("Models/Charizard/charizard.obj");
	ala_der_char = Model();
	ala_der_char.LoadModel("Models/Charizard/ala_der.obj");
	ala_izq_char = Model();
	ala_izq_char.LoadModel("Models/Charizard/ala_izq.obj");

	//Modelo Lucario
	lucario = Model();
	lucario.LoadModel("Models/Lucario/lucario.obj");
	brazo_der_luc = Model();
	brazo_der_luc.LoadModel("Models/Lucario/brazo_der.obj");
	brazo_izq_luc = Model();
	brazo_izq_luc.LoadModel("Models/Lucario/brazo_izq.obj");

	//MODELOS JUEGO DE DADOS
	dados_mesa = Model();
	dados_mesa.LoadModel("Models/Dados/dados_mesa.obj");
	dados_vaso = Model();
	dados_vaso.LoadModel("Models/Dados/dados_vaso.obj");
	dados_cubo1 = Model();
	dados_cubo1.LoadModel("Models/Dados/dados_cubo1.obj");
	dados_cubo2 = Model();
	dados_cubo2.LoadModel("Models/Dados/dados_cubo2.obj");

	//JUEGO DE HACHA
	CabinaHacha = Model();
	CabinaHacha.LoadModel("Models/Hacha/CabinaHacha.obj");
	Axe = Model();
	Axe.LoadModel("Models/Hacha/Axe.obj");

	//PERRY
	Perry_Cuerpo = Model();
	Perry_Cuerpo.LoadModel("Models/Perry/Perry_Cuerpo.obj");
	Perry_BrazoD = Model();
	Perry_BrazoD.LoadModel("Models/Perry/Perry_BrazoD.obj");
	Perry_BrazoI = Model();
	Perry_BrazoI.LoadModel("Models/Perry/Perry_BrazoI.obj");
	Perry_PiernaD = Model();
	Perry_PiernaD.LoadModel("Models/Perry/Perry_PiernaD.obj");
	Perry_PiernaI = Model();
	Perry_PiernaI.LoadModel("Models/Perry/Perry_PiernaI.obj");
	Perry_Sombrero = Model();
	Perry_Sombrero.LoadModel("Models/Perry/Perry_Sombrero.obj");

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
	//animaciones
	float contabasico = 0.0f;

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
				skybox_n.DrawSkybox(camara4.calculateViewMatrix(), projection);
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
				skybox_t.DrawSkybox(camara4.calculateViewMatrix(), projection);
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
				skybox.DrawSkybox(camara4.calculateViewMatrix(), projection);
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
				skybox.DrawSkybox(camara4.calculateViewMatrix(), projection);
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
				skybox_t.DrawSkybox(camara4.calculateViewMatrix(), projection);
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
			295.0f - mainWindow.getposfron());
		//rotación del avatar
		rotavatar = mainWindow.getrotavatar();
		rotavatarY = mainWindow.getrotavatarY();
		//CAMARAS	
		if (mainWindow.getcamtype() == 0) { //vista tercera persona
			// Offset detrás y arriba del personaje
			glm::vec3 camOffset = glm::vec3(0.0f, 40.0f, 35.0f);
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

			//CONDICIÓN JUEGO DADOS, para inicializar cámara

			if (mainWindow.getposlat() > 70.0f && mainWindow.getposlat() < 110.0f
				&& mainWindow.getposfron() > 460 && mainWindow.getposfron() < 485.0f) {
				camara3.lookAtTarget(glm::vec3(-92.5f, 22.5f, -190.0f), glm::vec3(-100.0f, 0.0f, -200.0f));
			}
			/// else if (conidiciones de tu juego) { cámara posicionada en tu juego } 
			else {
				//en caso de no estar cerca de ningún juego, se manda a una vista general del mapa
				//PARA PROBAR LA FUNCIÓN lookAtTarget de tu juego, puedes ponerlo aquí para evitar caminar hasta el lugar del mapa
				camara3.lookAtTarget(glm::vec3(0.0f, 50.0f, 300.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			}
			//Se inicia cámara
			glm::mat4 view = camara3.calculateViewMatrix();
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
			glUniform3f(uniformEyePosition, camara3.getCameraPosition().x, camara3.getCameraPosition().y, camara3.getCameraPosition().z);
		}
		else if (mainWindow.getcamtype() == 3) { //vista editor
			//para cámara editor se usa la cámara de siempre, se borrará cuando se vaya a entregar
			//VISTA SOBRE JUEGO DADOS:
			camara4.keyControl(mainWindow.getsKeys(), deltaTime);
			camara4.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			camara4.keyControl(mainWindow.getsKeys(), deltaTime);
			camara4.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camara4.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, camara4.getCameraPosition().x, camara4.getCameraPosition().y, camara4.getCameraPosition().z);
		}

		shaderList[0].SetDirectionalLight(&mainLight);

		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		//print para saber posición del avatar
		printf("\nposlat: %f ,  posfron: %f", mainWindow.getposlat(), mainWindow.getposfron());

		//Función para ciclo dia/noche usando solAng
		solAng += VelSol * deltaTime; //se aumenta el angulo del sol con forme al deltatime y la velocidad del sol
		if (solAng > 360.0f) //al llegar a 360 se reinicia el día
			solAng -= 360.0f;

		glm::vec3 baseDirection = glm::vec3(0.0f, -1.0f, 0.0f); //dirección inicial
		glm::mat4 rotadia = glm::rotate(glm::mat4(1.0f), glm::radians(solAng), glm::vec3(1.0f, 0.0f, 0.0f)); //rotación de la dirección
		glm::vec3 newDirection = glm::vec3(rotadia * glm::vec4(baseDirection, 0.0f)); //se rota la dirección base 
		mainLight.direction = glm::normalize(newDirection); // se normaliza para ajustar

		//printf("\nangulo del sol: %f", solAng);

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

		meshList[0]->RenderMesh();

		// ----------------------------------------------------- AVATAR --------------------------------------------------------------
		//PHINEAS 
		//cuerpo
		model = glm::mat4(1.0);
		//Posición se ajusta para el avatar
		model = glm::translate(model, glm::vec3(0.0f - mainWindow.getposlat(), 4.5f, 295.0f - mainWindow.getposfron())); //mov del avatar (cambiar a phineas)
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //ROTACIÖN OBLIGATORIA EN AVATAR
		model = glm::rotate(model, rotavatar * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); //rotación del avatar (cambiar a phineas)
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
		modelaux = model; //se guarda traslación
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		phineas_cuerpo.RenderModel();
		//brazo der
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.05f, 0.11f, 0.0f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, mainWindow.getcaminarD(), glm::vec3(1.0f, 0.0f, 0.0f));     // rotación senoidal en X
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		phineas_brazoD.RenderModel();
		//brazo izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.05f, 0.11f, 0.0f));
		model = glm::rotate(model, -30 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, mainWindow.getcaminarI(), glm::vec3(1.0f, 0.0f, 0.0f));     // rotación senoidal en X
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		phineas_brazoI.RenderModel();
		//pierna der
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.08f, -0.25f, 0.0f));
		model = glm::rotate(model, mainWindow.getcaminarD(), glm::vec3(1.0f, 0.0f, 0.0f));     // rotación senoidal en X
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		phineas_piernaD.RenderModel();
		//pierna izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.08f, -0.25f, -0.02f));
		model = glm::rotate(model, mainWindow.getcaminarI(), glm::vec3(1.0f, 0.0f, 0.0f));     // rotación senoidal en X
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		phineas_piernaI.RenderModel();

		// ----------------------------------------------------- OTROS PERSONAJES ----------------------------------------------------------
		//MARIO 
		//torso
		contabasico += 0.25f * deltaTime;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-75.0f, 10.5f - sin(contabasico * 0.1f) * 6, -200.0f));
		model = glm::rotate(model, 20 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		modelaux = model; //se guarda traslación
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_torso.RenderModel();
		//cabeza
		model = glm::translate(model, glm::vec3(0.0f, 0.675f, 0.0f));
		model = glm::rotate(model, 20 * toRadians * sin(contabasico * 0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_cabeza.RenderModel();
		//brazo izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.46f, 0.478f, -0.1f));
		model = glm::rotate(model, -50 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 20 * toRadians * sin(contabasico * 0.1f), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_b_izq.RenderModel();
		//brazo der
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.46f, 0.45f, -0.1f));
		model = glm::rotate(model, 60 * toRadians * sin(contabasico * 0.1f), glm::vec3(0.0f, -1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_b_der.RenderModel();
		//pierna izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.28f, -0.48f, -0.025f));
		model = glm::rotate(model, -20 * toRadians * sin(contabasico * 0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_p_izq.RenderModel();
		//zapato izq
		model = glm::translate(model, glm::vec3(0.0f, -0.85f, -0.08f));
		model = glm::rotate(model, 25 * toRadians - 20 * toRadians * sin(contabasico * 0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_p_izq_zap.RenderModel();
		//pierna der
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.25f, -0.42f, 0.0f));
		model = glm::rotate(model, -30 * toRadians + 40 * toRadians * sin(contabasico * 0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_p_der_1.RenderModel();
		//pierna der 2
		model = glm::translate(model, glm::vec3(-0.01f, -0.43f, -0.05f));
		model = glm::rotate(model, 20 * toRadians - 30 * toRadians * sin(contabasico * 0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_p_der_2.RenderModel();
		//zapato der
		model = glm::translate(model, glm::vec3(0.0f, -0.45f, -0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mario_p_der_zap.RenderModel();


		// ---------------------------------------------------- ENTORNO ----------------------------------------------------------------
		//Valla
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-300.0f, -1.8f, -290.0f));
		//model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();

		model = glm::translate(model, glm::vec3(10.6f, 0.0f, 14.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 200.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();

		model = glm::translate(model, glm::vec3(10.6f, 0.0f, 14.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();

		model = glm::translate(model, glm::vec3(10.6f, 0.0f, 14.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();

		//Arbol central 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.9f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol_central.RenderModel();


		// ------------------------------------------------------------ JUEGOS --------------------------------------------------------
		//JUEGO DADOS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 2.85f, -200.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.2f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dados_mesa.RenderModel();
		//vaso
		model = glm::translate(model, glm::vec3(3.0f, 5.0f, 3.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dados_vaso.RenderModel();
		//dado1
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dados_cubo1.RenderModel();
		//dado2
		model = glm::translate(model, glm::vec3(0.0f, 3.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dados_cubo2.RenderModel();

		//JUEGO HACHA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, -1.5f, -80.0f));
		modelaux = model;
		model = glm::rotate(model, 65 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.2f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CabinaHacha.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, -1.5f, -80.0f));
		modelaux = model;
		model = glm::rotate(model, 65 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.2f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Axe.RenderModel();

		// ------------------------------------------------------- PUESTOS DE COMIDA ---------------------------------------------------
		//Puesto de tacos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, 8.0f, 42.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(13.0f, 13.0f, 13.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puesto_tacos.RenderModel();
		//Trompo de la carne al pastor
		model = glm::translate(model, glm::vec3(-0.15f, -0.15f, 0.63f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		trompo_pastor.RenderModel();


		// ------------------------------------------------------------- NPC´S --------------------------------------------------------
		//Pikachu
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(37.0f, 8.0f, 56.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pikachu.RenderModel();
		//Cola
		model = glm::translate(model, glm::vec3(0.0f, 0.8f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cola.RenderModel();
		//Banquito
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banquito.RenderModel();


		//Charizard
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(250.0f, 15.0f, 250.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		charizard.RenderModel();
		//Ala derecha
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, -1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ala_der_char.RenderModel();
		//Ala izquierda
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ala_izq_char.RenderModel();

		//Lucario
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 11.8f, 250.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lucario.RenderModel();
		//brazo derecho
		model = glm::translate(model, glm::vec3(-1.7f, 3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazo_der_luc.RenderModel();
		//brazo izquierdo
		model = glm::translate(model, glm::vec3(3.4f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazo_izq_luc.RenderModel();

		//Perry
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, 0.0f, -90.0f));
		model = glm::rotate(model, 65 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Perry_Cuerpo.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.68f, 2.05f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Perry_BrazoD.RenderModel();

		float rotationAngle = sin(now * 1.5f) * glm::radians(-10.0f); // rotación senoidal entre -45° y 45°
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.65f, 2.06f, 0.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));     // rotación senoidal en Y
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Perry_BrazoI.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.4f, 0.43f, -0.16f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Perry_PiernaD.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.5f, 0.43f, -0.16f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Perry_PiernaI.RenderModel();

		float rotationAngleS = sin(now * 1.5f) * glm::radians(20.0f); // rotación senoidal entre -45° y 45°
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.2f, 3.7f, 0.0f));
		model = glm::rotate(model, -20 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rotationAngleS, glm::vec3(0.0f, 0.0f, 1.0f));     // rotación senoidal en Y
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Perry_Sombrero.RenderModel();

		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}