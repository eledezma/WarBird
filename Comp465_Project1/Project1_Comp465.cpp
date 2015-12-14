/*
Project1_Comp465.cpp
includes Shape3D.cpp

Project Member Names: Ernie Ledezma, Stanislav Kirdey

Instructions:  Open up the .sln project and execute the .cpp file!

Summary:
Overall everything for Phase 1 was accomplished.
We based our project on the ManyModelsStatic and ManyCubes examples.
We decided to use Shape3D.hpp class from the ManyCubes static since we found that loading models though it was much more efficient.
Using classes and objects helps to make our project more modular and therefore easier to edit for future phases.
And there was rotation logic available in the Shape3D that helped us with our orbital rotations.
As our project stands we have loaded 7 models, 5 of which are all spheres and the other is the custom models.
The moons and planets seem to be orbiting correctly and are at the right spot.
For our missile model we used a ‘missile.tri” shape and for the warbird we have “battleship.tri”. Both were done in AC3D.
Each View is controlled by pressing “v”.  At first the project is loaded in an initial view.
When the user hits v for the first time he enter the “toggle loop” which starts out with “Front View”.
So it might seem like hitting “v” did nothing the first time, but it did.
It simply switched from default view, to the first view on the toggle loop, which is “Front View”.
*/

#include <string>
#include <algorithm>
#include <vector>


# define __Windows__ // define your target operating system
# include "../includes465/include465.hpp"  

# include "Shape3D.hpp"

GLuint showTexture;
bool nowSkybox = true;

// Amount of Shapes
const int nShapes = 29;
const int WarbirdMissileRange = 5000;
int WBmissiles = 10;
int secundusMissiles = 5;
int unumMissiles = 5;
Shape3D * shape[nShapes];
int nextMissile = 6;
int unumMissile = 16;
int secundusMissile = 21;
bool secundusMissileOn = false;
bool unumMissileOn = false;
bool secundusSite = false;
bool unumSite = false;
int counter = 0;
int targetsDown = 0;

// Model for shapes
char * modelFile[nShapes] = {
	"ruber.tri",
	"unum.tri",
	"duo.tri",
	"primus.tri",
	"secundus.tri",
	"battleship.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missile.tri",
	"missileSite.tri",
	"missileSite.tri",
	"box.tri"
};

const int nVertices[nShapes] = {
	264 * 3,
	264 * 3,
	264 * 3,
	264 * 3,
	264 * 3,
	734 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	112 * 3,
	1696 * 3, //missile site unum
	1696 * 3, //missile site secundus
	3072 * 3 // box
};

float modelSize[nShapes] = {
	2000.0f,
	200.0f,
	400.0f,
	100.0f,
	150.0f,
	100.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	25.0f,
	30.0f,
	30.0f,
	50000.0f };

float modelBR[nShapes];  // modelFile's bounding radius

// Position shapes in the world
glm::vec3 translate[nShapes] = {
	glm::vec3(0, 0, 0),
	glm::vec3(4000, 0, 0),
	glm::vec3(-9000, 0, 0),
	glm::vec3(900, 0, 0),
	glm::vec3(1750, 0, 0),
	glm::vec3(5000, 1000, 5000),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(4000, 240, 0),  //missile base unum
	glm::vec3(1750, 190, 0),
	glm::vec3(0, 0, 0) }; //missile site in moon

// Rotation angels
float radians[nShapes] = { 0.0f, 0.004f, 0.002f, 0.004f, 0.002f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.004f, 0.002f, 0.0f };

bool orbital[nShapes] = { false, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false,
false, false, false, false, false, false, false, false, false, false, true, true, false };

// display state and "state strings" for title display
// window title strings
char warbird[50] = "Warbird:  10       ";
char unum[50] = "Unum:  5       ";
char secundus[50] = "Secundus:  5       ";
char us[50] = "U/S:";
char fs[50] = "F/S:";
char view[50] = "View: ";
char winlose[50] = "";
char fpsStr[15];
char viewStr[25] = "Front   ";
char updStr[15];
char titleStr[200];

GLuint VAO[nShapes];      // Vertex Array Objects
GLuint buffer[nShapes];   // Vertex Buffer Objects

GLuint shaderProgram;
char * vertexShaderFile = "simpleVertex.glsl";
char * fragmentShaderFile = "simpleFragment.glsl";
GLuint MVP, modelLoc, viewLoc, projLoc;  // Model View Projection matrix's handle
GLuint vPosition[nShapes], vColor[nShapes], vNormal[nShapes];

