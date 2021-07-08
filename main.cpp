#include "include/main.h"

double camRotateValue = 0.0;
double bounceWidth = 1.1;
double bounceHeightSquare1 = 0.6;
double bounceHeightSquare2 = -0.6;
double maxSpeed = 0.05;

bool registeringClick = false;
int lastX = 0;
int lastY = 0;
int mouseButton = 0;

Object* clickedObject;

vector<Object*> objects;

MatrixStack projectionStack(5);

//----------------------------------------------------------------------------
//Function used to update the bounds of the objects with different parameters according to task.
void updateObjectBounds() {
	objects[0]->setTranslation(0.0, bounceHeightSquare1, 0);
	objects[1]->setTranslation(0.0, bounceHeightSquare2, 0);
	objects[2]->setTranslation(-bounceWidth, bounceHeightSquare1, 0);
	objects[3]->setTranslation(bounceWidth, bounceHeightSquare1, 0);
	objects[4]->setTranslation(bounceWidth, bounceHeightSquare2, 0);
	objects[5]->setTranslation(-bounceWidth, bounceHeightSquare2, 0);
}

// OpenGL initialization
void init() {

	//Enable DEPTH on the buffer.
	glEnable(GL_DEPTH_TEST);

	//Grey Background
	glClearColor(0.2, 0.2, 0.2, 1.0);

	//Add to object display list.
	objects.push_back(new Object("sphere.obj", "Sphere1"));
	objects.push_back(new Object("sphere.obj", "Sphere2"));
	objects.push_back(new Object("square.obj", "Square1a"));
	objects.push_back(new Object("square.obj", "Square1b"));
	objects.push_back(new Object("square.obj", "Square2a"));
	objects.push_back(new Object("square.obj", "Square2b"));

	//Update object transformation references.
	updateObjectBounds();

	//Update starting speeds and rotation.
	objects[0]->setTranslationSpeed(0.02, 0, 0);
	objects[1]->setTranslationSpeed(0.005, 0, 0);
	objects[0]->setRotationSpeed(15);
	objects[1]->setRotationSpeed(5);

	//Setup objects for rendering.
	for(int i = 0; i < objects.size(); i++) {
		objects[i]->setupData(i, false);
	}
}

void registerColourPick() {
	//Fetch the pixel colour value at last click screenPoint and viewport offset/modification.
	GLint viewport[4];
	GLubyte pixel[3];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(lastX, viewport[3]-lastY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *)pixel);

	//Adjust the index
	int id = pixel[0] - 1;
	double modifier = 0.0;

	//Each pixel r value corresponds to an objectId (index) in objects vector.
	switch(id) {
	case 0: //Sphere1
	case 1: //Sphere2
		if(objects[id]->getTranslationSpeed()[0] * 1.25 > maxSpeed) break;
		modifier = mouseButton == 0 ? 1.25 : 0.75;
		objects[id]->setTranslationSpeed(objects[id]->getTranslationSpeed()[0] * modifier, 
										 objects[id]->getTranslationSpeed()[1], 
										 objects[id]->getTranslationSpeed()[2]);
		break;
	case 2: //Square1a
	case 3: //Square1b
	case 4: //Square2a
	case 5: //Square2b
		clickedObject = objects[id];
		break;
	}
	
}

