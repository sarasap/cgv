#include "include/object.h"

//Constructor for the object, just initialises most variables.
Object::Object(char* fileName, string objectName) {
	name = objectName;

	numVertices = 0;
	numIndices  = 0;
	
	currentTranslation[0] = 0.0;
	currentTranslation[1] = 0.0;
	currentTranslation[2] = 0.0;
	currentTranslationSpeed[0] = 0.0;
	currentTranslationSpeed[1] = 0.0;
	currentTranslationSpeed[2] = 0.0;
	currentRotation = 0;
	currentRotationSpeed = 0;
	currentDirection = 1;

	modelViewStack = new MatrixStack(5);
	
	readFile(fileName);
}

//Destructor.
Object::~Object() {
	
	delete modelViewStack;
	delete vertexPositions;
	delete vertexColours;
	delete vertexIndices;

}

void Object::setupData(int objectId, bool colourOff) {
	GLdouble* tempVertColours = new GLdouble[numVertices*4];

	//Do we want to do a colour-code pass?
	if(colourOff) {
		for(int i = 0; i < numVertices*4; i++) {
			tempVertColours[i] = vertexColours[i];
			vertexColours[i] = objectId * 0.001;
			//cout << objectId * 0.001;
			glDisable(GL_DITHER);
		}
	}

	id = objectId;

	// Load shaders and use the resulting shader program
	program = InitShader( "shaders/vertexShader.glsl", "shaders/pixelShader.glsl" );
	glUseProgram( program );

	// Create a vertex array object
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	// Create and initialize two buffer objects
	glGenBuffers(3, buffers);

	//one buffer for the vertices and colours
	glBindBuffer( GL_ARRAY_BUFFER, buffers[0]);
	glBufferData( GL_ARRAY_BUFFER, numVertexPositionBytes() + numVertexColourBytes(), NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, numVertexPositionBytes(), vertexPositions );
	glBufferSubData( GL_ARRAY_BUFFER, numVertexPositionBytes(), numVertexColourBytes(), vertexColours);

	//one buffer for the indices
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, numVertexIndexBytes(), vertexIndices, GL_STATIC_DRAW );

	//Allow the program to communicate with the shader programs by passing and receiving refrences.
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_DOUBLE, GL_FALSE, 0, BUFFER_OFFSET(0) );

	GLuint vColor = glGetAttribLocation( program, "vColor" );
	glEnableVertexAttribArray( vColor );
	glVertexAttribPointer( vColor, 4, GL_DOUBLE, GL_FALSE, 0, BUFFER_OFFSET(numVertexPositionBytes()) );

	GLuint fcurrentTranslation = glGetUniformLocation( program, "fcurrentTranslation" );
	glUniform1f(fcurrentTranslation, colourOff ? 0.0 : currentTranslation[0]);
	GLuint fcolourCodingOffsetRed = glGetUniformLocation( program, "fcolourCodingOffsetRed" );
	glUniform1f(fcolourCodingOffsetRed, colourOff ? id + 1.0 : 0.0);

	modelView = glGetUniformLocation( program, "model_view" );
	projection = glGetUniformLocation( program, "projection" );

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if(colourOff) {
		for(int i = 0; i < numVertices*4; i++) {
			vertexColours[i] = tempVertColours[i];
			glEnable(GL_DITHER);
		}
	}
	delete tempVertColours;
}

void Object::updateDisplay(MatrixStack projectionStack, bool colourOff) {
	setupData(id, colourOff);

	//Load modelView Identity
	modelViewStack->loadIdentity();

	//Translate the object in space.
	currentTranslation[0] += currentTranslationSpeed[0];
	currentTranslation[1] += currentTranslationSpeed[1];
	currentTranslation[2] += currentTranslationSpeed[2];
	modelViewStack->translated(currentTranslation[0], currentTranslation[1], currentTranslation[2]);

	//Rotate the object based on it's rotation speed (and current rotation).
	currentRotation += currentRotationSpeed;
	modelViewStack->rotated(currentRotation, 0, 1, 0);

	glUniformMatrix4fv(modelView, 1, GL_FALSE, modelViewStack->getMatrixf());
	glUniformMatrix4fv(projection, 1, GL_FALSE, projectionStack.getMatrixf());
	
	//Bind vao
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	//Indexing into vertices we need to use glDrawElements
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}

