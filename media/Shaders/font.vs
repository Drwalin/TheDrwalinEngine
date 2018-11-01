#version 330 core

layout ( location = 0 ) in vec2 position;
layout ( location = 1 ) in vec2 sTexCoord;
layout ( location = 2 ) in vec4 sColor;

out vec4 color;
out vec2 texCoord;

void main()
{
	gl_Position = vec4(position,0.0,1.0);
	color = sColor;
	texCoord = sTexCoord;
}

