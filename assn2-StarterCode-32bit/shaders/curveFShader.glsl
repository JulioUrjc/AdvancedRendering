#version 330 core

in vec3 vcolor;
in vec3 pos;

out vec4 outColor;

void main()
{
    outColor = vec4(vcolor,1);
}