glm::vec3 scale[nShapes];
glm::mat4 projectionMatrix;     // set in reshape()
glm::mat4 modelMatrix;          // set in shape[i]-->updateDraw()
glm::mat4 viewMatrix;           // set in keyboard()
glm::mat4 DuoMatrix;
glm::mat4 UnumMatrix;
glm::mat4 WarbirdMatrix;
glm::mat4 ModelViewProjectionMatrix; // set in display();


glm::vec3 missilePos;
glm::vec3 site1Pos;
glm::vec3 site2Pos;
float d1;
float d2;
glm::mat4 currentTarget;
GLboolean debugSetOn;

int cameraSwitch = 0;

bool idleTimerFlag = false;
// vectors and values for lookAt
glm::vec3 eye, at, up;

// rotation variables
glm::mat4 identity(1.0f);
glm::mat4 rotation;
int timerDelay = 40, frameCount = 0;  // A delay of 40 milliseconds is 25 updates / second
double currentTime, lastTime, timeInterval;
double currentTimeU, lastTimeU, timeIntervalU;
int updateCount = 0;
int targetNumber;

int thruster = 10; //thrustuer speed initially at 10
int warp = 1;  //1 = unum, 2 = duo
boolean gravity = false; //set gravity on/off, init off

GLuint HeadLightPosition, HeadLightIntensity, PointLightPosition, PointLightIntensity;
GLuint HeadLightOn;
GLuint PointLightOnLoc;
GLuint AmbientLightOnLoc;


GLboolean hl = false;
GLboolean pl = false;
GLboolean dl = false;
GLboolean al = false;



// load the shader programs, vertex data from model files, create the solids, set initial view
void init() {
	// load the shader programs
	shaderProgram = loadShaders(vertexShaderFile, fragmentShaderFile);
	glUseProgram(shaderProgram);

	// generate VAOs and VBOs
	glGenVertexArrays(nShapes, VAO);
	glGenBuffers(nShapes, buffer);

	// load the buffers from the model files
	for (int i = 0; i < nShapes; i++) {
		if (i == 28){
			vNormal[i] = (-1) * vNormal[i];
		}
		modelBR[i] = loadModelBuffer(modelFile[i], nVertices[i], VAO[i], buffer[i], shaderProgram,
			vPosition[i], vColor[i], vNormal[i], "vPosition", "vColor", "vNormal");
		// set scale for models given bounding radius  
		scale[i] = glm::vec3(modelSize[i] * 1.0f / modelBR[i]);

	}

	modelLoc = glGetUniformLocation(shaderProgram, "modelPos");
	viewLoc = glGetUniformLocation(shaderProgram, "viewPos");
	MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");
	showTexture = glGetUniformLocation(shaderProgram, "IsTexture");

	// initially use a front view
	eye = glm::vec3(0.0f, 10000.0f, 20000.0f);
	at = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	viewMatrix = glm::lookAt(eye, at, up);

	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	lastTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
	// create shape
	for (int i = 0; i < nShapes; i++) {

		shape[i] = new Shape3D(translate[i], radians[i], orbital[i]);


		shape[i]->setBoundingRadius(modelSize[i]);
		shape[i]->setScaleMatrix(scale[i]);
	}
	printf("%d Shapes created \n", nShapes);


	// Set the initial light settings
	GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
	GLint PointLightPositionLoc = glGetUniformLocation(shaderProgram, "PointLightPosition");
	PointLightOnLoc = glGetUniformLocation(shaderProgram, "PointLightOn");
	AmbientLightOnLoc = glGetUniformLocation(shaderProgram, "AmbientLightOn");
	GLint PointLightIntensityLoc = glGetUniformLocation(shaderProgram, "PointLightIntensity");

	HeadLightPosition = glGetUniformLocation(shaderProgram, "HeadLightPosition");
	HeadLightOn = glGetUniformLocation(shaderProgram, "HeadLightOn");
	HeadLightIntensity = glGetUniformLocation(shaderProgram, "HeadLightIntensity");

	glm::vec3 lightPos1 = glm::vec3(getPosition(shape[0]->getOrientationMatrix()));

	glUniform3f(PointLightPositionLoc, 0, 1000, 0);
	glUniform3f(PointLightIntensityLoc, 0.5f, 0.5f, 0.5f);
	glUniform3f(HeadLightIntensity, 0.5f, 0.5f, 0.5f);
	glUniform1f(AmbientLightOnLoc, true);

}

