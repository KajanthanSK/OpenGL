#version 330 core

out vec4 FragColor;

in vec3 outColor;
in vec2 textCoords;

uniform vec3 color;
uniform sampler2D containerTexture;
uniform sampler2D faceTexture;

uniform float alpha;

void main()
{
	//FragColor=vec4(color*outColor,1.0f);
	FragColor = mix(texture(containerTexture, textCoords), texture(faceTexture, textCoords), alpha) * vec4(outColor * color, 1.0f);		
}
