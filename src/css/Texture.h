
#ifndef TEXTURE_H
#define TEXTURE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include <iostream>

class Texture
{
private:
	
	ALLEGRO_BITMAP * bitmap;
	int mode;
	
public:
	
	const static int NEAREST = 0;
	const static int LINEAR = 1;
	const static int MIPMAP = 2;
	
	bool Load( std::string file, int mode );
	
	ALLEGRO_BITMAP * GetBitmapPtr();
	void Use();		// unused
	
	void Destroy();
	
	Texture();
	~Texture();
};

#endif

