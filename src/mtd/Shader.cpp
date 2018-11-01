
#ifndef SHADER_CPP
#define SHADER_CPP

#include "Shader.h"

unsigned long long GetFileLength( std::ifstream  & file )
{
	if( file.good() )
	{
		unsigned long long beg, end;
		beg = file.tellg();
		file.seekg( 0, std::ios::end );
		end = file.tellg();
		file.seekg( beg, std::ios::beg );
		return end;
	}
	return 0;
}

int Shader::Load( const char * vertexPath, const char * geometryPath, const char * fragmentPath )
{
	if( this->program )
		return 311;
	// 1. Retrieve the vertex/fragment source code from filePath
	std::ifstream gShaderFile;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	 
	char *gShaderCode = NULL;
	char *vShaderCode = NULL; 
	char *fShaderCode = NULL;
	
	{
		// Open files
		if( geometryPath && *geometryPath )
			gShaderFile.open( geometryPath );
		vShaderFile.open( vertexPath );
		fShaderFile.open( fragmentPath );
		if( !vShaderFile.good() || !fShaderFile.good() || ( geometryPath && !gShaderFile.good() && *geometryPath ) )
		{
			fprintf( stderr, "\n ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ  v%i - f%i - g%i ", (int)vShaderFile.good(), (int)fShaderFile.good(), (int)gShaderFile.good() );
			return 1;
		}
		else
		{
			// Read file's buffer contents into streams
			unsigned long long int glen = 0;
			if( geometryPath && *geometryPath )
				glen = GetFileLength( gShaderFile );
			unsigned long long int vlen = GetFileLength( vShaderFile );
			unsigned long long int flen = GetFileLength( fShaderFile );
			
			if( geometryPath && *geometryPath )
			{
				gShaderCode = (char*)malloc( glen+1 );
				gShaderCode[glen] = 0;
			}
			vShaderCode = (char*)malloc( vlen+1 );
			vShaderCode[vlen] = 0;
			fShaderCode = (char*)malloc( flen+1 );
			fShaderCode[flen] = 0;
			
			if( geometryPath && *geometryPath )
				gShaderFile.read( gShaderCode, glen );
			vShaderFile.read( vShaderCode, vlen );
			fShaderFile.read( fShaderCode, flen );
			
			// close file handlers
			if( geometryPath && *geometryPath )
				gShaderFile.close( );
			vShaderFile.close( );
			fShaderFile.close( );
		}
	}
	
	// 2. Compile shaders
	GLuint geometry, vertex, fragment;
	GLint success;
	GLchar infoLog[512];
	memset( infoLog, 0, 512 );
	
	// Geometry Shader
	if( geometryPath && *geometryPath )
	{
		geometry = glCreateShader( GL_GEOMETRY_SHADER );
		glShaderSource( geometry, 1, &gShaderCode, NULL );
		glCompileShader( geometry );
		// Print compile errors if any
		glGetShaderiv( geometry, GL_COMPILE_STATUS, &success );
		if ( !success )
		{
			int a;
			glGetShaderInfoLog( geometry, 512, &a, infoLog );
			fprintf( stderr, "\n ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n %c", infoLog );
			return 2;
		}
	}
	
	// Vertex Shader
	vertex = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertex, 1, &vShaderCode, NULL );
	glCompileShader( vertex );
	// Print compile errors if any
	glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
	if ( !success )
	{
		int a;
		glGetShaderInfoLog( vertex, 512, &a, infoLog );
		fprintf( stderr, "\n ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %c", infoLog );
		return 3;
	}
	
	// Fragment Shader
	fragment = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragment, 1, &fShaderCode, NULL );
	glCompileShader( fragment );
	// Print compile errors if any
	glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
	if ( !success )
	{
		int a;
		glGetShaderInfoLog( fragment, 512, &a, infoLog );
		fprintf( stderr, "\n ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %c", infoLog );
		return 4;
	}
	
	// Shader program
	this->program = glCreateProgram();
	if( geometryPath && *geometryPath )
		glAttachShader( this->program, geometry );
	glAttachShader( this->program, vertex );
	glAttachShader( this->program, fragment );
	glLinkProgram( this->program );
	// Print linking errors if any
	glGetProgramiv( this->program, GL_LINK_STATUS, &success );
	if (!success)
	{
		glGetProgramInfoLog( this->program, 512, NULL, infoLog );
		fprintf( stderr, "\n ERROR::SHADER::PROGRAM::LINKING_FAILED\n %c", infoLog );
		return 5;
	}
	// Delete the shaders as they're linked into our program now and no longer necessery
	if( geometryPath && *geometryPath )
		glDeleteShader( geometry );
	glDeleteShader( vertex );
	glDeleteShader( fragment );
	
	if( geometryPath && *geometryPath )
		free( gShaderCode );
	free( vShaderCode );
	free( fShaderCode );
	
	fprintf( stderr, "\n Shader compiled!" );
	return 0;
}
	
