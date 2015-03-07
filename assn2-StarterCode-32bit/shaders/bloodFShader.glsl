#version 330 core

in vec3 vertex;
in vec3 vnormal;
in vec3 vcolor;
in vec2 vTextCoord;

out vec4 outColor;

//Lighting
uniform vec3 ambientLight;
uniform vec3 diffuseLight;
uniform vec3 lightDirection;

uniform sampler2D textureVein;

//Material dependant
const vec3 specularLight = vec3(0.6f,0.6f,0.6f);
const float shininess = 0.9f;

void main(){
	
    //Ambient. The material is the same for all light components (just color)
	vec3 amb = ambientLight * vcolor;
	amb = clamp(amb, 0.0, 1.0);

	//Eye direction in world coordinates
	vec3 eyeDirection = normalize(-vertex);

	//Just directional light, so no position or angle, Diffuse color
	vec3 diffuse = vcolor * diffuseLight * max(dot(vnormal,lightDirection),0.0);
	diffuse = clamp(diffuse,0.0,1.0);

	//Specular color
	vec3 reflected = normalize(-reflect(lightDirection,vnormal));
	vec3 specular = vcolor * specularLight * pow(max(dot(reflected,eyeDirection),0.0),shininess);
	specular = clamp(specular,0.0,1.0);

	//Texture
	outColor = (texture2D(textureVein, vTextCoord)*0.8f + vec4(amb+diffuse+specular,1)*0.2f );
	//outColor = vec4(amb+diffuse+specular,1)*0.6f;
	//outColor = vec4(amb+diffuse,1)*0.6f;
}
