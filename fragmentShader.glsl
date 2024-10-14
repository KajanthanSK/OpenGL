#version 330 core

out vec4 FragColor;

in vec3 outColor;

//uniform float xColor;
//uniform float yColor;
//uniform float zColor;
uniform vec3 color;

void main()
{
	//FragColor=vec4(xColor*outColor.x,yColor*outColor.y,zColor*outColor.z,1.0f);
	FragColor=vec4(color*outColor,1.0f);
}