void reshape(int width, int height) {
	float aspectRatio = (float)width / (float)height;
	float FOVY = glm::radians(60.0f);
	glViewport(0, 0, width, height);
	printf("reshape: FOVY = %5.2f, width = %4d height = %4d aspect = %5.2f \n",
		FOVY, width, height, aspectRatio);
	projectionMatrix = glm::perspective(FOVY, aspectRatio, 1.0f, 100000.0f);
}

// update and display animation state in window title
void updateTitle() {
	strcpy(titleStr, warbird);
	strcat(titleStr, unum);
	strcat(titleStr, secundus);
	strcat(titleStr, us);
	strcat(titleStr, updStr);
	strcat(titleStr, fs);
	strcat(titleStr, fpsStr);
	strcat(titleStr, view);
	strcat(titleStr, viewStr);
	strcat(titleStr, winlose);

	glutSetWindowTitle(titleStr);
}

void display() {
	glClearColor(0.5f, 0.5f, 0.1f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);  // show only front faces
	glEnable(GL_DEPTH_TEST);

	//Update model matrices
	for (int m = 0; m < nShapes; m++) {

		if (m == 3 || m == 4 || m == 27) {

			DuoMatrix = shape[2]->getOrientationMatrix();
			modelMatrix = shape[m]->getModelMatrix();
			modelMatrix = DuoMatrix * modelMatrix;

		}
		else {
			modelMatrix = shape[m]->getModelMatrix();
		}

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(shape[m]->getModelMatrix()));
		ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));

		if (m == 28){
			glDepthFunc(GL_LESS);
			glUniform1f(showTexture, 1.0f);
		}
		else{
			glUniform1f(showTexture, 0.0f);
			glDepthFunc(GL_LEQUAL);
		}

		glBindVertexArray(VAO[m]);
		glDrawArrays(GL_TRIANGLES, 0, nVertices[m]);

	}
	DuoMatrix = shape[2]->getOrientationMatrix();
	UnumMatrix = shape[1]->getOrientationMatrix();
	WarbirdMatrix = shape[5]->getOrientationMatrix();

	if (cameraSwitch == 2) {

		glm::mat4 eyeWB = glm::translate(WarbirdMatrix, glm::vec3(0.0f, 300.0f, 1000.0f));
		glm::mat4 atWB = glm::translate(WarbirdMatrix, glm::vec3(0.0f, 300.0f, 0.0f));
		eye = glm::vec3(eyeWB[3].x, eyeWB[3].y, eyeWB[3].z);
		at = glm::vec3(atWB[3].x, atWB[3].y, atWB[3].z);
		up = glm::vec3(WarbirdMatrix[1].x, WarbirdMatrix[1].y, WarbirdMatrix[1].z);
		strcpy(viewStr, "Warbird");
		viewMatrix = glm::lookAt(eye, at, up);

	}

	if (cameraSwitch == 3) {

		eye = glm::vec3(UnumMatrix[3].x, 4000.0f, UnumMatrix[3].z);
		at = glm::vec3(UnumMatrix[3].x, UnumMatrix[3].y, UnumMatrix[3].z);
		up = glm::vec3(1.0f, 0.0f, 0.0f);
		strcpy(viewStr, "Unum");
		viewMatrix = glm::lookAt(eye, at, up);

	}

	if (cameraSwitch == 4) {

		eye = glm::vec3(DuoMatrix[3].x, 4000.0f, DuoMatrix[3].z);
		at = glm::vec3(DuoMatrix[3].x, DuoMatrix[3].y, DuoMatrix[3].z);
		up = glm::vec3(1.0f, 0.0f, 0.0f);
		strcpy(viewStr, "Duo");

		viewMatrix = glm::lookAt(eye, at, up);

	}
	glUniform3f(HeadLightPosition, viewMatrix[0].x, viewMatrix[0].y, viewMatrix[0].z);

	glUniform3f(viewLoc, viewMatrix[1].x, viewMatrix[2].y, viewMatrix[3].z);


	glutSwapBuffers();
	frameCount++;
	// see if a second has passed to set estimated fps information
	currentTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
	timeInterval = currentTime - lastTime;
	if (timeInterval >= 1000) {
		sprintf(fpsStr, "  %4d     ", (int)(frameCount / (timeInterval / 1000.0f)));
		lastTime = currentTime;
		frameCount = 0;
		updateTitle();
	}

}


