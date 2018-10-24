#version 330 core

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec2 textureCoord;
layout ( location = 2 ) in vec4 colorIn;

out vec2 texCoord;
out vec4 color;

uniform mat4 transformMatrix;

void main()
{
	//gl_Position = projection * view * model * vec4(position,1.0);
	gl_Position = transformMatrix * vec4(position,1.0);
	texCoord = textureCoord;
	color = colorIn;
}