//----------------------------------------------------------------------------
void display( void ) {

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	//Load the camera projection
	projectionStack.loadIdentity();
	projectionStack.perspective(75, 1, 0.1, 25);
	projectionStack.lookAt(sin(camRotateValue*2) * -3, 0, cos(camRotateValue*2) * -3, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	if(registeringClick && lastX != 0 && lastY != 0) {
		//Go through objects and display WITH COLOUR CODING
		for(int i = 0; i < objects.size(); i++) {
			objects[i]->updateDisplay(projectionStack, true);
		}
		
		registerColourPick();

		registeringClick = false;
	} 

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//Go through objects and display.
	for(int i = 0; i < objects.size(); i++) {
		objects[i]->updateDisplay(projectionStack, false);

		//First two are spheres, let's update their current standings.
		if(i < 2) {
			if(objects[i]->getTranslation()[0] + objects[i]->getTranslationSpeed()[0] > bounceWidth) {
				objects[i]->setDirection(-1);
				objects[i]->setTranslationSpeed(objects[i]->getDirection() * objects[i]->getTranslationSpeed()[0], 
												objects[i]->getDirection() * objects[i]->getTranslationSpeed()[1], 
												objects[i]->getDirection() * objects[i]->getTranslationSpeed()[2]);
			} else if(objects[i]->getTranslation()[0] + objects[i]->getTranslationSpeed()[0]  < -bounceWidth) {
				objects[i]->setDirection(1);
				objects[i]->setTranslationSpeed(objects[i]->getDirection() * -objects[i]->getTranslationSpeed()[0], 
												objects[i]->getDirection() * -objects[i]->getTranslationSpeed()[1], 
												objects[i]->getDirection() * -objects[i]->getTranslationSpeed()[2]);
			}
		}
	}
	
	glutSwapBuffers();

}

//----------------------------------------------------------------------------
void keyboard( unsigned char key, int x, int y ) {
	switch( key ) {

	//Quit
	case 033: // Escape Key
	case 'q': case 'Q':
	exit( EXIT_SUCCESS );
	break;

	//Rotate around the object using LookAt();
	case 'z':
	camRotateValue += 0.1;
	break;
	case 'x':
	camRotateValue -= 0.1;
	break;

	//Change size of window
	case 'w':
	bounceHeightSquare1 += bounceHeightSquare1 > 0.0 ? 0.1 : 0;
	bounceHeightSquare2 += bounceHeightSquare2 > 0.0 ? 0.1 : 0;
	updateObjectBounds();
	break;
	case 's':
	bounceHeightSquare1 -= bounceHeightSquare1 > 0.1 ? 0.1 : 0;
	bounceHeightSquare2 -= bounceHeightSquare2 > 0.1 ? 0.1 : 0;
	updateObjectBounds();
	break;
	case 'd':
	bounceWidth += bounceWidth > 0.0 ? 0.1 : 0;
	updateObjectBounds();
	break;
	case 'a':
	bounceWidth -= bounceWidth > 0.1 ? 0.1 : 0;
	updateObjectBounds();
	break;

	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------
void reshape( int width, int height ) {

	glViewport( 0, 0, width, height );

}

void mouse(int button, int state, int x, int y) {
	cout << button << " MouseClick - x: " << x << " y: " << y << " state: " << state << endl;

	if(state == 0) {
		registeringClick = true;
		mouseButton = button;
		lastX = x;
		lastY = y;
	} else {
		//If we aren't lifting our mouse without having hit a correct object.
		//Lift up signifies we're done dragging an object.
		if(clickedObject != NULL) {
			GLint viewport[4];
			glGetIntegerv( GL_VIEWPORT, viewport );
			GLdouble modelview[16];                 // Where The 16 Doubles Of The Modelview Matrix Are To Be Stored
			glGetDoublev(GL_MODELVIEW_MATRIX, modelview);       // Retrieve The Modelview Matrix
			GLdouble projection[16];                // Where The 16 Doubles Of The Projection Matrix Are To Be Stored
			glGetDoublev(GL_PROJECTION_MATRIX, projection);     // Retrieve The Projection Matrix

			GLdouble objX;
			GLdouble objY;
			GLdouble objZ;
			gluUnProject(x, y, 0.0, modelview, projection, viewport, &objX, &objY, &objZ);

			//clickedObject->setTranslation(clickedObject->getTranslation()[0], objY * 2, clickedObject->getTranslation()[2]);

			if(clickedObject->id == 2 || clickedObject->id == 3)	//Top squares
				bounceHeightSquare1 = objY * -2;
			else if(clickedObject->id == 4 || clickedObject->id == 5)	//Bottom squares
				bounceHeightSquare2 = objY * -2;

			updateObjectBounds();
			clickedObject = NULL;
		}
	}
}

//----------------------------------------------------------------------------
int main( int argc, char **argv ) {

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( 512, 512 );
	glutCreateWindow( "Orbit the Color Cube - Orthographic" );

	glewInit();

	init();

	glutIdleFunc( display );
	glutKeyboardFunc( keyboard );
	glutReshapeFunc( reshape );
	glutMouseFunc( mouse );

	glutMainLoop();

	//Remove the allocation of memory.
	for(int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}

	return 0;
}