GLboolean collision(Shape3D * a, Shape3D * b, int target) {


	float d;

	if (target == 27) {
		DuoMatrix = shape[2]->getOrientationMatrix();
		modelMatrix = shape[27]->getModelMatrix();
		modelMatrix = DuoMatrix * modelMatrix;
		d = glm::distance(getPosition(a->getOrientationMatrix()), getPosition(modelMatrix));
	}
	else {
		d = glm::distance(getPosition(a->getOrientationMatrix()), getPosition(b->getOrientationMatrix()));
		/*if (target == 0) {

			printf("\n Distance: %f", d);
			printf("\n radius: %f", b->getBoundingRadius());
			}*/
	}

	if (target > 0 && target < 5) {
		d = glm::distance(getPosition(a->getOrientationMatrix()), getPosition(b->getOrientationMatrix()));
		if (d < b->getBoundingRadius() + a->getBoundingRadius()) {
			return true;
		}
	}
	else {
		if (a->getBoundingRadius() + 300 + b->getBoundingRadius() + 300 > d) { //made bounding radius largers for missiles and ship and sites

			return true;
		}
	}


	return false;
}

void update() {

	if (unumMissileOn == false) {
		glm::vec3 warbird = getPosition(shape[5]->getOrientationMatrix());
		glm::vec3 unumBase = getPosition(shape[26]->getOrientationMatrix());
		d1 = distance(warbird, unumBase);

		if (d1 <= 5000.0f) {
			if (unumMissile < 21) {
				shape[unumMissile]->warpShip(shape[1]->getOrientationMatrix(), 1);
				shape[unumMissile]->Active(true);
				shape[unumMissile]->hasTarget(true);
				unumMissileOn = true;
				unumMissiles--;
				sprintf(unum, "Unum:  %i       ", unumMissiles);


			}
		}


	}

	for (int i = 0; i < nShapes; i++) {
		if (i > 15 && i < 21) {
			if (shape[i]->getActive()) {
				shape[i]->orientTowards(shape[5]->getOrientationMatrix());
				shape[i]->mForward(40);

				if (collision(shape[5], shape[i], 5)) {
					shape[i]->DestroyObject();
					unumMissileOn = false;
					unumMissile++;
					sprintf(winlose, "%s", "  Cadet resigns from the war college");
				}
			}
		}

	}


	if (secundusMissileOn == false) {
		glm::vec3 warbird = getPosition(shape[5]->getOrientationMatrix());
		DuoMatrix = shape[2]->getOrientationMatrix();
		modelMatrix = shape[27]->getModelMatrix();
		modelMatrix = DuoMatrix * modelMatrix;
		glm::vec3 secundusBase = getPosition(modelMatrix);
		d2 = distance(warbird, secundusBase);

		if (d2 <= 5000.0f) {
			printf("\n D2");
			if (secundusMissile < 27) {

				shape[secundusMissile]->warpShip(shape[2]->getOrientationMatrix(), 2);
				shape[secundusMissile]->Active(true);
				shape[secundusMissile]->hasTarget(true);
				secundusMissileOn = true;
				secundusMissiles--;
				sprintf(secundus, "Secundus:  %i       ", secundusMissiles);

			}
		}

	}

	for (int i = 0; i < nShapes; i++) {
		if (i > 20 && i < 27) {
			if (shape[i]->getActive()) {
				shape[i]->orientTowards(shape[5]->getOrientationMatrix());
				shape[i]->mForward(40);

				if (collision(shape[5], shape[i], 5)) {
					shape[i]->DestroyObject();
					secundusMissileOn = false;
					secundusMissile++;
					sprintf(winlose, "%s", "  Cadet resigns from war college");
				}
			}
		}

	}

	for (int i = 0; i < nShapes; i++) {

		if (i >= 0 && i < 5) {

			if (collision(shape[5], shape[i], i)) {

				sprintf(winlose, "%s", "  Cadet resigns from the war college");
			}
		}
	}

	for (int i = 0; i < nShapes; i++) {

		shape[i]->update();



		if (shape[i]->getActive()) {
			if (shape[i]->getSmartMissile()) {

				if (shape[i]->getTarget() == false) {  //if no target look for it
					//printf("\n %f", d1);
					if (shape[26]->getDestroyed() == false) {
						site1Pos = getPosition(shape[26]->getOrientationMatrix());
						missilePos = getPosition(shape[i]->getOrientationMatrix());
						d1 = distance(site1Pos, missilePos);

						if (d1 < 5000) {
							targetNumber = 26;
							currentTarget = shape[26]->getOrientationMatrix();
							shape[i]->orientTowards(currentTarget);
							shape[i]->hasTarget(true);

						}

					}

					if (shape[27]->getDestroyed() == false) {
						DuoMatrix = shape[2]->getOrientationMatrix();
						modelMatrix = shape[27]->getModelMatrix();
						modelMatrix = DuoMatrix * modelMatrix;
						missilePos = getPosition(shape[i]->getOrientationMatrix());

						site2Pos = getPosition(modelMatrix);
						d2 = distance(site2Pos, missilePos);


						if (d2 < 5000) {
							targetNumber = 27;
							currentTarget = modelMatrix;
							shape[i]->orientTowards(currentTarget);
							shape[i]->hasTarget(true);

						}
					}

					shape[i]->mForward(40);  //keep moving at 20 speed regardless of orientation
				}

				else {  //target has been found
					if (targetNumber == 27) {
						currentTarget = modelMatrix;
					}
					else {
						currentTarget = shape[targetNumber]->getOrientationMatrix();
					}

					shape[i]->orientTowards(currentTarget);
					shape[i]->mForward(40);//once target is on lock, pursue it

					if (collision(shape[i], shape[targetNumber], targetNumber)) {
						printf("\n Target Destroyed");
						targetsDown++;
						shape[targetNumber]->setScaleMatrix(glm::vec3(0, 0, 0));  //destroy both missile and target if collided
						shape[targetNumber]->DestroyObject();
						shape[i]->DestroyObject();
						if (targetNumber == 26) {
							unumMissileOn = true;
						}
						if (targetNumber == 27) {
							secundusMissileOn = true;
						}
					}
				}

			}
			else {
				shape[i]->mForward(40);  //intial move before becoming smart
			}
		}
	}
	if (gravity) {
		shape[5]->gravity();
	}

	if (targetsDown == 2) {
		sprintf(winlose, "%s", "  Cadet passes flight training!");

	}

	updateCount++;
	counter++;
	currentTimeU = glutGet(GLUT_ELAPSED_TIME);
	timeIntervalU = currentTimeU - lastTimeU;


	if (timeIntervalU >= 1000) {
		sprintf(updStr, "%4d     ", (int)(updateCount / (timeIntervalU / 1000.0f)));
		lastTimeU = currentTimeU;
		updateCount = 0;
		updateTitle();
	}
	glUniform3f(HeadLightPosition, viewMatrix[3].x, viewMatrix[3].y, viewMatrix[3].z);
	glutPostRedisplay();
}

