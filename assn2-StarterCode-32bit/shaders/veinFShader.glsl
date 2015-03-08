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

//Show or not texture
uniform int showTexture;
uniform float iGlobalTime;
uniform int mutation;

#define PI 3.141592653589

void main(){

	vec2 r =  2.0f*vec2(gl_FragCoord.x - 0.5f*640, gl_FragCoord.y - 0.5f*480)/480;
	float t = iGlobalTime;
    r = r * 8.0;

	float v1 = sin(r.x +t);
    float v2 = sin(r.y +t);
    float v3 = sin(r.x+r.y +t);
    float v4 = sin(sqrt(r.x*r.x+r.y*r.y) +1.7*t);
	float v = v1+v2+v3+v4;

	vec3 ret;
	//v *= 3.0;
	ret = vec3(sin(v), sin(v+0.5*PI), sin(v+1.0*PI));
	ret = 0.5 + 0.5*ret;

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
	outColor =(texture2D(textureVein, vTextCoord)*showTexture*0.1f + vec4(amb+diffuse+specular,1)*(1-showTexture)*(1-mutation)*0.7f + vec4(ret,1)*mutation*1.0f);
}
