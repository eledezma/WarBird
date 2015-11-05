
class Missle3D : public Shape3D{

private:
	int countOfFramesOfExistence = 0;

public:
	Missle3D() : Shape3D(){}
	Missle3D(glm::vec3 translationParam) : Shape3D(){
		rotationMatrix = glm::mat4();
		rotationAxis = glm::vec3(0, 1, 0);
		radians = 0.0f;
		orbital = false;
		translationMatrix = glm::translate(glm::mat4(), translationParam);
	}
	
	~Missle3D(){} // Default destruction destructor to self destruct

	virtual void update(){
		countOfFramesOfExistence = countOfFramesOfExistence + 1;
		translationMatrix = glm::translate(translationMatrix, glm::vec3(0.0f, 0.0f, -5.0f));

		if (countOfFramesOfExistence == 200){
			// Hack
			translationMatrix = glm::translate(translationMatrix, glm::vec3(100000.0f, 100000.0f, 100000.0f));
		}

	};

	glm::mat4 getPositionMatrix(){
		return translationMatrix;
	}


};