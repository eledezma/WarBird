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

	virtual void update() {
		rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
		//translationMatrix = glm::translate(translationMatrix, translation);
	}
};