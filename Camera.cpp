#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_T])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_G])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_F])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_H])
	{
		position += right * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}


glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
//función para seguimiento de cámara, esta requiere rotacionavatar y rotacionavatarY para rotar a los lados y hacia arriba y abajo:
void Camera::followObject(glm::vec3 targetPosition, glm::vec3 offset, float followSpeed, GLfloat deltaTime, float yawDegrees, float pitchDegrees) {
	// Aplicar rotaciones de yaw y pitch
	glm::mat4 rotationYaw = glm::rotate(glm::mat4(1.0f), glm::radians(yawDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationPitch = glm::rotate(glm::mat4(1.0f), glm::radians(pitchDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 Rotacompleta = rotationYaw * rotationPitch;
	// Aplicar rotación al offset para obtener la posición deseada de la cámara
	glm::vec3 rotaOffset = glm::vec3(Rotacompleta * glm::vec4(offset, 0.0f));
	glm::vec3 PosObjetivo = targetPosition + rotaOffset; //posición objetivo
	// Movimiento suavizado de modo que no existan pequeños saltos cuando se mueve el personaje
	position = glm::mix(position, PosObjetivo, 1.0f - glm::exp(-followSpeed * deltaTime));
	// Calcular dirección hacia el objetivo
	glm::vec3 FrenObjetivo = glm::normalize(targetPosition - position); //frente objetivo
	front = glm::normalize(glm::mix(front, FrenObjetivo, 1.0f - glm::exp(-followSpeed * deltaTime)));
	// Recalcular right y up
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
//FUNCIÓN QUE PERMITE CAMBIAR DE POSICIÓN LA CÁMARA SOBRE LOS JUEGO
void Camera::lookAtTarget(glm::vec3 campos, glm::vec3 camdire)
{
	position = campos;
	front = glm::normalize(camdire - position);

	// Recalcular los vectores right y up
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera()
{
}
