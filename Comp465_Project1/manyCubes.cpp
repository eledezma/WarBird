/*
manyCubes.cpp
includes Shape3D.cpp

A modification of CubeTriModel.cpp that uses C and C++.
This file (manyCube.cpp) is a C file -- it defines no classes
but has 1000 instances of Shape3D.

File Shape3D defines a 3D shape that can translate and rotate.
There is only 1 model created from the "Cube2.tri" file.

However 1,000 Shapes3D are drawn as cubes.
Each has its own position and orientation.
Each cube rotates either orbitally or about its center.
Each cube is updated approximately 25 times per second.
Each frame is drawn whenever glut is idle, see fps value in title.

465 utility include files:  shader465.h, triModel465.h
Shaders:  simpleVertex.glsl  amd simpleFragment.glsl

User commands:
'f' view from front (0, 0, 3) looking at origin
't' view from above (0, 3, 3) looking at origin
'b' view from below (0, -3, 0) looking at origin

Current state is displayed in the window title.

The cube2.tri model uses 12 triangular surfaces to display 12 trianlge surfaces.
So, there are 36 (3 * 12) vertices.

The triModel465.hpp utility creates colors, and normals for ever vertex.
However the simple*.glsl shaders do not use the normal.
Since the same color is created for every vertex of every surface,
the object is rendered with flat shading.

A reference for estimating fps using glut functions:
http://mycodelog.com/2010/04/16/fps/

Mike Barnes
8/16/2014
*/


# define __Windows__ // define your target operating system
# include "../includes465/include465.hpp"  

# include "Shape3D.hpp"

// Shapes
const int nShapes = 7;
Shape3D * shape[nShapes];
// Model for shapes


char * modelFile[nShapes] = {
	"ruber.tri",
	"unum.tri",
	"duo.tri",
	"primus.tri",
	"secundus.tri",
	"battleship.tri",
	"battleship.tri"
};

const int nVertices[nShapes] = {
	264 * 3,
	264 * 3,
	264 * 3,
	264 * 3,
	264 * 3,
	734 * 3,
	734 * 3
};

float modelSize[nShapes] = {
	2000.0f,
	200.0f,
	400.0f,
	100.0f,
	150.0f,
	400.0f,
	400.0f };


float modelBR[nShapes];  // modelFile's bounding radius


// display state and "state strings" for title display
// window title strings
char baseStr[50] = "465 manyCubes Example {f, t, r} : ";
char fpsStr[15], viewStr[15] = " Front View |";
char titleStr[100];

GLuint VAO[nShapes];      // Vertex Array Objects
GLuint buffer[nShapes];   // Vertex Buffer Objects

GLuint shaderProgram;
char * vertexShaderFile = "simpleVertex.glsl";
char * fragmentShaderFile = "simpleFragment.glsl";
GLuint MVP;  // Model View Projection matrix's handle
GLuint vPosition[nShapes], vColor[nShapes], vNormal[nShapes];

glm::vec3 scale[nShapes];
glm::mat4 projectionMatrix;     // set in reshape()
glm::mat4 modelMatrix;          // set in shape[i]-->updateDraw()
glm::mat4 viewMatrix;           // set in keyboard()
glm::mat4 DuoMatrix;
glm::mat4 ModelViewProjectionMatrix; // set in display();
bool idleTimerFlag = false;
int cameraSwitch = 0;
int modulo = 15;
int initial = 0;
// vectors and values for lookAt
glm::vec3 eye, at, up;

// rotation variables
glm::mat4 identity(1.0f);
glm::mat4 rotation;
int timerDelay = 40, frameCount = 0;  // A delay of 40 milliseconds is 25 updates / second
double currentTime, lastTime, timeInterval;

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
		modelBR[i] = loadModelBuffer(modelFile[i], nVertices[i], VAO[i], buffer[i], shaderProgram,
			vPosition[i], vColor[i], vNormal[i], "vPosition", "vColor", "vNormal");
		// set scale for models given bounding radius  
		scale[i] = glm::vec3(modelSize[i] * 1.0f / modelBR[i]);
	}

	MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");
	// initially use a front view
	eye = glm::vec3(0.0f, 20000.0f, 10000.0f);   // eye is 1000 "out of screen" from origin
	at = glm::vec3(0);   // looking at origin
	up = glm::vec3(0.0f, 1.0f, 0.0f);   // camera'a up vector
	viewMatrix = glm::lookAt(eye, at, up);

	// set render state values
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	// create shape
	for (int i = 0; i < nShapes; i++){
		shape[i] = new Shape3D(i);
		shape[i]->setScaleMatrix(scale[i]);
	}
	printf("%d Shapes created \n", nShapes);

	lastTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
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
	strcpy(titleStr, baseStr);
	strcat(titleStr, viewStr);
	strcat(titleStr, fpsStr);
	// printf("title string = %s \n", titleStr);
	glutSetWindowTitle(titleStr);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Update model matrices
	for (int m = 0; m < nShapes; m++) {

		if (m == 3 || m == 4){
			DuoMatrix = shape[2]->getPositionMatrix();
			modelMatrix = shape[m]->getModelMatrix();
			modelMatrix = DuoMatrix * modelMatrix;

		}
		else{
			modelMatrix = shape[m]->getModelMatrix();
		}

		ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
		glBindVertexArray(VAO[m]);
		glDrawArrays(GL_TRIANGLES, 0, nVertices[m]);
	}

	glutSwapBuffers();
	frameCount++;
	// see if a second has passed to set estimated fps information
	currentTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
	timeInterval = currentTime - lastTime;
	if (timeInterval >= 1000) {
		sprintf(fpsStr, " fps %4d", (int)(frameCount / (timeInterval / 1000.0f)));
		lastTime = currentTime;
		frameCount = 0;
		updateTitle();
	}
}


// Animate scene objects by updating their transformation matrices
// timerDelay = 40, or 25 updates / second
void update(int i) {
	glutTimerFunc(timerDelay, update, 1);
	for (int i = 0; i < nShapes; i++) shape[i]->update();
}
void toggleCamera(){

}

// Quit or set the view
void keyboard(unsigned char key, int x, int y) {

	int camera = 0;
	
	switch (key) {
	
	case 033: case 'q':  case 'Q': exit(EXIT_SUCCESS); break;
	case 'v': case 'V':  
		
		if (cameraSwitch == 0){
			eye = glm::vec3(0.0f, 10000.0f, 20000.0f);
			at = glm::vec3(0.0f, 0.0f, 0.0f);
			up = glm::vec3(0.0f, 1.0f, 0.0f);
			strcpy(viewStr, "Front View |");
			cameraSwitch = 1;
			printf("%d", cameraSwitch);
			break;
		
			}
		if (cameraSwitch == 1){
				eye = glm::vec3(0.0f, 30000.0f, 0.0f);
				at = glm::vec3(0.0f, 0.0f, 0.0f);
				up = glm::vec3(1.0f, 0.0f, 0.0f);
				strcpy(viewStr, "Top View |");
				cameraSwitch = 0;
				printf("%d", cameraSwitch);
				break;
			}

	}
	viewMatrix = glm::lookAt(eye, at, up);
	updateTitle();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("465 manyCubes Example {v to switch camera} : Front View |");
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
	glutTimerFunc(timerDelay, update, 1);
	glutIdleFunc(display);
	glutMainLoop();
	printf("done\n");
	return 0;
}


