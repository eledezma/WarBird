/*
Shape3D.cpp

Represent the scale, translation, and rotation of a 3D shape.
If you overload the constructor you can create a shape with
arguments for scale, translation, and rotation.

Mike Barnes
8/24/2014
*/

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

class Shape3D {

protected:

	glm::mat4 rotationMatrix;
	glm::mat4 DuoMatrix;
	glm::vec3 rotationAxis;
	float radians;
	bool orbital;
	int id;
	glm::vec3 position;
	glm::mat4 translationMatrix;
	glm::mat4 scaleMatrix;


public:

	Shape3D(){}

	Shape3D(glm::vec3 translationParam, float radiansParam, bool orbitalParam ) {
		rotationMatrix = glm::mat4();
		translationMatrix = glm::translate(glm::mat4(), translationParam);
		rotationAxis = glm::vec3(0, 1, 0);
		radians = glm::radians(radiansParam);
		orbital = orbitalParam;
	}

	glm::mat4 getPositionMatrix(){
		if (orbital)
			return rotationMatrix * translationMatrix;
		else
			return translationMatrix * rotationMatrix;
	}

	void setScaleMatrix(glm::vec3 matrix){
		scaleMatrix = glm::scale(glm::mat4(), matrix);
	}

	glm::mat4 getModelMatrix() {
		if (orbital) // orbital rotation
			return(rotationMatrix * translationMatrix * scaleMatrix);
		else  // center rotation
			return(translationMatrix * rotationMatrix * scaleMatrix);
	}

	void warpShip(glm::mat4 m, int planet) {

		if (planet == 1) {
			translationMatrix = m * glm::translate(glm::mat4(), glm::vec3(0.0f, 200.0f, -1500.0f));
			rotationMatrix = glm::rotate(glm::mat4(), 3.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else {
			translationMatrix = m * glm::translate(glm::mat4(), glm::vec3(0.0f, 200.0f, 3500.0f));
			rotationMatrix = glm::rotate(glm::mat4(), -0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}

	void mForward(int speed) {
		glm::vec3 destination = glm::vec3(speed*rotationMatrix[2].x, speed*rotationMatrix[2].y, speed*rotationMatrix[2].z);
		translationMatrix = glm::translate(translationMatrix, glm::vec3(-destination.x, -destination.y, -destination.z));
	}

	void mBack(int speed) {
		glm::vec3 destination = glm::vec3(speed*rotationMatrix[2].x, speed*rotationMatrix[2].y, speed*rotationMatrix[2].z);
		translationMatrix = glm::translate(translationMatrix, glm::vec3(destination.x, destination.y, destination.z));
	}

	void yawRightorLeft(float r) {
		rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
		rotationMatrix = glm::rotate(rotationMatrix, r, rotationAxis);
	}

	void rollRightorLeft(float r) {
		rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
		rotationMatrix = glm::rotate(rotationMatrix, r, rotationAxis);
	}

	void pitchRightorLeft(float r) {
		rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
		rotationMatrix = glm::rotate(rotationMatrix, r, rotationAxis);
	}

	virtual void update() {
		rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
		//translationMatrix = glm::translate(translationMatrix, translation);
	}
};