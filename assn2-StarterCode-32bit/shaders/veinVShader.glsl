#version 330 core

in vec3 inVertex;
in vec3 inNormal;
in vec2 inTextCoord;

out vec3 vertex;
out vec3 vnormal;
out vec3 vcolor;
out vec2 vTextCoord;

uniform mat4 modelViewProjection;
uniform mat4 modelView;

void main(){
	vcolor = vec3(1.0,0.0,0.0);
	vnormal= inNormal;
	vTextCoord= inTextCoord;
	vertex = (modelView*vec4(inVertex,1)).xyz;
	gl_Position = modelViewProjection*vec4(inVertex,1);
}