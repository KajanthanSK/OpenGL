#version 330 core

out vec4 FragColor;

in vec3 outColor;
uniform vec3 color;

in vec2 textCoords;
uniform sampler2D texture1;

void main()
{
	//FragColor=vec4(color*outColor,1.0f);
	FragColor=texture(texture1,textCoords)*vec4(outColor*color,1.0f);
}
