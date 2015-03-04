#version 330 core

in vec3 inColor;
in vec3 inVertex;

out vec3 vcolor;
out vec3 pos;

uniform mat4 modelViewProjection;

void main()
{
	vcolor = vec3(0.0,0.0,1.0);
	pos = inVertex;
    gl_Position = modelViewProjection*vec4(inVertex,1);
}