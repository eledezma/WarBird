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
	float radians, boundingRadius;
	bool orbital;
	glm::vec3 position;
	glm::mat4 translationMatrix;
	glm::mat4 orientationMatrix;
	glm::mat4 scaleMatrix;

	bool target = false;
	bool destroyed = false;
	bool active = false;
	bool smartMissile = false;
	int missileUpdates = 0;




public:

	Shape3D() {}

	Shape3D(glm::vec3 translationParam, float radiansParam, bool orbitalParam) {
		rotationMatrix = glm::mat4();
		translationMatrix = glm::translate(glm::mat4(), translationParam);
		rotationAxis = glm::vec3(0, 1, 0);
		radians = radiansParam;
		orbital = orbitalParam;
	}

	void setOrientationMatrix(glm::mat4 om) {
		orientationMatrix = om;
	}

	glm::mat4 getOrientationMatrix() {
		if (orbital) {// orbital rotation
			orientationMatrix = (rotationMatrix * translationMatrix);
			return orientationMatrix;
		}
		else{ // center rotation
			orientationMatrix = (translationMatrix * rotationMatrix);
			return orientationMatrix;
		}
	}

	glm::mat4 getTranslation() {
		return translationMatrix;
	}

	void setTranslation(glm::mat4 tm) {
		translationMatrix = tm;
	}



	void orientTowards(glm::mat4 targetsPosition) {
		// all vectors have to be normalized

		glm::vec3 target = getPosition(targetsPosition) - getPosition(getOrientationMatrix());
		glm::vec3 atVec = getIn(getOrientationMatrix());
		target = glm::normalize(target);
		atVec = glm::normalize(atVec);

		glm::vec3 axisOR = glm::cross(target, atVec);
		float direction = axisOR.x + axisOR.y + axisOR.z;
		float rad = glm::acos(glm::dot(target, atVec));

		if (direction > 0) {
			rad = glm::acos(glm::dot(target, atVec));
		}
		else {
			rad = (2 * PI) - glm::acos(glm::dot(target, atVec));
		}

		rotationMatrix = glm::rotate(rotationMatrix, rad, axisOR);
	}

	void Active(bool a) {
		active = a;
	}

	boolean getActive() {
		return active;
	}

	void setRotation(glm::mat4 rm) {
		rotationMatrix = rm;
	}

	glm::mat4 getRotation() {
		return rotationMatrix;
	}


	void setScaleMatrix(glm::vec3 matrix) {
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

	void setBoundingRadius(float b) {
		boundingRadius = b;
	}

	float getBoundingRadius() {
		return boundingRadius;
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

	void gravity() {
		float distance = glm::distance(glm::vec3(translationMatrix[3].x, translationMatrix[3].y, translationMatrix[3].z), glm::vec3(0, 0, 0));
		float g = 90000000 / pow(distance, 2);
		glm::vec3 direction = glm::normalize(glm::vec3(-1 * translationMatrix[3].x, -1 * translationMatrix[3].y, -1 * translationMatrix[3].z));

		translationMatrix = glm::translate(translationMatrix, g*direction);

	}



	void hasTarget(boolean s) {
		target = s;
	}

	boolean getTarget() {
		return target;
	}

	void setSmartMissile(boolean s) {
		smartMissile = s;
	}
	boolean getSmartMissile() {
		return smartMissile;
	}


	void DestroyObject() {
		hasTarget(false);
		Active(false);
		setDestroyed(true);
		setScaleMatrix(glm::vec3(0, 0, 0));
	}

	void setDestroyed(boolean b) {
		destroyed = b;
	}

	boolean getDestroyed() {
		return destroyed;
	}

	virtual void update() {

	

		rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
		//translationMatrix = glm::translate(translationMatrix, translation);
		if (getActive()) {
			missileUpdates++;
			if (missileUpdates == 200) {
				smartMissile = true; //after 200 updates missiles turn smart
			}

			if (missileUpdates == 2000 && getTarget() == false) {
				printf("\n %s", "Missile destroyed for being idle too long");
				DestroyObject();
			}
		}
	}
};