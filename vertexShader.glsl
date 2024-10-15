#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;
layout (location=2) in vec2 aTextCoords;

out vec3 outColor;
out vec2 textCoords;

uniform mat4 model;

void main()
{
	textCoords=aTextCoords;
	outColor=aColor;
	gl_Position=model * vec4(aPos,1.0f);
}
