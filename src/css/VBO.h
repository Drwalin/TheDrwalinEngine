
#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>		// ??
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include <cstring>

#include <vector>

#include <Debug.h>
#include <SmartPtr.h>

#include <Texture.h>
#include <Shader.h>

class Engine;

class VBO
{
private:
	
	std::vector<int>arrayOfShaderLocations;
	
	SmartPtr<Texture> texture;
	SmartPtr<Shader> shader;
	
	int vertexSize;
	bool generated;
	unsigned vaoID, vboID;
	std::vector < unsigned char > vertices;
	GLenum type;
	
	friend class Model;
	friend class TextPrinter;
	
public:
	
	std::vector < unsigned char > & AccessVertices();
	void SetAttribPointer( const int location, const unsigned int count, const GLenum type, const bool normalized, const unsigned int offset );		// offset in bytes
	void AddVertex( const void * bytes );
	void AddVertices( const void * ptr, const unsigned int vertices );
	void SetVertexSize( const unsigned int size );
	
	bool IsGenerated() const;
	void Generate( Engine * engine, const GLenum type );
	void ClearVertices();
	
	void SetTexture( SmartPtr<Texture> texture );
	SmartPtr<Texture> GetTexture();
	void SetShader( SmartPtr<Shader> shader );
	SmartPtr<Shader> GetShader();
	
	void Draw( const glm::mat4 & transformMatrix );
	
	void Destroy();
	
	VBO & operator = ( const VBO & other );
	VBO & operator = ( const SmartPtr<VBO> other );
	
	VBO( const VBO & other );
	VBO();
	~VBO();
};




#endif

