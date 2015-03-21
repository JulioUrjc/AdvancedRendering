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
const float NUMCELL = 15.0;

vec2 Hash2(vec2 p){
	float r = 523.0*sin(iGlobalTime/900000+dot(p, vec2(53.3158, 43.6143)));
	return vec2(fract(15.32354 * r), fract(17.25865 * r));
}

float minDistance(in vec2 p){
	p *= NUMCELL;
	float minD = 2.0;
	float minD2 = 2.0;
	float minD3 = 2.0;
	float minAux=0;

	for (int i = -1; i <= 1; i++){
		for (int j = -1; j <= 1; j++){
			vec2 auxP = floor(p) + vec2(i, j);
			vec2 hashP = Hash2(mod(auxP, NUMCELL));
			//Euclidea distance
			//minD = min(minD, sqrt(pow((p.x-auxP.x-hashP.x),2)+pow((p.y-auxP.y-hashP.y),2)));
			minD = min(minD, length(p - auxP - hashP)); //mod to do a circular texture
			//Manhattan distance
			//minD = min(minD, (abs(p.x-auxP.x-hashP.x)+abs(p.y-auxP.y-hashP.y)));
			////Teselas mode
			minD2 = min(minD3, length(p - auxP - hashP));
			
			if (minD2<minD){
				minAux= minD2;
				minD2=minD;
				minD=minAux;
			} 
		}
	}
	return minD;               //Celular
	//return minD2-minD;         //Teselas
	//return minD3-minD2-minD;   //Teselas Distantes
}

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

	vec2 uv = gl_FragCoord.xy / vec2(640.0,480.0);
	float minD = minDistance(uv);

	//Texture
	//outColor =(texture2D(textureVein, vTextCoord)*showTexture*0.8f + vec4(amb+diffuse+specular,1)*(1-showTexture)*(1-mutation)*0.7f + vec4(ret,1)*mutation*1.0f);
	//outColor =texture(textureVein, vTextCoord)*showTexture*0.8f + vec4(amb+diffuse+specular,1)*(1-showTexture)*(1-mutation)*0.7f + vec4(ret,1)*mutation*0.4f;
	outColor = vec4(minD*.93, minD*.23, minD*.13,1.0)*showTexture*0.3f + vec4(amb+diffuse+specular,1)*(1-mutation)*0.7f + vec4(ret,1)*mutation*0.4f;
}
