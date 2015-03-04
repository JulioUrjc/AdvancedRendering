#version 330 core

in vec3 inVertex;
in vec3 inNormal;

out vec3 vertex;
out vec3 vnormal;
out vec3 vcolor;

uniform mat4 modelViewProjection;
uniform mat4 modelView;
uniform vec3 inColor;

void main(){
	vcolor = inColor;
	vnormal= inNormal;
	vertex = (modelView*vec4(inVertex,1)).xyz;
	gl_Position = modelViewProjection*vec4(inVertex,1);
}