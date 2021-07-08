#ifndef OBJECT_H
#define OBJECT

#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include "include/MatrixStack.h"
#include "include/InitShader.h"
#include <math.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

class Object
{

	public:
		
		int id;
		string name;

		Object(char* fileName, string objectName);
		~Object();

		void updateDisplay(MatrixStack projectionStack, bool colourOff);
		void setupData(int objectId, bool colourOff);

		MatrixStack* modelViewStack;

		void setTranslation(double x, double y, double z);
		void setTranslationSpeed(double x, double y, double z);
		void setRotation(int angle);
		void setRotationSpeed(int speed);
		void setDirection(int direction);
		
		double* getTranslation();
		double* getTranslationSpeed();
		int getRotation();
		int getRotationSpeed();
		int getDirection();

	private:
	
		int numVertices;
		int numIndices;

		int currentDirection;

		double currentTranslation[3];
		double currentTranslationSpeed[3];
		int currentRotation;
		int currentRotationSpeed;

		void readFile(char* fileName);

		GLdouble* vertexPositions;
		GLdouble* vertexColours;
		GLuint* vertexIndices;
		
		GLuint  modelView;  // model-view matrix uniform shader variable location
		GLuint  projection; // projection matrix uniform shader variable location
		
		GLuint numVertexPositionBytes();
		GLuint numVertexColourBytes();
		GLuint numVertexIndexBytes();

		//----------------------------------------------------------------------------
		GLuint vao;
		GLuint program;
		GLuint buffers[2];


};

#endif