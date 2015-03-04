#version 330 core

in vec3 inColor;
in vec3 inVertex;
out vec3 vcolor;
out vec3 pos;

uniform mat4 modelViewProjection;

void main()
{
	vcolor = inColor;
	pos = inVertex;
    gl_Position = modelViewProjection*vec4(inVertex,1);
}