void Shader::Use()
{
	glUseProgram( this->program );
}

unsigned int Shader::GetProgram()
{
	return this->program;
}

int Shader::GetUniformLocation( const char * name ) const
{
	return glGetUniformLocation( this->program, name );
}

int Shader::GetAttributeLocation( const char * name ) const
{
	return glGetAttribLocation( program, name );
}

void Shader::SetBool( const int location, bool value ) const
{		 
	glUniform1i( location, (int)value ); 
}
void Shader::SetInt( const int location, int value ) const
{ 
	glUniform1i( location, value ); 
}
void Shader::SetInt( const int location, const std::vector < int >& array ) const
{
	glUniform1uiv( location, array.size(), (unsigned int*)&array.front() );
}
void Shader::SetInt( const int location, const std::vector < unsigned int >& array ) const
{
	glUniform1uiv( location, array.size(), (unsigned int*)&array.front() );
}
void Shader::SetFloat( const int location, float value ) const
{ 
	glUniform1f( location, value ); 
}
void Shader::SetFloat( const int location, const std::vector < float >& array ) const
{
	glUniform1fv( location, array.size(), (float*)&array.front() );
}
void Shader::SetVec2( const int location, const glm::vec2 &value ) const
{
	glUniform2fv( location, 1, glm::value_ptr( value ) );
}
void Shader::SetVec3( const int location, const glm::vec3 &value ) const
{ 
	glUniform3fv( location, 1, glm::value_ptr( value ) );
}
void Shader::SetVec3( const int location, const std::vector < glm::vec3 >& array ) const
{
	glUniform3fv( location, array.size(), (float*)&array.front() );
}
void Shader::SetVec4( const int location, const glm::vec4 &value ) const
{ 
	glUniform4fv( location, 1, glm::value_ptr( value ) );
}
void Shader::SetMat2( const int location, const glm::mat2 &mat ) const
{
	glUniformMatrix2fv( location, 1, GL_FALSE, glm::value_ptr( mat ) );
}
void Shader::SetMat3( const int location, const glm::mat3 &mat ) const
{
	glUniformMatrix3fv( location, 1, GL_FALSE, glm::value_ptr( mat ) );
}
void Shader::SetMat4( const int location, const glm::mat4 &mat ) const
{
	glUniformMatrix4fv( location, 1, GL_FALSE, glm::value_ptr( mat ) );
}
void Shader::SetMat4( const int location, const std::vector < glm::mat4 >& array ) const
{
	glUniformMatrix4fv( location, array.size(), GL_FALSE, (float*)&array.front() );
}

Shader::Shader()
{
	this->program = 0;
}

Shader::~Shader()
{
	if( this->program )
	{
		glUseProgram( 0 );
		glDeleteProgram( this->program );
	}
	this->program = 0;
}

#endif