void intervalTimer(int i) {
	glutTimerFunc(timerDelay, intervalTimer, 1);
	if (!idleTimerFlag) update();
}

// Quit or set the view
void keyboard(unsigned char key, int x, int y) {

	switch (key) {

	case 'a': case 'A':

		if (al == false){
			al = true;
			glUniform1f(AmbientLightOnLoc, true);
		}
		else if (al == true) {
			al = false;
			glUniform1f(AmbientLightOnLoc, false);
		}

		break;

	case 'h': case 'H':

		if (hl == false){
			hl = true;
			glUniform1f(HeadLightOn, true);
		}
		else if (hl == true) {
			hl = false;
			glUniform1f(HeadLightOn, false);
		}

		break;

	case 'p': case 'P':

		if (pl == false){
			pl = true;
			glUniform1f(PointLightOnLoc, true);
		}
		else if (pl == true) {
			pl = false;
			glUniform1f(PointLightOnLoc, false);
		}

		break;


	case 033: case 'q':  case 'Q': exit(EXIT_SUCCESS); break;

	case 's': case'S':
		if (thruster == 10) {

			thruster = 50;
		}
		else if (thruster == 50) {

			thruster = 200;
		}
		else if (thruster == 200) {

			thruster = 10;
		}

		break;

	case 'g': case 'G':
		if (gravity) {
			gravity = false;

		}
		else {
			gravity = true;
		}
		break;

	case 't': case 'T':
		if (timerDelay == 40) {
			timerDelay = 100;
		}
		else if (timerDelay == 100) {
			timerDelay = 250;
		}
		else if (timerDelay == 250) {
			timerDelay = 500;

		}
		else if (timerDelay == 500) {
			timerDelay = 40;
		}

		break;

	case 'w': case 'W':
		if (warp == 1) {

			glm::mat4 uPosition = shape[1]->getOrientationMatrix();

			shape[5]->warpShip(uPosition, 1);
			warp = 2;
		}
		else if (warp == 2) {


			glm::mat4 dPosition = shape[2]->getOrientationMatrix();

			shape[5]->warpShip(dPosition, 2);
			warp = 1;
		}

		break;

	case 'v': case 'V':

		cameraSwitch++;
		if (cameraSwitch == 5) {
			cameraSwitch = 0;
			printf("Switch Reset\n");
		}

		if (cameraSwitch == 0) {
			eye = glm::vec3(0.0f, 10000.0f, 20000.0f);
			at = glm::vec3(0.0f, 0.0f, 0.0f);
			up = glm::vec3(0.0f, 1.0f, 0.0f);
			strcpy(viewStr, "Front");

		}
		if (cameraSwitch == 1) {
			eye = glm::vec3(0.0f, 20000.0f, 0.0f);
			at = glm::vec3(0.0f, 0.0f, 0.0f);
			up = glm::vec3(1.0f, 0.0f, 0.0f);
			strcpy(viewStr, "Top");
		}
		break;

	case 'f': case 'F':

		if (shape[nextMissile + 1]->getActive() == false && nextMissile != 17) {

			WBmissiles--;
			shape[nextMissile]->setTranslation(shape[5]->getTranslation());
			shape[nextMissile]->setRotation(shape[5]->getRotation());
			shape[nextMissile]->Active(true);
			printf("\n %i", nextMissile);
			sprintf(warbird, "Warbird:  %i       ", WBmissiles);

			nextMissile++;
		}

		break;
	case 'x': case 'X':

		cameraSwitch--;
		if (cameraSwitch == 0) {
			eye = glm::vec3(0.0f, 10000.0f, 20000.0f);
			at = glm::vec3(0.0f, 0.0f, 0.0f);
			up = glm::vec3(0.0f, 1.0f, 0.0f);
		}

		if (cameraSwitch == 1) {
			eye = glm::vec3(0.0f, 20000.0f, 0.0f);
			at = glm::vec3(0.0f, 0.0f, 0.0f);
			up = glm::vec3(1.0f, 0.0f, 0.0f);
			strcpy(viewStr, "Top");

		}

		if (cameraSwitch == -1) {
			cameraSwitch = 4;
			printf("Switch Reset\n");
		}

		break;


	}
	viewMatrix = glm::lookAt(eye, at, up);
	glUniform3f(HeadLightPosition, viewMatrix[3].x, viewMatrix[3].y, viewMatrix[3].z);
	updateTitle();
}

