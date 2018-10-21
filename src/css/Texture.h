
#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>		// ??
#include <GL/gl.h>
#include <GL/glu.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_opengl.h>

#include <Debug.h>

#include <SmartPtr.h>

#include <string>

class Texture
{
private:
	
	ALLEGRO_BITMAP * bitmap;
	int mode;
	unsigned int textureID;
	
public:
	std::string file;
	
	const static int NEAREST = 0;
	const static int LINEAR = 1;
	const static int MIPMAP = 2;
	
	bool Load( std::string file, int mode );
	int GetWidth();
	int GetHeight();
	
	ALLEGRO_BITMAP * GetBitmapPtr();
	void Use( int id = 0 );
	
	void Destroy();
	
	Texture( SmartPtr<Texture> other );
	Texture();
	~Texture();
};

#endif

