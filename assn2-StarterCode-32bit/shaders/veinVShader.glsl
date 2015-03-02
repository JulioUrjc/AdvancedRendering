#version 330 core

in vec3 inVertex;
in vec3 normal;

out vec3 vcolor;
out vec3 vnormal;
out vec3 vertex;

uniform mat4 modelViewProjection;
uniform mat4 modelView;

void main(){
	vcolor = vec3(0.0,0.0,0.8);
	vnormal= normal;
	vertex = (modelView*vec4(inVertex,1)).xyz;
	gl_Position = modelViewProjection*vec4(inVertex,1);
	gl_Position = vec4(inVertex,1);
}