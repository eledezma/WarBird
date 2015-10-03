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

private:

	int id;
	glm::mat4 rotationMatrix;
	glm::mat4 DuoMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 translationMatrix;
	glm::vec3 rotationAxis;
	float radians;
	bool orbital;

public:

	Shape3D(int number) {
		id = number;  // for debugging

		switch (id) { 
		case 0: //Ruber

			
			rotationMatrix = glm::mat4();
			translationMatrix = glm::translate(glm::mat4(), glm::vec3(0, 0, 0));
			rotationAxis = glm::vec3(0, 1, 0);
			radians = glm::radians(0.0f);
			orbital = false;
			printf("Ruber Created\n");
			break;

		case 1: //Unum

			
			rotationMatrix = glm::mat4();
			translationMatrix = glm::translate(glm::mat4(), glm::vec3(4000, 0, 0));
			rotationAxis = glm::vec3(0, 1, 0);
			radians = glm::radians(0.4f);
			orbital = true;
			printf("Unum was Created\n");
			break;

		case 2: //Duo

			rotationMatrix = glm::mat4();
			translationMatrix = glm::translate(glm::mat4(), glm::vec3(-9000, 0, 0));
			DuoMatrix = rotationMatrix * translationMatrix;
			rotationAxis = glm::vec3(0, 1, 0);
			radians = glm::radians(0.2f);
			orbital = true;
			printf("Duo was Created\n");
			break;

		case 3: //Primus

			translationMatrix = glm::translate(glm::mat4(), glm::vec3(900, 0, 0));
			rotationMatrix = glm::mat4();
			rotationAxis = glm::vec3(0, 1, 0);
			radians = glm::radians(0.4f); 
			orbital = true;
			printf("Primus was Created\n");
			break;

		case 4: //Secundus

			translationMatrix = glm::translate(glm::mat4(), glm::vec3(1750, 0, 0));
			rotationMatrix = glm::mat4();
			rotationAxis = glm::vec3(0, 1, 0);
			radians = glm::radians(0.2f);
			orbital = true;
			printf("Primus was Created\n");
			break;

		case 5: //Warbird

			translationMatrix = glm::translate(glm::mat4(), glm::vec3(5000, 1000, 5000));
			rotationMatrix = glm::mat4();
			rotationAxis = glm::vec3(0, 1, 0);
			radians = glm::radians(0.0f);
			orbital = false;
			printf("Warbird was Created\n");
			break;

		case 6: //Missile

			translationMatrix = glm::translate(glm::mat4(), glm::vec3(4900, 1000, 4850));
			rotationMatrix = glm::mat4();
			rotationAxis = glm::vec3(0, 1, 0);
			radians = glm::radians(0.0f);
			orbital = false;
			printf("Missile was Created\n");
			break;

		}


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


		void update() {
			rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
			//translationMatrix = glm::translate(translationMatrix, translation);
		}
	};