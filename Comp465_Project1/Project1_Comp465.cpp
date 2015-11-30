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

# define __Windows__ // define your target operating system
# include "../includes465/include465.hpp"  

# include "Shape3D.hpp"
# include "Missle3D.hpp"

// Amount of Shapes
const int nShapes = 9;
Shape3D * shape[nShapes];

// Model for shapes
char * modelFile[nShapes] = {
	"ruber.tri",
	"unum.tri",
	"duo.tri",
	"primus.tri",
	"secundus.tri",
	"battleship.tri",
	"missile.tri",
	"missileSite.tri",
	"missileSite.tri"
};

const int nVertices[nShapes] = {
	264 * 3,
	264 * 3,
	264 * 3,
	264 * 3,
	264 * 3,
	734 * 3,
	112 * 3,
	1696 * 3, //missile site unum
	1696 * 3 //missile site secundus
};

float modelSize[nShapes] = {
	2000.0f,
	200.0f,
	400.0f,
	100.0f,
	150.0f,
	100.0f,
	25.0f,
	30.0f,
	30.0f };

float modelBR[nShapes];  // modelFile's bounding radius

						 // Position shapes in the world
glm::vec3 translate[nShapes] = {
	glm::vec3(0, 0, 0),
	glm::vec3(4000, 0, 0),
	glm::vec3(-9000, 0, 0),
	glm::vec3(900, 0, 0),
	glm::vec3(1750, 0, 0),
	glm::vec3(5000, 1000, 5000),
	glm::vec3(4900, 1000, 4850),
	glm::vec3(4000, 220, 0),
	glm::vec3(1750, 175, 0) };

// Rotation angels
float radians[nShapes] = { 0.0f, 0.4f, 0.2f, 0.4f, 0.2f, 0.0f, 0.0f, 0.4f, 0.2f };

bool orbital[nShapes] = { false, true, true, true, true, false, false, true, true };

// display state and "state strings" for title display
// window title strings
char warbird[50] = "Warbird:        ";
char unum[50] = "Unum:        ";
char secundus[50] = "Secundus:        ";
char us[50] = "U/S:";
char fs[50] = "F/S:";
char view[50] = "View: ";
char fpsStr[15], viewStr[15] = "Front";
char updStr[15];
char titleStr[100];

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

int thruster = 10; //thrustuer speed initially at 0
int warp = 1;  //1 = unum, 2 = duo
boolean gravity = false; //set gravity on/off, init off

GLuint HeadLightPosition, HeadLightIntensity, PointLightPosition, PointLightIntensity;
GLboolean HeadLightOn, PointLightOn;

// load the shader programs, vertex data from model files, create the solids, set initial view
void init() {	
	// set render state values
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_POLYGON_SMOOTH);

	// load the shader programs
	shaderProgram = loadShaders(vertexShaderFile, fragmentShaderFile);
	glUseProgram(shaderProgram);

	// generate VAOs and VBOs
	glGenVertexArrays(nShapes, VAO);
	glGenBuffers(nShapes, buffer);

	// load the buffers from the model files
	for (int i = 0; i < nShapes; i++) {

		modelBR[i] = loadModelBuffer(modelFile[i], nVertices[i], VAO[i], buffer[i], shaderProgram,
			vPosition[i], vColor[i], vNormal[i], "vPosition", "vColor", "vNormal");
		// set scale for models given bounding radius  
		scale[i] = glm::vec3(modelSize[i] * 1.0f / modelBR[i]);

	}

	modelLoc = glGetUniformLocation(shaderProgram, "modelPos");
	viewLoc = glGetUniformLocation(shaderProgram, "viewPos");
	MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");
	// initially use a front view
	eye = glm::vec3(0.0f, 10000.0f, 20000.0f);
	at = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	viewMatrix = glm::lookAt(eye, at, up);

	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	lastTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
											// create shape
	for (int i = 0; i < nShapes; i++) {
		if (i == 6) {
			// Create a missle shape and send it to the travel
			shape[i] = new Missle3D(translate[i]);
		}
		else {
			shape[i] = new Shape3D(translate[i], radians[i], orbital[i]);
		}


		shape[i]->setScaleMatrix(scale[i]);
	}
	printf("%d Shapes created \n", nShapes);


	// Set the initial light settings
	GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
	GLint PointLightPositionLoc = glGetUniformLocation(shaderProgram, "PointLightPosition");
	GLint PointLightOnLoc = glGetUniformLocation(shaderProgram, "PointLightOn");
	GLint PointLightIntensityLoc = glGetUniformLocation(shaderProgram, "PointLightIntensity");

	HeadLightPosition = glGetUniformLocation(shaderProgram, "HeadLightPosition");
	HeadLightOn = glGetUniformLocation(shaderProgram, "HeadLightOn");
	HeadLightIntensity = glGetUniformLocation(shaderProgram, "HeadLightIntensity");

	glm::vec3 lightPos1 = glm::vec3(0.1f, 0.1, 0.1f);

	glUniform3f(PointLightPositionLoc, lightPos1.x, lightPos1.y, lightPos1.z);
	glUniform1f(PointLightOnLoc, true);
	glUniform3f(PointLightIntensityLoc, 0.3f, 0.2f, 0.1f);

	glUniform1f(HeadLightOn, true);
	glUniform3f(HeadLightIntensity, 0.1f, 0.3f, 0.4f);

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

	glutSetWindowTitle(titleStr);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Update model matrices
	for (int m = 0; m < nShapes; m++) {

		if (m == 3 || m == 4 || m == 8) {

			DuoMatrix = shape[2]->getPositionMatrix();
			modelMatrix = shape[m]->getModelMatrix();
			modelMatrix = DuoMatrix * modelMatrix;

		}
		else {
			modelMatrix = shape[m]->getModelMatrix();
		}

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(shape[m]->getModelMatrix()));

		ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
		glBindVertexArray(VAO[m]);
		glDrawArrays(GL_TRIANGLES, 0, nVertices[m]);

	}
	DuoMatrix = shape[2]->getPositionMatrix();
	UnumMatrix = shape[1]->getPositionMatrix();
	WarbirdMatrix = shape[5]->getPositionMatrix();

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

void update() {


	for (int i = 0; i < nShapes; i++) shape[i]->update();

	if (gravity) {
		shape[5]->gravity();
	}

	updateCount++;
	currentTimeU = glutGet(GLUT_ELAPSED_TIME);
	timeIntervalU = currentTimeU - lastTimeU;


	if (timeIntervalU >= 1000) {
		sprintf(updStr, "%4d     ", (int)(updateCount / (timeIntervalU / 1000.0f)));
		lastTimeU = currentTimeU;
		updateCount = 0;
		updateTitle();
	}
	glutPostRedisplay();
}

void intervalTimer(int i) {
	glutTimerFunc(timerDelay, intervalTimer, 1);
	if (!idleTimerFlag) update();
}

// Quit or set the view
void keyboard(unsigned char key, int x, int y) {

	switch (key) {

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

			glm::mat4 uPosition = shape[1]->getPositionMatrix();

			shape[5]->warpShip(uPosition, 1);
			warp = 2;
		}
		else if (warp == 2) {


			glm::mat4 dPosition = shape[2]->getPositionMatrix();

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

	case 'x': case 'X':

		cameraSwitch--;
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


		if (cameraSwitch == -1) {
			cameraSwitch = 4;
			printf("Switch Reset\n");
		}



		break;

	}
	viewMatrix = glm::lookAt(eye, at, up);
	glUniform3f(HeadLightPosition, viewMatrix[0].x, viewMatrix[0].y, viewMatrix[0].z);
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


