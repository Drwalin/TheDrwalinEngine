
#ifndef VBO_CPP
#define VBO_CPP

#include <VBO.h>
#include <Engine.h>

std::vector < unsigned char > & VBO::AccessVertices()
{
	return vertices;
}

void VBO::SetAttribPointer( const int location, const unsigned int count, const GLenum type, const bool normalized, const unsigned int offset )
{
	if( generated )
	{
		glBindVertexArray( vaoID );
		glBindBuffer( GL_ARRAY_BUFFER, vboID );
		glEnableVertexAttribArray( location );
		glVertexAttribPointer( location, count, type, normalized, vertexSize, (void*)offset );
	}
}

void VBO::AddVertex( const void * bytes )
{
	if( bytes )
	{
		unsigned long long oldsize = vertices.size();
		vertices.resize( oldsize + vertexSize );
		memcpy( &(vertices[oldsize]), bytes, vertexSize );
	}
}

void VBO::AddVertices( const void * ptr, const unsigned int vertices )
{
	if( ptr )
	{
		unsigned long long oldsize = this->vertices.size();
		this->vertices.resize( oldsize + (vertexSize*vertices) );
		memcpy( &(this->vertices[oldsize]), ptr, (vertexSize*vertices) );
	}
}

void VBO::SetVertexSize( const unsigned int size )
{
	vertexSize = size;
}

bool VBO::IsGenerated() const
{
	return generated;
}

void VBO::Generate( Engine * engine, const GLenum type )
{
	if( generated )
		Destroy();
	
	glGenVertexArrays( 1, &vaoID );
	glBindVertexArray( vaoID );
	
	glGenBuffers( 1, &vboID );
	glBindBuffer( GL_ARRAY_BUFFER, vboID );
	glBufferData( GL_ARRAY_BUFFER, vertices.size()*sizeof(unsigned char), &(vertices.front()), GL_STATIC_DRAW );
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	
	this->type = type;
	
	generated = true;
	
	vertices.shrink_to_fit();
	
	if( engine )
	{
		SmartPtr<Shader> core = engine->GetShader( "Core" );
		if( core )
		{
			this->SetAttribPointer( core->GetAttributeLocation( "position" ), 3, GL_FLOAT, false, 0 );
			this->SetAttribPointer( core->GetAttributeLocation( "textureCoord" ), 2, GL_FLOAT, false, 3 * sizeof(float) );
			this->SetAttribPointer( core->GetAttributeLocation( "colorIn" ), 4, GL_UNSIGNED_BYTE, false, 5 * sizeof(float) );
		}
	}
}

void VBO::ClearVertices()
{
	vertices.clear();
	vertices.shrink_to_fit();
}

void VBO::SetTexture( SmartPtr<Texture> texture )
{
	this->texture = texture;
}

SmartPtr<Texture> VBO::GetTexture()
{
	return texture;
}

void VBO::SetShader( SmartPtr<Shader> shader )
{
	this->shader = shader;
}

SmartPtr<Shader> VBO::GetShader()
{
	return shader;
}

void VBO::Draw( const glm::mat4 & transformMatrix )
{
	if( generated )
	{
		glBindVertexArray( vaoID );
		glBindBuffer( GL_ARRAY_BUFFER, vboID );
		
		if( shader )
		{
			if( arrayOfShaderLocations.size() < 2 )
			{
				arrayOfShaderLocations.resize( 2 );
				arrayOfShaderLocations[0] = -1;
				arrayOfShaderLocations[1] = -1;
			}
			
			shader->Use();
			
			if( texture )
			{
				texture->Use( 0 );
				if( arrayOfShaderLocations[0] < 0 )
					arrayOfShaderLocations[0] = shader->GetUniformLocation( ( std::string("texture0") ).c_str() );
				shader->SetInt( arrayOfShaderLocations[0], 0 );
			}
			else
			{
				DEBUG( "No texture" );
			}
			
			if( arrayOfShaderLocations[1] < 0 )
			{
				arrayOfShaderLocations[1] = shader->GetUniformLocation( "transformMatrix" );
			}
			
			shader->SetMat4( arrayOfShaderLocations[1], transformMatrix );		// projection * view * model
		}
		else
		{
			DEBUG( "No shader" );
		}
		
		glDrawArrays( type, 0, vertices.size() );
		
		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glUseProgram( 0 );
	}
}

void VBO::Destroy()
{
	arrayOfShaderLocations.clear();
	arrayOfShaderLocations.shrink_to_fit();
	if( generated )
	{
		glDeleteVertexArrays( 1, &vaoID );
		glDeleteBuffers( 1, &vboID );
		generated = false;
		vaoID = 0;
		vboID = 0;
	}
}

VBO & VBO::operator = ( const VBO & other )
{
	this->Destroy();
	
	this->texture = other.texture;
	this->vertexSize = other.vertexSize;
	this->vertices = other.vertices;
	
	vertices.shrink_to_fit();
}

VBO & VBO::operator = ( const SmartPtr<VBO> other )
{
	this->Destroy();
	
	this->texture = other->texture;
	this->vertexSize = other->vertexSize;
	this->vertices = other->vertices;
	
	vertices.shrink_to_fit();
}

VBO::VBO( const VBO & other )
{
	vertexSize = 5 * sizeof(float);
	generated = false;
	vaoID = 0;
	vboID = 0;
	type = GL_TRIANGLES;
	this->operator=( other );
}

VBO::VBO()
{
	vertexSize = 5 * sizeof(float);
	generated = false;
	vaoID = 0;
	vboID = 0;
	type = GL_TRIANGLES;
}

VBO::~VBO()
{
	Destroy();
	vertices.clear();
}

#endif

