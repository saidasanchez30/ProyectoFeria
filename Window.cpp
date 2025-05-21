#include "Window.h"
#include <glm.hpp>
#include <algorithm>

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Proyecto Feria - Mundo de Phineas y Ferb", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
	// Asignar valores de la ventana y coordenadas

	//Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	GLfloat deltaTime = 0.0f;
	GLfloat lastTime = 0.0f;
	static double limitFPS = 1.0 / 60.0;
	GLfloat now = glfwGetTime();
	deltaTime = now - lastTime;
	deltaTime += (now - lastTime) / limitFPS;
	lastTime = now;

	// Convertir el ángulo de rotación a radianes para poder hacer calculos correctos
	float rotRad = glm::radians(theWindow->rotavatar);
	// Calcular la dirección hacia en frnte del personaje con respecto a la rotación
	glm::vec3 forwardDir = glm::vec3(sin(rotRad), 0.0f, cos(rotRad));
	glm::vec3 rightDir = glm::vec3(cos(rotRad), 0.0f, -sin(rotRad));
	float rotationAngleD;
	float rotationAngleI;
	float velmov = 1.5f; //variable de velocidad de movimiento del personaje

	//TECLA PRA CAMBIAR DE POSICION LA LUZ NOCTURNA
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		theWindow->indiceLuzVerde++;
		if (theWindow->indiceLuzVerde > 6)
		{
			theWindow->indiceLuzVerde = 1;
		}
	}

	//MOVIMIENTO DEL PERSONAJE
	if (key == GLFW_KEY_A) //izquierda
	{
		theWindow->poslat += rightDir.x * velmov;
		theWindow->posfron += rightDir.z * velmov;
	}
	if (key == GLFW_KEY_D) //derecha
	{
		theWindow->poslat -= rightDir.x * velmov;
		theWindow->posfron -= rightDir.z * velmov;
	}
	if (key == GLFW_KEY_W) //adelante
	{
		theWindow->poslat += forwardDir.x * velmov;
		theWindow->posfron += forwardDir.z * velmov;
		theWindow->rotationAngleD = sin(now * 10.0f) * glm::radians(35.0f); // rotación senoidal entre -45° y 45°
		theWindow->rotationAngleI = sin(now * 10.0f) * glm::radians(-35.0f); // rotación senoidal entre -45° y 45°
	}
	if (key == GLFW_KEY_S) //atras
	{
		theWindow->poslat -= forwardDir.x * velmov;
		theWindow->posfron -= forwardDir.z * velmov;
		theWindow->rotationAngleD = sin(now * 10.0f) * glm::radians(35.0f); // rotación senoidal entre -45° y 45°
		theWindow->rotationAngleI = sin(now * 10.0f) * glm::radians(-35.0f); // rotación senoidal entre -45° y 45°
	}

	//LIMITAR MOVIMIENTO A 600x600 (X y Z entre -300 y 300)
	theWindow->poslat = std::max(-290.0f, std::min(290.0f, theWindow->poslat));
	theWindow->posfron = std::max(0.0f, std::min(595.0f, theWindow->posfron));

	//ROTACIÓN DEL PERSONAJE
	if (key == GLFW_KEY_LEFT) //izq
	{
		theWindow->rotavatar += 2.0f; //rota 2 unidades a la izq
	}
	if (key == GLFW_KEY_RIGHT) //DER
	{
		theWindow->rotavatar -= 2.0f; //rota 2 unidades a la der
	}
	if (key == GLFW_KEY_UP) //arriba
	{
		if (theWindow->rotavatarY <= 50) //límite para evitar rotaciones completas
			theWindow->rotavatarY += 1.5f; //rota 1.5 unidades arriba
	}
	if (key == GLFW_KEY_DOWN) //abajoo
	{
		if (theWindow->rotavatarY >= -40) //limite para evitar rotaciones completas
			theWindow->rotavatarY -= 1.5f; //rota 1.5 unidades abajo
	}

	//CAMBIO DE CÁMARA
	if (key == GLFW_KEY_1) //camara tercera persona
	{
		theWindow->camtype = 0.0f;
		theWindow->dentrojuego = 0.0f;
	}
	if (key == GLFW_KEY_2) //camara desde arriba
	{
		theWindow->camtype = 1.0f;
		theWindow->dentrojuego = 0.0f;
	}
	if (key == GLFW_KEY_3) //camara sobre juego
	{
		if (theWindow->camtype == 0.0f || theWindow->camtype == 1.0f || theWindow->camtype == 8.0f || theWindow->camtype == 7.5f) {
			theWindow->camtype = 2.0f;
			theWindow->dentrojuego = 0.0f;
		}
		else {
			if (theWindow->camtype <= 7.5f) {
				theWindow->camtype += 0.5f;
				theWindow->dentrojuego = 0.0f;
			}
		}
	}
	if (key == GLFW_KEY_4) //camara editor
	{
		theWindow->camtype = 8.0f;
		theWindow->dentrojuego = 0.0f;
	}
	if (key == GLFW_KEY_F) //meter moneda
	{
		theWindow->dentrojuego = 1.0f;
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}