void controls(int k, int x, int y) {

	int specialKey = glutGetModifiers();

	if (k == GLUT_KEY_UP) {
		if (specialKey == GLUT_ACTIVE_CTRL) {
			shape[5]->pitchRightorLeft(0.02f);
		}
		else {
			shape[5]->mForward(thruster);
		}

	}
	else if (k == GLUT_KEY_DOWN) {
		if (specialKey == GLUT_ACTIVE_CTRL) {
			shape[5]->pitchRightorLeft(-0.02f);
		}
		else {
			shape[5]->mBack(thruster);
		}
	}
	else if (k == GLUT_KEY_LEFT) {
		if (specialKey == GLUT_ACTIVE_CTRL) {
			shape[5]->rollRightorLeft(-0.02f);
		}
		else {
			shape[5]->yawRightorLeft(0.02f);
		}
	}
	else if (k == GLUT_KEY_RIGHT) {
		if (specialKey == GLUT_ACTIVE_CTRL) {
			shape[5]->rollRightorLeft(0.02f);
		}
		else {
			shape[5]->yawRightorLeft(-0.02f);
		}
	}

}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Project 1: Ernie Ledezma, Stanislav Kirdey");

	// initialize and verify glew
	glewExperimental = GL_TRUE;  // needed my home system 
	GLenum err = glewInit();
	if (GLEW_OK != err)
		printf("GLEW Error: %s \n", glewGetErrorString(err));
	else {
		printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
		printf("OpenGL %s, GLSL %s\n",
			glGetString(GL_VERSION),
			glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	// initialize scene
	init();
	// set glut callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(timerDelay, intervalTimer, 1);
	glutIdleFunc(display);  //disp
	glutSpecialFunc(controls);
	glutMainLoop();
	printf("done\n");
	return 0;
}


