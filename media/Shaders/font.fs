#version 330 core

in vec2 texCoord;
in vec4 color;

out vec4 FragColor;

uniform sampler2D texture0;

void main()
{
	FragColor = vec4( textureLod( texture0, vec2( texCoord.x, 1.0 - texCoord.y ), 0 ).xyz * color, 1.0 );
	if( FragColor.x < 0.01 && FragColor.y < 0.01 && FragColor.z < 0.01 )
		FragColor.w = 0.0;
}

