#version 330 core

in vec2 texCoord;
in vec4 color;

out vec4 FragColor;

// mipmaps:
uniform sampler2D texture0;

float GetDepth()
{
	return gl_FragCoord.z / gl_FragCoord.w;
}

void main()
{
	//FragColor = texture2D( texture3, texCoord );
	//FragColor = GetTextureColor();
	//FragColor = textureLod( texture0, texCoord, max( LengthSquare( dFdx( texCoord ) ), LengthSquare( dFdy( texCoord ) ) ) * 600 );
	FragColor = texture( texture0, texCoord );
	//FragColor = vec4( vec3( 1.0 ) * ( 50 - GetDepth() ) / 50, 1.0 );
    
	//FragColor *= color;
	//gl_FragDepth = ...
	//discard;
}