//Reads the file in and grabs vertex information, face information etc.
void Object::readFile(char* fileName) {
	ifstream in(fileName);

	//Use vectors for now so we know how to assign global arrays space in memory later.
	vector<GLdouble> tempPositions;
	vector<GLuint> tempIndices;

	//Keep track of what line we're on.
	int currentLine = 0;

	string lineStr;
    while(getline(in, lineStr)) {

        istringstream lineSS( lineStr );
        string lineType;
        lineSS >> lineType;

        //Fetch a line with vertices
        if(lineType == "v") {
            GLdouble x = 0.0, y = 0.0, z = 0.0, w = 1.0;
            lineSS >> x >> y >> z;

			//Debug text.
            //cout << currentLine + 1 << " " << x << " " << y << " " << z << " " << w << endl;

			tempPositions.push_back(x);
			tempPositions.push_back(y);
			tempPositions.push_back(z);
			tempPositions.push_back(w);

			numVertices += 1;
        }

		//Fetch a line with vertex indices
        if(lineType == "f") {
            GLuint x = 0, y = 0, z = 0;
            lineSS >> x >> y >> z;

			//Debug text.
            //cout << currentLine + 1 << " " << x << " " << y << " " << z << endl;
			
			tempIndices.push_back(x);
			tempIndices.push_back(y);
			tempIndices.push_back(z);

			numIndices += 3;
        }

		currentLine++;
    }

	//cout << numVertices / 3 << "  ";
	//cout << numIndices << "  ";

	//Store contents of vectors in global array.
	vertexPositions = new GLdouble[numVertices*4];
	for(int i = 0; i < numVertices*4; i++) {
		vertexPositions[i] = tempPositions.at(i);
		//cout << tempPositions.at(i) << " ";
	}
	vertexIndices = new GLuint[numIndices];
	for(int i = 0; i < numIndices; i++) {
		vertexIndices[i] = tempIndices.at(i) - 1;
		//cout << tempIndices.at(i) << " ";
	}
	vertexColours = new GLdouble[numVertices*4];
	int modifier = 0;
	for(int i = 0; i < numVertices*4; i++) {
		modifier += 1;
		vertexColours[i] = (modifier % 10) * 0.1;
		//cout << vertexColours[i] << " ";
	}

}

//ACCESSORS AND SETTERS ARE BELOW
void Object::setTranslation(double x, double y, double z) { 
	currentTranslation[0] = x;	
	currentTranslation[1] = y;	
	currentTranslation[2] = z;	
}
void Object::setTranslationSpeed(double x, double y, double z) { 
	currentTranslationSpeed[0] = x;	
	currentTranslationSpeed[1] = y;	
	currentTranslationSpeed[2] = z;	
}

void Object::setRotation(int angle) { 
	currentRotation = angle;
}
void Object::setRotationSpeed(int speed) { 
	currentRotationSpeed = speed;
}

void Object::setDirection(int direction) { 
	currentDirection = direction;
}

double* Object::getTranslation() { return currentTranslation;	}
int Object::getRotation()		 { return currentRotation;		}
int Object::getDirection()		 { return currentDirection;		}

double* Object::getTranslationSpeed() { return currentTranslationSpeed;	}
int Object::getRotationSpeed()		  { return currentRotationSpeed;	}

GLuint Object::numVertexPositionBytes() { return numVertices*4*sizeof(GLdouble);	}
GLuint Object::numVertexColourBytes()	{ return numVertices*4*sizeof(GLdouble);	}
GLuint Object::numVertexIndexBytes()	{ return numIndices*sizeof(GLuint);			}