#include "DrawCurve.h"

DrawCurve::DrawCurve(BezierCurve* c){
	curve = c;
	color = glm::vec3(0.0,0.0,1.0);

	initValues();
	generateVectors();
	initShaders();
	generateBuffers();
}
//OpenGL 3.0 - gpu compute
void DrawCurve::draw(Camara* camera, int modo){

	glUseProgram(program);

	glm::mat4 modelMatrix = glm::translate(glm::vec3(0, 0, 0));

	glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &camera->getModelViewProjection(modelMatrix)[0][0]);

	//Drawing   
	glBindVertexArray(vao);

	glLineWidth(3);
	if (modo==1)
		glDrawElements(GL_POINTS, pointVector.size(), GL_UNSIGNED_INT, 0);
	else if (modo==2)
		glDrawElements(GL_LINES, pointVector.size(), GL_UNSIGNED_INT, 0);
	else
		glDrawElements(GL_LINE_LOOP, pointVector.size(), GL_UNSIGNED_INT, 0);
	glLineWidth(1);

	glUseProgram(NULL);

}

void DrawCurve::initValues(){

	program = 0;

	inVertex = -1;
	inColor = -1;
	
	mvpMatrixID = -1;
}

void DrawCurve::generateVectors(){

	for (int i = 0; i <curve->nPoints(); i++){
		PV3D* punto = curve->getPointList().at(i)->clone();
		vertexVector.push_back(punto->getX());
		vertexVector.push_back(punto->getY());
		vertexVector.push_back(punto->getZ());
	}

	for (unsigned int i = 0; i<vertexVector.size() / 3; ++i)
		pointVector.push_back(i);

}

void DrawCurve::initShaders(){
	//Compile shaders
	vShader.compileShader();
	fShader.compileShader();

	//Link
	program = glCreateProgram();
	glAttachShader(program, vShader.getShaderID());
	glAttachShader(program, fShader.getShaderID());
	glBindAttribLocation(program, 0, "inVertex");
	glBindAttribLocation(program, 1, "inColor");

	glLinkProgram(program);

	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked){
		//Calculate error string
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

		char *logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete logString;

		glDeleteProgram(program);
		program = 0;
		return;
	}

	//Uniform variable
	mvpMatrixID = glGetUniformLocation(program, "modelViewProjection");

	//Attributes
	inColor = glGetAttribLocation(program, "inColor");
	inVertex = glGetAttribLocation(program, "inVertex");

}

//Generate VBO && VAO
void DrawCurve::generateBuffers(){
	
	glFrontFace(GL_CW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, buffer);

	//Vertex
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexVector.size(), &(vertexVector.front()), GL_STATIC_DRAW);
	glVertexAttribPointer(inVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);  //Shader input
	glEnableVertexAttribArray(inVertex);

	//Points
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*pointVector.size(), &(pointVector.front()), GL_STATIC_DRAW);

	glBindVertexArray(0);

}
