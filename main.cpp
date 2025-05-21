
/*
PROYECTO CGEIHC.
FECHA DE ENTREGA 20/05/25

Aguilar Perez José Ramón
Dominguez Chavez Jesus Abner
Sanchez Calvillo Saida Mayela
Sanchez Villalpando Johan

FERIA INSPIRADA EN UNIVERSOS:
PHINES Y FERB
HOLLOW KNIGHT
MARIO BROS
POKEMON
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

//PARA AUDIO
#include <SFML\Audio.hpp>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

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
Model estrellita;
Model hongo_verde;
Model moneda;
Model tragamoneda;
Model toad_cuerpo;
Model toad_b_izq;
Model toad_b_der;

//PHINEAS
Model phineas_cuerpo;
Model phineas_brazoD;
Model phineas_brazoI;
Model phineas_piernaD;
Model phineas_piernaI;

//HOLLOW KNIGHT
Model cuerpo;
Model mano_izq;
Model capa;
Model mask;

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
Model entrada;
Model faro;
Model piso_piedras;
Model rueda;
Model canastas;
Model base;
Model carrusel;
Model centro;
Model caballo;
Model caballo2;
Model caballo3;
Model caballo4;

//PUESTOS DE COMIDA
Model puesto_tacos;
Model trompo_pastor;
Model palomitas;

//ROTULOS
Model rotulo_perry;
Model rotulo_mario;
Model rotulo_hollow;

//JUEGO DE DADOS
Model dados_mesa;
Model dados_vaso;
Model dados_cubo1;
Model dados_cubo2;

//JUEGO HACHA
Model CabinaHacha;
Model Axe;

//JUEGO GLOBOS
Model carpa;
Model globo1;
Model globo2;
Model globo3;
Model tronco;
Model hornet;
Model linea_H;
Model dardo;

//JUEGO TOPOS
Model topos;
Model topito;
Model mazo;

//JUEGO BOLOS
Model linea_bolos;
Model voltorb;
Model pino;

//JUEGO JAULA DE BATEO
Model jaula;
Model bate;
Model pokeball;
Model lanzabolas;
Model guardabolas;

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
Material Material_metalico;
Material Material_tela;
Material Material_madera;
Material Material_ceramica;
Material Material_planta;

//Sphere cabeza = Sphere(0.5, 20, 20);

//DECLARACIONES PARA VARIABLES DE TIEMPO

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//LUCES TIPO POINTLIGHT
PointLight pointLights[MAX_POINT_LIGHTS];
PointLight lucesFeria[MAX_POINT_LIGHTS];
PointLight lucesHacha[MAX_POINT_LIGHTS];
PointLight lucesGlobos[MAX_POINT_LIGHTS];
PointLight lucesJaula[MAX_SPOT_LIGHTS];
//LUCES TIPO SPOTLIGHTS
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight lucesDados[MAX_SPOT_LIGHTS];
SpotLight lucesSpotJaula[MAX_SPOT_LIGHTS];
SpotLight lucesTopos[MAX_SPOT_LIGHTS];
SpotLight lucesBolos[MAX_SPOT_LIGHTS];
SpotLight lucesFaros[MAX_SPOT_LIGHTS];

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
	//Objetos universo Mario
	estrellita = Model(); //Creamos modelo
	estrellita.LoadModel("Models/Mario/estrellita.obj");
	hongo_verde = Model(); //Creamos modelo
	hongo_verde.LoadModel("Models/Mario/1upmushroom.obj");
	moneda = Model(); //Creamos modelo
	moneda.LoadModel("Models/Mario/moneda.obj");
	tragamoneda = Model(); //Creamos modelo
	tragamoneda.LoadModel("Models/tragamoneda.obj");

	//MODEL TOAD
	toad_cuerpo = Model(); //Creamos modelo
	toad_cuerpo.LoadModel("Models/Toad/toad_cuerpo.obj");
	toad_b_izq = Model(); //Creamos modelo
	toad_b_izq.LoadModel("Models/Toad/toad_b_izq.obj");
	toad_b_der = Model(); //Creamos modelo
	toad_b_der.LoadModel("Models/Toad/toad_b_der.obj");

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

	//MODELO HOLLOW KNIGHT
	cuerpo = Model();
	cuerpo.LoadModel("Models/HollowKnight/cuerpo.obj");
	mano_izq = Model();
	mano_izq.LoadModel("Models/HollowKnight/mano_izq.obj");
	capa = Model();
	capa.LoadModel("Models/HollowKnight/capa.obj");
	mask = Model();
	mask.LoadModel("Models/HollowKnight/mask.obj");

	//MODELOS ENTORNO
	valla = Model();
	valla.LoadModel("Models/Valla.obj");
	arbol_central = Model();
	arbol_central.LoadModel("Models/arbol_central.obj");
	entrada = Model();
	entrada.LoadModel("Models/Entrada.obj");
	faro = Model();
	faro.LoadModel("Models/faro1.obj");
	piso_piedras = Model();
	piso_piedras.LoadModel("Models/piso_piedras.obj");
	rueda = Model();
	rueda.LoadModel("Models/Rueda/rueda.obj");
	canastas = Model();
	canastas.LoadModel("Models/Rueda/Canastas.obj");
	base = Model();
	base.LoadModel("Models/Rueda/base.obj");
	carrusel = Model();
	carrusel.LoadModel("Models/Carrusel/carrusel.obj");
	centro = Model();
	centro.LoadModel("Models/Carrusel/centro.obj");
	caballo = Model();
	caballo.LoadModel("Models/Carrusel/caballo.obj");
	caballo2 = Model();
	caballo2.LoadModel("Models/Carrusel/caballo2.obj");
	caballo3 = Model();
	caballo3.LoadModel("Models/Carrusel/caballo3.obj");
	caballo4 = Model();
	caballo4.LoadModel("Models/Carrusel/caballo4.obj");

	//Modelo Puesto de tacos
	puesto_tacos = Model();
	puesto_tacos.LoadModel("Models/Comida/puesto_tacos.obj");
	trompo_pastor = Model();
	trompo_pastor.LoadModel("Models/Comida/Trompo.obj");
	palomitas = Model();
	palomitas.LoadModel("Models/Palomitas/palomitas.obj");

	//MODELOS ROTULOS
	rotulo_perry = Model();
	rotulo_perry.LoadModel("Models/Rotulos/rotulo_perry.obj");
	rotulo_mario = Model();
	rotulo_mario.LoadModel("Models/Rotulos/rotulo_mario.obj");
	rotulo_hollow = Model();
	rotulo_hollow.LoadModel("Models/Rotulos/rotulo_hollow.obj");

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
	//JUEGO DE GLOBOS
	carpa = Model();
	carpa.LoadModel("Models/Globos/carpa.obj");
	globo1 = Model();
	globo1.LoadModel("Models/Globos/globo1.obj");
	globo2 = Model();
	globo2.LoadModel("Models/Globos/globo2.obj");
	globo3 = Model();
	globo3.LoadModel("Models/Globos/globo3.obj");
	tronco = Model();
	tronco = Model();
	tronco.LoadModel("Models/Globos/tronco.obj");
	hornet = Model();
	hornet.LoadModel("Models/Globos/Hornet.obj");
	linea_H = Model();
	linea_H.LoadModel("Models/Globos/Hornet_linea.obj");
	dardo = Model();
	dardo.LoadModel("Models/Globos/dardo.obj");

	//JUEGO DE TOPOS
	topos = Model();
	topos.LoadModel("Models/Topo/Whack.obj");
	topito = Model();
	topito.LoadModel("Models/Topo/diglett.obj");
	mazo = Model();
	mazo.LoadModel("Models/Topo/mazo.obj");

	//JUEGO DE BOLOS
	linea_bolos = Model();
	linea_bolos.LoadModel("Models/Bolos/bolos.obj");
	pino = Model();
	pino.LoadModel("Models/Bolos/pino.obj");
	voltorb = Model();
	voltorb.LoadModel("Models/Bolos/voltorb.obj");

	//JUEGO JAULA DE BATEO
	jaula = Model();
	jaula.LoadModel("Models/JaulaBateo/jaula.obj");
	bate = Model();
	bate.LoadModel("Models/JaulaBateo/bate.obj");
	lanzabolas = Model();
	lanzabolas.LoadModel("Models/JaulaBateo/lanzapelotas.obj");
	guardabolas = Model();
	guardabolas.LoadModel("Models/JaulaBateo/guardapelotas.obj");
	pokeball = Model();
	pokeball.LoadModel("Models/JaulaBateo/pokeball.obj");

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
	Material_metalico = Material(0.9f, 256); //muy brillante
	Material_tela = Material(0.05f, 2);  // Tela
	Material_madera = Material(0.2f, 16);      //poco reflejo y brillo moderado
	Material_ceramica = Material(0.6f, 64);    //superficie lisa con brillo medio-alto
	Material_planta = Material(0.1f, 8);       //superficie mate con poco brillo


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.4f, 0.4f,
		0.0f, -1.0f, 0.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.4f, 0.4f,
		-168.0f, 30.0f, 168.0f,
		0.3f, 0.05f, 0.01f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//----------------------------------------------------- LUCES DE NOCHE ------------------------------------------

	unsigned int lucesFarosCount = 0;

	lucesFaros[0] = SpotLight(1.0f, 0.2f, 1.0f,       // Magenta fuerte
		1.5f, 2.5f,                                   // Intensidad: visible pero no deslumbrante
		49.0f, 60.0f, 275.0f,                         // Posición
		0.0f, -0.5f, -1.0f,                           // Dirección inclinada hacia el suelo
		0.01f, 0.005f, 0.00005f,                     // Atenuación suave: larga distancia
		35.0f);
	lucesFarosCount++;

	lucesFaros[1] = SpotLight(0.2f, 0.6f, 1.0f,       // Azul eléctrico
		1.5f, 2.5f,
		-39.0f, 60.0f, 275.0f,
		0.0f, -0.5f, -1.0f,
		0.01f, 0.005f, 0.00005f,
		35.0f);
	lucesFarosCount++;

	lucesFaros[2] = SpotLight(1.0f, 1.0f, 0.0f,       // Amarillo bonito
		1.5f, 3.5f,
		250.0f, 65.0f, -20.0f,
		-1.0f, 0.0f, 0.0f,
		0.01f, 0.005f, 0.00005f,
		35.0f);
	lucesFarosCount++;

	lucesFaros[3] = SpotLight(0.0f, 1.0f, 0.4f,     // Verde feria brillante
		0.5f, 0.7f,
		0.0f, 60.0f, 0.0f,                           // Posición temporal (se actualiza y se queda en la ultima posicion)
		0.0f, -1.0f, 0.0f,
		0.01f, 0.005f, 0.0005f,
		50.0f);
	lucesFarosCount++;

	//unsigned int lucesFeriaCount = 0;

	//lucesFeria[0] = PointLight(1.0f, 0.8f, 0.6f,
	//	0.6f, 0.4f,
	//	-150.0f, 60.0f, 150.0f,   // Esquina inferior izquierda
	//	0.02f, 0.002f, 0.0003f);
	//lucesFeriaCount++;

	//lucesFeria[1] = PointLight(1.0f, 0.8f, 0.6f,
	//	0.6f, 0.4f,
	//	150.0f, 60.0f, 150.0f,    // Esquina inferior derecha
	//	0.02f, 0.002f, 0.0003f);
	//lucesFeriaCount++;

	//lucesFeria[2] = PointLight(1.0f, 0.8f, 0.6f,
	//	0.6f, 0.4f,
	//	-140.0f, 60.0f, -110.0f,  // Esquina superior izquierda
	//	0.02f, 0.002f, 0.0003f);
	//lucesFeriaCount++;

	//lucesFeria[3] = PointLight(1.0f, 0.8f, 0.6f,
	//	0.6f, 0.4f,
	//	140.0f, 60.0f, -110.0f,   // Esquina superior derecha
	//	0.02f, 0.002f, 0.0003f);
	//lucesFeriaCount++;

	//-------------------------------------------------------LUCES PARA LOS DADOS ----------------------------------------------
	unsigned int lucesDadosCount = 0;

	lucesDados[0] = SpotLight(
		1.0f, 0.8f, 0.6f,                // Color calido
		1.5f, 2.0f,
		-100.0f, 30.0f, -200.0f,          // Posición 
		0.0f, -1.0f, 0.0f,             // Dirección 
		0.3f, 0.02f, 0.005f,             // Atenuación
		52.0f                           // Ángulo 
	);
	lucesDadosCount++;

	//---------------------------------------------------LUCES PARA LAS HACHAS ---------------------------------------------
	unsigned int lucesHachaCount = 0;

	lucesHacha[0] = PointLight(1.0f, 0.8f, 0.6f,        // Color cálido
		0.6f, 0.4f,
		-180.0f, 30.0f, -78.0f,
		0.2f, 0.02f, 0.005f);     //Atenuación
	lucesHachaCount++;

	lucesHacha[1] = PointLight(1.0f, 0.8f, 0.6f,
		0.6f, 0.4f,               //Menor brillo
		-200.0f, 20.0f, -80.0f,
		0.2f, 0.02f, 0.005f);
	lucesHachaCount++;

	lucesHacha[2] = PointLight(1.0f, 0.8f, 0.6f,
		0.6f, 0.4f,
		-215.0f, 30.0f, -83.0f,
		0.2f, 0.02f, 0.005f);
	lucesHachaCount++;

	//------------------------------------------------ LUCES PARA LOS GLOBOS --------------------------------------------------
	unsigned int lucesGlobosCount = 0;

	lucesGlobos[0] = PointLight(1.0f, 0.8f, 0.6f,       // Color cálido 
		1.5f, 1.0f,             // Intensidad 
		-175.0f, 15.0f, 180.0f, // Posición
		0.3f, 0.05f, 0.01f);    // Atenuación (mayor alcance)
	lucesGlobosCount++;

	lucesGlobos[1] = PointLight(1.0f, 0.8f, 0.6f,
		1.5f, 1.0f,
		-190.0f, 15.0f, 150.0f,
		0.3f, 0.05f, 0.01f);
	lucesGlobosCount++;

	lucesGlobos[2] = PointLight(1.0f, 0.8f, 0.6f,
		1.5f, 1.0f,
		-199.0f, 15.0f, 105.0f,
		0.3f, 0.05f, 0.01f);
	lucesGlobosCount++;

	//------------------------------------------------ LUCES PARA LA JAULA DE BATEO --------------------------------------------------

	unsigned int lucesJaulaCount = 0;
	unsigned int lucesSpotJaulaCount = 0;

	// Luz en esquina izquierda superior
	lucesJaula[0] = PointLight(1.0f, 0.9f, 0.7f,   // Color cálido
		1.5f, 1.0f,
		85.0f, 25.0f, -210.0f,                    // Posición
		0.3f, 0.05f, 0.01f);                      // Atenuación
	lucesJaulaCount++;

	// Luz en esquina derecha superior
	lucesJaula[1] = PointLight(1.0f, 0.9f, 0.7f,
		1.5f, 1.0f,
		95.0f, 25.0f, -170.0f,
		0.3f, 0.05f, 0.01f);
	lucesJaulaCount++;

	// Spotlight central
	lucesSpotJaula[0] = SpotLight(1.0f, 1.0f, 0.9f,                         // Color cálido blanco
		0.7f, 0.5f,
		115.0f, 30.0f, -175.0f,                    // Posición arriba de la maquina
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);                                   // Ángulo del cono
	lucesSpotJaulaCount++;


	//------------------------------------------------ LUCES PARA LOS TOPOS --------------------------------------------------

	unsigned int lucesToposCount = 0;

	lucesTopos[0] = SpotLight(1.0f, 0.9f, 0.6f,       // Color cálido
		1.5f, 2.0f,
		186.0f, 40.0f, -70.0f,                        // Posición en lo alto de la cabina
		0.0f, -1.0f, 0.0f,                           // Dirección hacia abajo
		0.3f, 0.02f, 0.009f,
		70.0f);                                       // Ángulo del cono
	lucesToposCount++;


	//------------------------------------------------ LUCES PARA LOS BOLOS --------------------------------------------------

	unsigned int lucesBolosCount = 0;

	lucesBolos[0] = SpotLight(1.0f, 1.0f, 1.0f,       // Color blanco
		2.0f, 2.5f,                                   // Intensidad
		170.0f, 30.0f, 155.0f,                        // Posición 
		0.0f, -1.0f, 0.0f,                            // Dirección hacia abajo
		0.3f, 0.02f, 0.01f,
		35.0f);                                       // Ángulo 
	lucesBolosCount++;

	lucesBolos[1] = SpotLight(1.0f, 1.0f, 1.0f,
		2.0f, 2.5f,
		144.0f, 30.0f, 132.0f,                        // Mitad de la pista
		0.0f, -1.0f, 0.0f,
		0.3f, 0.02f, 0.01f,
		35.0f);
	lucesBolosCount++;

	lucesBolos[2] = SpotLight(1.0f, 1.0f, 1.0f,
		2.0f, 2.5f,
		195.0f, 30.0f, 177.0f,                        // Sobre los pinos
		0.0f, -1.0f, 0.0f,
		0.3f, 0.02f, 0.01f,
		35.0f);
	lucesBolosCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	//algunas variables para avatar y cámara
	glm::mat4 view;
	glm::vec3 avatarPos; //posición del avatar
	float rotavatar, rotavatarY;//rotación del avatar
	//animaciones
	int juegoactivo = 0; //VARIABLE QUE NOS DICE EN QUÉ JUEGO HACER ANIMACIÓN.
	float contabasico = 0.0f, monedamovx = 0.0f, monedamovy = 0.0f, animationTime = 0.0f;
	float dado1x = 0.0f, dado1y = 0.0f, dado1z = 0.0f, vasox = 0.0f, vasoy = 0.0f, vasoz = 0.0f, dado2x = 0.0f, dado2y = 0.0f, dado2z = 0.0f, rotavaso, rotadado1, rotadado2;
	float axemovX = 0.0f, axemovZ = 0.0f, axerotX = 0.0f, axerotZ = 0.0f;
	float dardomovX = 0.0f, dardomovY = 0.0f, dardomovZ = 0.0f, dardogiroY = 0.0f, globoscale = 1.0f;
	float muevebate = 0.0f, muevebola = 0.0f, bateOffset = 0.5f, pelotaOffset = 0.4f;
	float muevetopo = 0.0f, muevemazo = 0.0f, toposOffset = 0.1f, mazoOffset = 1.0f;
	float muevepino = 0.0f, muevevoltorb = 0.0f, voltorbOffset = 0.3f, pinoOffset = 2.0f;
	bool globosPonchados = false;
	float brazoPhineas = -100.0f, muevebrazo = 0.0f, brazoOffset = 0.3f;

	//sf::Music music;
	////Reproduccion de musica fuera del ciclo
	//music.openFromFile("soundtrack.wav");
	//music.setVolume(100.f);
	//music.setPitch(1.0f);
	//music.setLoop(true);
	//music.play();

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		float desplazamientoGlobo = sin(glfwGetTime() * 1.5f) * 1.0f; // oscila entre -1 y 1 (multiplica por 1 para que sean 2 unidades de diferencia)

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
			if (mainWindow.getcamtype() >= 2 && mainWindow.getcamtype() < 8) { //camara viendo juego
				skybox_n.DrawSkybox(camara3.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 8) { //camara editor
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
			if (mainWindow.getcamtype() >= 2 && mainWindow.getcamtype() < 8) { //camara viendo juego
				skybox_t.DrawSkybox(camara3.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 8) { //camara editor
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
			if (mainWindow.getcamtype() >= 2 && mainWindow.getcamtype() < 8) { //camara viendo juego
				skybox.DrawSkybox(camara3.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 8) { //camara editor
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
			if (mainWindow.getcamtype() >= 2 && mainWindow.getcamtype() < 8) { //camara viendo juego
				skybox.DrawSkybox(camara3.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 8) { //camara editor
				skybox.DrawSkybox(camara4.calculateViewMatrix(), projection);
			}
		}
		else if (solAng >= 30.0f && solAng < 80) { //amanecer/atardecer
			if (mainWindow.getcamtype() == 0) { //camara 3ra persona
				skybox_t.DrawSkybox(camara1.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 1) { //camara desde arriba
				skybox_t.DrawSkybox(camara2.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() >= 2 && mainWindow.getcamtype() < 8) { //camara viendo juego
				skybox_t.DrawSkybox(camara3.calculateViewMatrix(), projection);
			}
			if (mainWindow.getcamtype() == 8) { //camara editor
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
			camara2.lookAtTarget(glm::vec3(0.0f, 550.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			//Se inicia cámara
			glm::mat4 view = camara2.calculateViewMatrix();
			//se mandan al shader
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
			glUniform3f(uniformEyePosition, camara2.getCameraPosition().x, camara2.getCameraPosition().y, camara2.getCameraPosition().z);

		}
		else if (mainWindow.getcamtype() != 1.0 && mainWindow.getcamtype() != 8.0) { //vista sobre juego
			//PARA LA FUNCIÓN lookAtTarget el primer vector es la posición de la cámara y el segundo vector es la dirección a la que apunta
			//CONDICIÓN JUEGO DADOS
			if (mainWindow.getcamtype() >= 2.0f && mainWindow.getcamtype() < 3.0f) {
				camara3.lookAtTarget(glm::vec3(-92.5f, 22.5f, -180.0f), glm::vec3(-100.0f, 0.0f, -200.0f));
				juegoactivo = 1;
			}
			//CONDICION JUEGO HACHA
			else if (mainWindow.getcamtype() >= 3.0f && mainWindow.getcamtype() < 4.0f) {
				camara3.lookAtTarget(glm::vec3(-150.0f, 18.0f, -55.0f), glm::vec3(-200.0f, 18.0f, -80.0f));
				juegoactivo = 2;
			}
			//CONDICION JUEGO JAULA DE DARDOS
			else if (mainWindow.getcamtype() >= 4.0f && mainWindow.getcamtype() < 5.0f) {
				camara3.lookAtTarget(glm::vec3(-135.0f, 18.0f, 130.0f), glm::vec3(-200.0f, 18.0f, 160.0f));
				juegoactivo = 3;
			}
			//CONDICION JUEGO JAULA DE BATEO
			else if (mainWindow.getcamtype() >= 5.0f && mainWindow.getcamtype() < 6.0f) {
				camara3.lookAtTarget(glm::vec3(30.0f, 18.0f, -190.0f), glm::vec3(200.0f, 18.0f, -190.0f));
				juegoactivo = 4;
			}
			//CONDICION JUEGO TOPOS
			else if (mainWindow.getcamtype() >= 6.0f && mainWindow.getcamtype() < 7.0f) {
				camara3.lookAtTarget(glm::vec3(120.0f, 40.0f, -30.0f), glm::vec3(180.0f, 0.0f, -50.0f));
				juegoactivo = 5;
			}
			//CONDICION JUEGO BOLOS
			else if (mainWindow.getcamtype() >= 7.0f && mainWindow.getcamtype() < 8.0f) {
				camara3.lookAtTarget(glm::vec3(95.0f, 35.0f, 100.0f), glm::vec3(150.0f, 5.0f, 130.0f));
				juegoactivo = 6;
			}
			printf("\ncamtype, %f", mainWindow.getcamtype());
			//Se inicia cámara
			glm::mat4 view = camara3.calculateViewMatrix();
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
			glUniform3f(uniformEyePosition, camara3.getCameraPosition().x, camara3.getCameraPosition().y, camara3.getCameraPosition().z);
		}
		else if (mainWindow.getcamtype() == 8) { //vista editor
			//para cámara editor se usa la cámara de siempre
			camara4.keyControl(mainWindow.getsKeys(), deltaTime);
			camara4.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			camara4.keyControl(mainWindow.getsKeys(), deltaTime);
			camara4.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camara4.calculateViewMatrix()));
			glUniform3f(uniformEyePosition, camara4.getCameraPosition().x, camara4.getCameraPosition().y, camara4.getCameraPosition().z);
		}

		shaderList[0].SetDirectionalLight(&mainLight);

		//------------------------------------------------------------------- ACTIVACION DE LUCES ------------------------------------------

		bool esNoche = (solAng >= 80.0f && solAng < 250.0f);
		int camType = mainWindow.getcamtype();

		// ----- LUCES PUNTUALES --------
		/*if ((camType == 0 || camType == 1 || camType == 8) && esNoche) {
			shaderList[0].SetPointLights(lucesFeria, lucesFeriaCount);
		}
		else */if (camType == 3 && juegoactivo == 2 && esNoche) {
			shaderList[0].SetPointLights(lucesHacha, lucesHachaCount);
		}
		else if (camType == 4 && juegoactivo == 3 && esNoche) {
			shaderList[0].SetPointLights(lucesGlobos, lucesGlobosCount);
		}
		else {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}

		// ----- LUCES SPOTLIGHTS -----
		if (camType == 2 && juegoactivo == 1 && esNoche) {
			shaderList[0].SetSpotLights(lucesDados, lucesDadosCount);
		}
		else if (camType == 6 && juegoactivo == 5 && esNoche) {
			shaderList[0].SetSpotLights(lucesTopos, lucesToposCount);
		}
		else if (camType == 7 && juegoactivo == 6 && esNoche) {
			shaderList[0].SetSpotLights(lucesBolos, lucesBolosCount);
		}
		else {
			shaderList[0].SetSpotLights(spotLights, spotLightCount);
		}

		// -------- LUCES DE LA JAULA DE BATEO -------
		if (camType == 5 && juegoactivo == 4 && esNoche) {
			shaderList[0].SetPointLights(lucesJaula, lucesJaulaCount);
			shaderList[0].SetSpotLights(lucesSpotJaula, lucesSpotJaulaCount);
		}

		// ----------- LUCES DE NOCHE ---------------------------
		//Movimiento senoidal de los faros
		float tiempo = glfwGetTime();
		float anguloFaro1 = std::sin(tiempo * 1.5f) * glm::radians(20.0f); // Oscilación en eje X
		float anguloFaro2 = std::cos(tiempo * 1.5f) * glm::radians(20.0f);

		// Posiciones de los faros 
		glm::vec3 posFaro1(49.0f, 60.0f, 275.0f);
		glm::vec3 posFaro2(-39.0f, 60.0f, 275.0f);

		//Dirección base
		glm::vec3 direccionBase = glm::normalize(glm::vec3(0.0f, -0.3f, -1.0f));

		//Matrices de rotación sobre eje X (para que inclinen hacia los lados)
		glm::mat4 rotX1 = glm::rotate(glm::mat4(1.0f), anguloFaro1, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotX2 = glm::rotate(glm::mat4(1.0f), anguloFaro2, glm::vec3(1.0f, 0.0f, 0.0f));

		// Direcciones resultantes aplicando la rotación a la dirección base
		glm::vec3 direccionFaro1 = glm::vec3(rotX1 * glm::vec4(direccionBase, 0.0f));
		glm::vec3 direccionFaro2 = glm::vec3(rotX2 * glm::vec4(direccionBase, 0.0f));

		//Actualización de luces Spotlight
		lucesFaros[0].SetFlash(posFaro1, glm::normalize(direccionFaro1));
		lucesFaros[1].SetFlash(posFaro2, glm::normalize(direccionFaro2));


		if ((camType == 0 || camType == 1 || camType == 8) && esNoche)
		{
			glm::vec3 posicionesJuegos[] = {
				glm::vec3(0.0f),                      // índice 0 (no se usa)
				glm::vec3(165.0f, 60.0f, 145.0f),      // juego bolos
				glm::vec3(165.0f, 60.0f, -40.0f),     // juego topos
				glm::vec3(90.0f, 60.0f, -190.0f),      // juego bateo
				glm::vec3(-100.0f, 60.0f, -200.0f),     // juego dados
				glm::vec3(-200.0f, 60.0f, -80.0f),     // juego hachas
				glm::vec3(-185.0f, 60.0f, 160.0f)     // juego globos
			};

			glm::vec3 nuevaPos = posicionesJuegos[mainWindow.getIndiceLuzVerde()];
			glm::vec3 direccion = glm::vec3(0.0f, -1.0f, 0.0f);  // Luz apuntando hacia abajo
			lucesFaros[3].SetFlash(nuevaPos, direccion);
		}

		//Activación de luces generales de la feria
		if ((camType == 0 || camType == 1 || camType == 8) && esNoche) {
			shaderList[0].SetSpotLights(lucesFaros, lucesFarosCount);
		}

		//Función para ciclo dia/noche usando solAng
		solAng += VelSol * deltaTime; //se aumenta el angulo del sol con forme al deltatime y la velocidad del sol
		if (solAng > 360.0f) //al llegar a 360 se reinicia el día
			solAng -= 360.0f;

		glm::vec3 baseDirection = glm::vec3(0.0f, -1.0f, 0.0f); //dirección inicial
		glm::mat4 rotadia = glm::rotate(glm::mat4(1.0f), glm::radians(solAng), glm::vec3(1.0f, 0.0f, 0.0f)); //rotación de la dirección
		glm::vec3 newDirection = glm::vec3(rotadia * glm::vec4(baseDirection, 0.0f)); //se rota la dirección base 
		mainLight.direction = glm::normalize(newDirection); // se normaliza para ajustar

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

		//ANIMACIÓN JUEGO DADOS
		if (juegoactivo == 1 && monedamovy < -7.0f) { //se verifica que estemos en ese juego
			animationTime += deltaTime * 0.01;
			// Fases de la animación
			if (animationTime < 2.0f) {
				vasoy = (animationTime / 2.0f) * 3.0f;  // Subida de 3 unidades
			}
			else if (animationTime < 4.0f) {
				// Fase 2: El vaso se menea (oscila)
				vasoy = 3.0f;
				rotavaso = sin(animationTime * 10.0) * 20.0f;  // Oscilación
			}
			else if (animationTime < 7.0f) {
				// Fase 3: Lanza los dados
				vasoy = 3.0f; //sube vaso
				rotavaso = 110.0f;  // Inclinación del vaso hacia adelante
				vasox = 1.0f;
				float t = animationTime - 4.0f;
				t *= 2.0f;
				//movimiento en Y, limitado a 5.0
				dado1y = glm::min(t * 3.0f, 5.0f);
				dado2y = glm::min(t * 3.0f, 5.0f);
				//movimiento en z limitada a -5.0
				dado1z = glm::max(-1.5f * t * t - 0.5f, -10.0f);
				dado2z = glm::max(-1.5f * t * t - 0.5f, -9.0f);
				//separación leve en x para que no caigan en el mismo punto
				dado1x = 0.3f;
				dado2x = -0.3f;
				// Rotación sencilla mientras caen
				if (t < 3.0f) {
					rotadado1 = t * 75.0;
					rotadado2 = t * 75.0f;
				}
			}
		}
		else { //en caso de que no, variables no se modifican.
			dado1x = 0.0f, dado1y = 0.0f, dado1z = 0.0f, rotadado1 = 0.0f;
			dado2x = 0.0f, dado2y = 0.0f, dado2z = 0.0f, rotadado2 = 0.0f;
			vasox = 0.0f, vasoy = 0.0f, vasoy = 0.0f, rotavaso = 0.0f;
			animationTime = 0.0f;

		}

		//ANIMACION MONEDA AÑADIR ACTIVADOR CON TECLA
		if (mainWindow.getdentrojuego() == 1.0f) {
			if (monedamovx <= 20.0F) {
				monedamovx += 0.02f * deltaTime * 8.0f;
				monedamovy -= 0.006f * deltaTime * 8.0f;
			}
			if (monedamovx > 20.0f) {
				monedamovy -= 0.006f * deltaTime * 8.0f;
			}
		}
		else { monedamovx = 0.0f; monedamovy = 0.0f; }

		//ANIMACION DE HACHA(AXE)
		if (juegoactivo == 2 && monedamovy < -7.0f) {
			if (axemovX >= -40.0f) {
				axemovX -= 1.0f * deltaTime * 0.5f;
				axemovZ -= 0.5f * deltaTime * 0.5f;
				axerotX += 1.0f * deltaTime * 5.0f;
			}
			if (axemovX < -40.0f) {
				axemovX -= 1.0f * deltaTime * 0.0f;
				axemovZ -= 0.5f * deltaTime * 0.0f;
				axerotX += 1.0f * deltaTime * 0.0f;
			}
		}
		else { axemovX = 0.0f, axemovZ = 0.0f, axerotX = 0.0f; }

		// ANIMACIÓN DE DARDOS
		if (juegoactivo == 3 && monedamovy < -7.0f) {
			if (dardomovX >= -43.0f) {
				dardomovX -= 1.0f * deltaTime * 0.5f;
				dardomovZ += 0.5f * deltaTime * 0.5f;
				dardogiroY += 10.0f * deltaTime * 0.5f;
			}
			if (dardomovX < -43.0f) {
				globosPonchados = true;  // Marcar globos como ponchados
			}
		}
		else {
			dardomovX = 0.0f;
			dardomovZ = 0.0f;
			dardogiroY = 0.0f;
			globosPonchados = false;  // Reinicia si sales del juego
		}

		//ANIMACION DE JAULA DE BATEO
		if (juegoactivo == 4 && monedamovy < -7.0f) {
			muevebate += bateOffset * deltaTime;
			muevebola += pelotaOffset * deltaTime;
			if (muevebate > 50.0f) {
				muevebate = 50.0f; //Limite para evitar atasco
				bateOffset *= -1.0f;

			}
			else if (muevebate < -30.0f) {
				muevebate = -30.0f; //Limite para evitar atasco
				bateOffset *= -1.0f;
			}
			if (muevebola > 0.0f) {
				muevebola = 0.0f; //Limite para evitar atasco
				pelotaOffset *= -1.0f;

			}
			else if (muevebola < -40.0f) {
				muevebola = -40.0f; //Limite para evitar atasco
				pelotaOffset *= -1.0f;
			}
		}
		else { muevebate = 0.0f, muevebola = 0.0f, bateOffset = 0.5f, pelotaOffset = 0.4f; }

		//ANIMACION DE TOPOS
		if (juegoactivo == 5 && monedamovy < -7.0f) {
			muevetopo += toposOffset * deltaTime;
			muevemazo += mazoOffset * deltaTime;
			if (muevetopo > 2.3f) {
				muevetopo = 2.3f; //Limite para evitar atasco
				toposOffset *= -1.0f;

			}
			else if (muevetopo < -1.0f) {
				muevetopo = -1.0f; //Limite para evitar atasco
				toposOffset *= -1.0f;
			}
			if (muevemazo > 50.0f) {
				muevemazo = 50.0f; //Limite para evitar atasco
				mazoOffset *= -1.0f;

			}
			else if (muevemazo < -50.0f) {
				muevemazo = -50.0f; //Limite para evitar atasco
				mazoOffset *= -1.0f;
			}

		}

		//ANIMACION BOLOS
		if (juegoactivo == 6 && monedamovy < -7.0f) {
			muevevoltorb += voltorbOffset * deltaTime;
			if (muevevoltorb > 78.0f) {
				muevevoltorb = 78.0f;
				voltorbOffset *= 0.0;
			}
			if (muevevoltorb > 65.0f) {
				muevepino += pinoOffset * deltaTime;
				if (muevepino > 90.0f) {
					muevepino = 90.0f;
					pinoOffset *= 0.0f;
				}
			}
		}
		else { muevepino = 0.0f, muevevoltorb = 0.0f, voltorbOffset = 0.3f, pinoOffset = 2.0f; }

		if (!globosPonchados) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-192.0f, 14.5f + desplazamientoGlobo, 148.0f));
			model = glm::rotate(model, 115 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			globo2.RenderModel();

			model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f)); // z, y, x
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			globo1.RenderModel();
		}

		//Brazos de Phineas
		if (juegoactivo != 0 && monedamovy < -7.0f) {
			brazoPhineas = 0.0f;
		}
		else {
			brazoPhineas = -100.0f;
		}

		//ANIMACION BRAZOS
		if (juegoactivo != 0 && monedamovy < -7.0f) {
			muevebrazo += (brazoOffset * deltaTime) * 6.0f;
			if (muevebrazo > 50.0f) {
				muevebrazo = 50.0f;
			}
		}
		else { muevebrazo = 0.0f, brazoOffset = 0.3f; }


		// ---------------------------------------------------------- AVATAR -------------------------------------------------------------------
		//PHINEAS 
		//cuerpo
		model = glm::mat4(1.0);
		//Posición se ajusta para el avatar
		model = glm::translate(model, glm::vec3(0.0f - mainWindow.getposlat(), 5.5f, 295.0f - mainWindow.getposfron())); //mov del avatar (cambiar a phineas)
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
		contabasico += 0.45f * deltaTime;
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


		//LUCARIO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(170.0f, 19.8f, 40.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -100 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lucario.RenderModel();
		//brazo derecho
		float rotabrazo = sin(now * 3.0f) * glm::radians(-20.0f); // rotación senoidal entre -45° y 45°
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.3f, 3.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rotabrazo, glm::vec3(1.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazo_der_luc.RenderModel();
		//brazo izquierdo
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.3f, 3.0f, 0.0f));
		model = glm::rotate(model, rotabrazo / 2, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazo_izq_luc.RenderModel();

		//HOLOW KNIGHT
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-202.0f, 0.0f, 43.0f));
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		//modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuerpo.RenderModel();
		//MANO IZQUIERDA
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, 22.0f, 3.6f));
		model = glm::rotate(model, 20 * toRadians * desplazamientoGlobo, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mano_izq.RenderModel();
		//CAPA
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.3f, 18.0f, 3.2f));
		model = glm::rotate(model, 10.0f * toRadians * rotabrazo, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		capa.RenderModel();


		// ---------------------------------------------------------------- ENTORNO ----------------------------------------------------------------
		//Valla
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-300.0f, -1.8f, -290.0f));
		Material_madera.UseMaterial(uniformSpecularIntensity, uniformShininess);
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
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_planta.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol_central.RenderModel();

		//Entrada de la feria
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 23.0f, 185.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_madera.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		entrada.RenderModel();
		// FARO 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(49.0f, 60.0f, 275.0f));
		model = glm::rotate(model, anguloFaro1, glm::vec3(1.0f, 0.0f, 0.0f));  // rotar en eje X
		model = glm::scale(model, glm::vec3(2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		faro.RenderModel();
		// FARO 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-38.0f, 60.0f, 272.0f));
		model = glm::rotate(model, anguloFaro2, glm::vec3(1.0f, 0.0f, 0.0f));  // también en eje X
		model = glm::scale(model, glm::vec3(2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		faro.RenderModel();


		//PISO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.9f, -10.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piso_piedras.RenderModel();


		//RUEDA DE LA FORTUNA
		float angulo = tiempo * glm::radians(30.0f);  // 30 grados por segundo
		// BASE
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(250.0f, 56.0f, -20.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		base.RenderModel();
		// Centro de la rueda
		glm::mat4 centroRueda = glm::translate(model, glm::vec3(0.0f, 4.5f, 0.0f));
		// RUEDA
		glm::mat4 modelRueda = glm::rotate(centroRueda, angulo, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRueda));
		rueda.RenderModel();

		// Posiciones fijas desde el centro de la rueda
		std::vector<glm::vec3> posicionesCanastas = {
			glm::vec3(0.0f, 2.3f, 11.3f),
			glm::vec3(0.0f, -7.3f, 8.2f),
			glm::vec3(0.0f, -11.3f, 0.0f),
			glm::vec3(0.0f, -7.3f, -8.2f),
			glm::vec3(0.0f, 2.3f, -11.3f),
			glm::vec3(0.0f, 10.3f, -4.3f),
			glm::vec3(0.0f, 10.3f, 4.4f)
		};

		// ---------------- CANASTA 1 PARA LUZ ----------------
		glm::mat4 modelCanasta1 = glm::translate(modelRueda, posicionesCanastas[0]);       // Posición en la rueda girada
		modelCanasta1 = glm::rotate(modelCanasta1, -angulo, glm::vec3(1.0f, 0.0f, 0.0f));  // Contrarrotación para que cuelgue

		// Posición exacta de la canasta 1
		glm::vec3 posCanasta1 = glm::vec3(modelCanasta1[3]);

		// Dirección fija hacia -X
		glm::vec3 direccionCanasta1 = glm::vec3(-1.0f, 0.0f, 0.0f);

		// Actualizar la luz amarilla
		lucesFaros[2].SetFlash(posCanasta1, direccionCanasta1);

		// Render de cada canasta, partiendo SIEMPRE del centro de la rueda girado
		for (const auto& offset : posicionesCanastas) {
			glm::mat4 modelCanasta = glm::translate(modelRueda, offset);                     // posición girada
			modelCanasta = glm::rotate(modelCanasta, -angulo, glm::vec3(1.0f, 0.0f, 0.0f));  // contrarrotación para colgar
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCanasta));
			canastas.RenderModel();
		}


		//CARRUSEL
		float anguloY = tiempo * glm::radians(30.0f);  // giro del carrusel
		float subida = sin(tiempo * 2.0f) * 1.0f;       // subida y bajada senoidal de ±1 unidad
		//base
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-220.0f, 0.0f, -220.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		carrusel.RenderModel();
		//centro que gira
		glm::mat4 centroCarrusel = glm::rotate(model, -anguloY, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(centroCarrusel));
		centro.RenderModel();
		//caballos
		std::vector<glm::vec3> posicionesCaballos = {
			glm::vec3(2.0f, 4.0f, 2.0f),
			glm::vec3(0.0f, 4.0f, 2.0f),
			glm::vec3(0.0f, 4.0f, -2.0f),
			glm::vec3(-2.0f, 4.0f, 0.0f)
		};
		//animacion de los caballos
		std::vector<float> desfases = { 0.0f, 1.0f, 2.0f, 3.0f };

		for (size_t i = 0; i < posicionesCaballos.size(); ++i) {
			glm::vec3 pos = posicionesCaballos[i];
			float subidaCaballo = sin(tiempo * 2.0f + desfases[i]) * 1.0f;

			glm::mat4 modelCaballo = glm::translate(centroCarrusel, pos + glm::vec3(0.0f, subidaCaballo, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCaballo));

			switch (i) {
			case 0: caballo.RenderModel(); break;
			case 1: caballo2.RenderModel(); break;
			case 2: caballo3.RenderModel(); break;
			case 3: caballo4.RenderModel(); break;
			}
		}


		// ------------------------------------------------------------ JUEGOS --------------------------------------------------------
//JUEGO DADOS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 2.85f, -200.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.2f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_tela.UseMaterial(uniformSpecularIntensity, uniformShininess);
		dados_mesa.RenderModel();
		//vaso
		model = glm::translate(model, glm::vec3(3.0f + vasox, 5.0f + vasoy, 3.0f + vasoz));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, rotavaso * toRadians, glm::vec3(0.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_tela.UseMaterial(uniformSpecularIntensity, uniformShininess);
		dados_vaso.RenderModel();
		modelaux = model;
		//dado1
		model = glm::translate(model, glm::vec3(0.5f + dado1x, -0.5f + dado1y, 0.0f + dado1z));
		model = glm::rotate(model, rotadado1 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dados_cubo1.RenderModel();
		//dado2
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f + dado2x, 0.0f + dado2y, 0.0f + dado2z));
		model = glm::rotate(model, rotadado2 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dados_cubo2.RenderModel();
		//Brazo Phineas
		model = modelaux2;
		model = glm::translate(model, glm::vec3(5.3f, brazoPhineas + 0.0f, -3.1f));
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
		model = glm::rotate(model, rotavaso * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		phineas_brazoD.RenderModel();


		//JUEGO HACHA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, -1.5f, -80.0f));
		modelaux = model;
		model = glm::rotate(model, 65 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.7f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CabinaHacha.RenderModel();
		//Hacha Derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-175.0f + axemovX, 14.0f, -77.5f + axemovZ));
		modelaux = model;
		model = glm::rotate(model, -115 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, axerotX * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.2f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Axe.RenderModel();
		//Hacha izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-183.0f + axemovX, 14.0f, -61.0f + axemovZ));
		modelaux2 = model;
		model = glm::rotate(model, -115 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, axerotX * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.2f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Axe.RenderModel();
		//Brazo Derecho 
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.0f - axemovX, brazoPhineas + -1.0f, 1.1f - axemovZ));
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -muevebrazo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		phineas_brazoD.RenderModel();
		//Brazo Izquierdo 
		model = modelaux2;
		model = glm::translate(model, glm::vec3(4.0f - axemovX, brazoPhineas + -1.0f, 1.2f - axemovZ));
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::rotate(model, muevebrazo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		phineas_brazoI.RenderModel();



		//JUEGO GLOBOS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-175.0f, 0.5f, 180.0f));
		model = glm::rotate(model, 115 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_tela.UseMaterial(uniformSpecularIntensity, uniformShininess);
		carpa.RenderModel();
		//GLOBOS
		//model = glm::mat4(1.0);
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, 2.0f + desplazamientoGlobo, -2.0f)); //z, y, x       z en -2.0
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo1.RenderModel();
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo2.RenderModel();
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f)); //z, y, x 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo1.RenderModel();
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo2.RenderModel();
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f)); //z, y, x  
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo1.RenderModel();
		//__
		model = glm::translate(model, glm::vec3(5.0f, 12.0f, 0.0f)); //z, y, x  
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo1.RenderModel();

		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo2.RenderModel();
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f)); //z, y, x    
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo1.RenderModel();
		//__
		model = glm::translate(model, glm::vec3(-5.0f, 12.0f, 0.0f)); //z, y, x   
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo1.RenderModel();
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo2.RenderModel();
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f)); //z, y, x       
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo1.RenderModel();
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo2.RenderModel();
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f)); //z, y, x      
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo1.RenderModel();
		//GLOBO 3
		model = modelaux;
		model = glm::translate(model, glm::vec3(69.0f, 4.0f, -3.5f)); //z, y, x      
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo3.RenderModel();
		model = glm::translate(model, glm::vec3(7.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo3.RenderModel();
		model = glm::translate(model, glm::vec3(7.0f, 0.0f, 0.0f)); //z, y, x      
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo3.RenderModel();
		model = glm::translate(model, glm::vec3(7.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo3.RenderModel();
		//__
		model = glm::translate(model, glm::vec3(0.0f, 12.0f, 0.0f)); //z, y, x     
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo3.RenderModel();
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo3.RenderModel();
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 0.0f)); //z, y, x   

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo3.RenderModel();
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo3.RenderModel();
		//__
		model = glm::translate(model, glm::vec3(0.0f, 12.0f, 0.0f)); //z, y, x 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo3.RenderModel();
		model = glm::translate(model, glm::vec3(7.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo3.RenderModel();
		model = glm::translate(model, glm::vec3(7.0f, 0.0f, 0.0f)); //z, y, x    
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo3.RenderModel();
		model = glm::translate(model, glm::vec3(7.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(1.0f * globoscale, 1.0f * globoscale, 1.0f * globoscale));  NO BORRAR
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		globo3.RenderModel();


		//JUEGO GLOBOS (PARTE DEL DARDO)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f + dardomovX, 15.0f, 140.0f + dardomovZ));
		modelaux2 = model;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -25 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, dardogiroY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		dardo.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-156.0f + dardomovX, 15.0f, 130.0f + dardomovZ));
		modelaux = model;
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -25 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, dardogiroY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		dardo.RenderModel();
		//Brazo Derecho 
		model = modelaux;
		model = glm::translate(model, glm::vec3(11.5f - dardomovX, brazoPhineas + -2.0f, -5.0f - dardomovZ));
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 35 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -muevebrazo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		phineas_brazoD.RenderModel();
		//Brazo Izquierdo 
		model = modelaux2;
		model = glm::translate(model, glm::vec3(11.5f - dardomovX, brazoPhineas +  -2.0f, -5.0f - dardomovZ));
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 35 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, muevebrazo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		phineas_brazoI.RenderModel();
		//TAQUILLA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-155.0f, 3.0f, 215.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 130 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		Material_madera.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tronco.RenderModel();
		//HORNET
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		hornet.RenderModel();
		//LINEA
		model = glm::translate(model, glm::vec3(0.5f, 10.0f, -0.5f));
		float rotLinea = static_cast<float>(glfwGetTime()) * 1.0f;  // Tiempo en segundos, convertido a float
		model = glm::rotate(model, rotLinea, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		linea_H.RenderModel();


		//JUEGO TOPOS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(165.0f, 5.8f, -40.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -170 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		Material_madera.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		topos.RenderModel();
		//__MAZO
		model = modelaux; 
		model = glm::translate(model, glm::vec3(7.0f, 3.0f, 0.0f));
		modelaux2 = model; 
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, muevemazo * toRadians, glm::vec3(1.0f, 1.0f, 1.0f));
		Material_madera.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mazo.RenderModel();
		//__Topos
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.9f, -muevetopo + 3.0f, -6.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		topito.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.0f, muevetopo + 3.0f, -6.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		topito.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.9f, muevetopo + 3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		topito.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.0f, -muevetopo + 3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		topito.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.9f, -muevetopo + 3.0f, 6.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		topito.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.0f, muevetopo + 3.0f, 6.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		topito.RenderModel();
		//BRAZO PHINEAS
		model = modelaux2;
		model = glm::translate(model, glm::vec3(3.8f, brazoPhineas + -1.0f, 0.5f));
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));   
		model = glm::rotate(model, -80 * toRadians, glm::vec3(1.0f, 0.0f, 1.0f)); 
		model = glm::rotate(model, muevemazo * toRadians, glm::vec3(1.0f, 1.0f, 1.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));  
		phineas_brazoD.RenderModel();


		//SECCIÓN DE MAQUINAS PARA MONEDAS
		/*Todas se animan a la vez, ya que no se podrá ver la ejecución de todas a la vez*/
		//Maquina de monedas sobre juego TOPOS:
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, -60.0f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tragamoneda.RenderModel();
		//MONEDA RELATIVA AL TRAGAMONEDAS
		if (mainWindow.getdentrojuego() == 1.0f) { //moneda solo se dibuja y anima si estás en un juego.
			model = glm::translate(model, glm::vec3(0.1f, 20.0f + monedamovy, -20.0f + monedamovx));
			model = glm::rotate(model, -85 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
			moneda.RenderModel();
		}

		//JUEGO DE JAULA DE BATEO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.0f, 21.0f, -190.0f));
		model = glm::rotate(model, -28 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		jaula.RenderModel();
		//Lanzapelotas
		model = modelaux;
		model = glm::translate(model, glm::vec3(35.0f, -13.0f, 0.0f));
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lanzabolas.RenderModel();
		//Guardapelotas
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 6.0f, -1.5f));
		modelaux2 = model;
		model = glm::rotate(model, sin(contabasico * 0.01f) * 10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		guardabolas.RenderModel();
		//Bate
		model = modelaux;
		model = glm::translate(model, glm::vec3(-15.0f, -10.0f, 7.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -60 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, muevebate * toRadians, glm::vec3(0.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_madera.UseMaterial(uniformSpecularIntensity, uniformShininess);
		bate.RenderModel();
		//Pokeball
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-2.0f, 3.0f, 0.0f));
		model = glm::translate(model, glm::vec3(muevebola + 0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, muevebola * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pokeball.RenderModel();
		//Brazo Phineas 
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.8f, brazoPhineas + -0.0f, 5.3f));
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); 
		model = glm::rotate(model, muevebate * toRadians, glm::vec3(-1.0f, 1.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		phineas_brazoI.RenderModel();

		//SECCIÓN DE MAQUINAS PARA MONEDAS
		/*Todas se animan a la vez, ya que no se podrá ver la ejecución de todas a la vez*/
		//Maquina de monedas sobre juego JAULA DE BATEO:
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, 0.0f, -215.0f));
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tragamoneda.RenderModel();
		//MONEDA RELATIVA AL TRAGAMONEDAS
		if (mainWindow.getdentrojuego() == 1.0f) { //moneda solo se dibuja y anima si estás en un juego.
			model = glm::translate(model, glm::vec3(0.1f, 20.0f + monedamovy, -20.0f + monedamovx));
			model = glm::rotate(model, -85 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
			moneda.RenderModel();
		}

		//JUEGO DE BOLOS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(165.0f, 3.0f, 145.0f));
		model = glm::rotate(model, -130 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_ceramica.UseMaterial(uniformSpecularIntensity, uniformShininess);
		linea_bolos.RenderModel();
		//Voltorb (decoracion)
		model = modelaux;
		model = glm::translate(model, glm::vec3(10.0f, 3.0f, 47.5f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		voltorb.RenderModel();
		//Voltorb
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -0.7f, -muevevoltorb + 30.0f));
		modelaux2 = model; 
		model = glm::rotate(model, -muevevoltorb * 10 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		voltorb.RenderModel();
		//Pino1
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, -40.0f));
		modelaux = model;
		model = glm::rotate(model, -muevepino * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pino.RenderModel();
		//Pino2
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.5f, 0.0f, -1.5f));
		model = glm::rotate(model, -muevepino * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pino.RenderModel();
		//Pino3
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, -1.5f));
		model = glm::rotate(model, -muevepino * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pino.RenderModel();
		//Pino4
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, -3.0f));
		model = glm::rotate(model, -muevepino * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pino.RenderModel();
		//Pino5
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, -muevepino * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pino.RenderModel();
		//Pino6
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.5f, 0.0f, -3.0f));
		model = glm::rotate(model, -muevepino * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pino.RenderModel();
		//BRAZO PHINEAS
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, brazoPhineas + 5.0f, muevevoltorb + 3.6f));
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, muevebrazo * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		phineas_brazoD.RenderModel(); 

		//SECCIÓN DE MAQUINAS PARA MONEDAS
		/*Todas se animan a la vez, ya que no se podrá ver la ejecución de todas a la vez*/
		//Maquina de monedas sobre juego JAULA DE BATEO:
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, 0.0f, 100.0f));
		model = glm::rotate(model, 65 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tragamoneda.RenderModel();
		//MONEDA RELATIVA AL TRAGAMONEDAS
		if (mainWindow.getdentrojuego() == 1.0f) { //moneda solo se dibuja y anima si estás en un juego.
			model = glm::translate(model, glm::vec3(0.1f, 20.0f + monedamovy, -20.0f + monedamovx));
			model = glm::rotate(model, -85 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
			moneda.RenderModel();
		}



		// ------------------------------------------------------- PUESTOS DE COMIDA ---------------------------------------------------
		//Puesto de tacos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, 10.0f, 42.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(13.0f, 13.0f, 13.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		puesto_tacos.RenderModel();
		//Trompo de la carne al pastor
		model = glm::translate(model, glm::vec3(-0.15f, -0.15f, 0.63f));
		model = glm::rotate(model, sin(contabasico * 0.01f) * 10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_tela.UseMaterial(uniformSpecularIntensity, uniformShininess);
		trompo_pastor.RenderModel();


		//PALOMITAS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-37.0f, 1.0f, 65.0f));
		model = glm::rotate(model, -25 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		/*model = glm::rotate(model, 30 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); */
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		/*Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);*/
		palomitas.RenderModel();


		// ------------------------------------------------------------- NPC´S --------------------------------------------------------
		//Pikachu
		float rotacola = sin(now * 4.0f) * glm::radians(-15.0f); // rotación senoidal entre -45° y 45° 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(37.0f, 8.3f, 56.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_tela.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pikachu.RenderModel();
		//Cola
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.8f, 1.0f));
		model = glm::rotate(model, rotacola, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cola.RenderModel();
		//Banquito
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banquito.RenderModel();


		//Charizard
		float rotachar = sin(now * 2.0f) * glm::radians(-40.0f); // rotación senoidal entre -45° y 45° 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.0f, 30.0f + 3.0 * rotachar, 190.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, -145 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		charizard.RenderModel();
		//Ala derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f, 0.6f, -1.2f));
		model = glm::rotate(model, rotachar, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ala_der_char.RenderModel();
		//Ala izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.5f, 0.6f, -1.2f));
		model = glm::rotate(model, -rotachar, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ala_izq_char.RenderModel();

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

		//TOAD---------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 1.0f, 70.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_tela.UseMaterial(uniformSpecularIntensity, uniformShininess);
		modelaux = model;
		toad_cuerpo.RenderModel();
		//brazos
		model = glm::translate(model, glm::vec3(1.0f, 2.0f, 0.0f));
		model = glm::rotate(model, sin(contabasico * 0.06f) * 0.6f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		toad_b_der.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.0f, 2.0f, 0.0f));
		model = glm::rotate(model, -sin(contabasico * 0.06f) * 0.6f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		toad_b_izq.RenderModel();

		//----------------------------------------------------------- OBJETOS DE UNIVERSOS -------------------------------------------------- 
		//ESTRELLA DE MARIO BROS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 2.0f + sin(contabasico * 0.3), 150.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.0f)));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		estrellita.RenderModel();

		//HONGO VERDE DE MARIO BROS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f + sin(contabasico * 0.05) * 20.0f, 0.3f, 150.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_tela.UseMaterial(uniformSpecularIntensity, uniformShininess);
		hongo_verde.RenderModel();

		//MASCARA DE HOLLOW KNIGHT
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 0.5f, -50.0f));
		//model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mask.RenderModel();

		// ------------------------------------------------------------------ ROTULOS --------------------------------------------------------- 
		//Rotulo de Perry 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 8.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rotulo_perry.RenderModel();

		//Rotulo de Mario
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-140.0f, 8.0f, -130.0f));
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rotulo_mario.RenderModel();

		//Rotulo de Hoolow Knight
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, 8.0f, 15.0f));
		model = glm::rotate(model, 75 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rotulo_hollow.RenderModel();


		//---------------------------------------------------------------SECCIÓN DE MAQUINAS PARA MONEDAS----------------------------------------------
		/*Todas se animan a la vez, ya que no se podrá ver la ejecución de todas a la vez*/

		//Maquina de monedas sobre juego DADOS:
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-86.5f, -0.5f, -195.0f));
		model = glm::rotate(model, 195 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tragamoneda.RenderModel();
		//MONEDA RELATIVA AL TRAGAMONEDAS
		if (mainWindow.getdentrojuego() == 1.0f) { //moneda solo se dibuja y anima si estás en un juego.
			model = glm::translate(model, glm::vec3(0.1f, 20.0f + monedamovy, -20.0f + monedamovx));
			model = glm::rotate(model, -85 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
			moneda.RenderModel();
		}

		//Maquina de monedas sobre juego DARDOS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-160.0f, 0.0f, 128.0f));
		model = glm::rotate(model, -63 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tragamoneda.RenderModel();
		//MONEDA RELATIVA AL TRAGAMONEDAS
		if (mainWindow.getdentrojuego() == 1.0f) { //moneda solo se dibuja y anima si estás en un juego.
			model = glm::translate(model, glm::vec3(0.1f, 20.0f + monedamovy, -20.0f + monedamovx));
			model = glm::rotate(model, -85 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
			moneda.RenderModel();
		}

		//Maquina de monedas sobre juego HACHAS:
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-160.0f, 0.0f, -75.0f));
		model = glm::rotate(model, -115 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tragamoneda.RenderModel();
		//MONEDA RELATIVA AL TRAGAMONEDAS
		if (mainWindow.getdentrojuego() == 1.0f) { //moneda solo se dibuja y anima si estás en un juego.
			model = glm::translate(model, glm::vec3(0.1f, 20.0f + monedamovy, -20.0f + monedamovx));
			model = glm::rotate(model, -85 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
			moneda.RenderModel();
		}

		//Maquina de monedas sobre juego TOPOS:
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, -60.0f));
		model = glm::rotate(model, 120 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tragamoneda.RenderModel();
		//MONEDA RELATIVA AL TRAGAMONEDAS
		if (mainWindow.getdentrojuego() == 1.0f) { //moneda solo se dibuja y anima si estás en un juego.
			model = glm::translate(model, glm::vec3(0.1f, 20.0f + monedamovy, -20.0f + monedamovx));
			model = glm::rotate(model, -85 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
			moneda.RenderModel();
		}

		//Maquina de monedas sobre juego JAULA DE BATEO:
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, 0.0f, -215.0f));
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tragamoneda.RenderModel();
		//MONEDA RELATIVA AL TRAGAMONEDAS
		if (mainWindow.getdentrojuego() == 1.0f) { //moneda solo se dibuja y anima si estás en un juego.
			model = glm::translate(model, glm::vec3(0.1f, 20.0f + monedamovy, -20.0f + monedamovx));
			model = glm::rotate(model, -85 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
			moneda.RenderModel();
		}

		//Maquina de monedas sobre juego JAULA DE BATEO:
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, 0.0f, 100.0f));
		model = glm::rotate(model, 65 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tragamoneda.RenderModel();
		//MONEDA RELATIVA AL TRAGAMONEDAS
		if (mainWindow.getdentrojuego() == 1.0f) { //moneda solo se dibuja y anima si estás en un juego.
			model = glm::translate(model, glm::vec3(0.1f, 20.0f + monedamovy, -20.0f + monedamovx));
			model = glm::rotate(model, -85 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_metalico.UseMaterial(uniformSpecularIntensity, uniformShininess);
			moneda.RenderModel();
		}

		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}