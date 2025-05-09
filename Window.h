#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	//GLfloat getmuevex() { return muevex; }
	//Funciones que retornan variables window
	GLfloat getposlat() { return poslat; } //mov el avatar a los lados
	GLfloat getposfron() { return posfron; } //mov el avatar enfrente y atr�s
	GLfloat getcamtype() { return camtype; } //cambiar la c�mara
	GLfloat getrotavatar() { return rotavatar; } //obtener rotaci�n de avatar
	GLfloat getrotavatarY() { return rotavatarY; } //obtener rotaci�n de avatar del eje Y
	GLfloat getcaminarD() { return rotationAngleD; }
	GLfloat getcaminarI() { return rotationAngleI; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	//Caminata
	GLfloat rotationAngleD;
	GLfloat rotationAngleI;
	//se agrega movimiento al avatar.
	GLfloat poslat = 0.0f, posfron = 0.0f, rotavatar = 0.0f, rotavatarY = 30.0f;
	//para c�mara
	GLfloat camtype = 0.0;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};