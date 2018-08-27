
#ifndef VBO_H
#define VBO_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include <vector>

#include <Debug.h>
#include <SmartPtr.h>

#include <Texture.h>

class VBO
{
private:
	
	SmartPtr<Texture> texture;
	std::vector < ALLEGRO_VERTEX > vertices;
	std::vector < int > indices;
	
	friend class Model;
	
public:
	
	void AddTriangle( ALLEGRO_VERTEX a, ALLEGRO_VERTEX b, ALLEGRO_VERTEX c );
	
	void Draw();
	void Generate();
	void Destroy();
	void SetTexture( SmartPtr<Texture> texture );
	SmartPtr<Texture> GetTexture();
	
	VBO & operator = ( const VBO & other );
	
	VBO( const VBO & other );
	VBO();
	~VBO();
};




#endif

