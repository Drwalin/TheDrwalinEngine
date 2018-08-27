
#ifndef TEXTURE_CPP
#define TEXTURE_CPP

#include <Texture.h>

bool Texture::Load( std::string file, int mode )
{
	Destroy();
	this->file = file;
	this->mode = mode;
	al_set_new_bitmap_flags( ( (mode&Texture::LINEAR) ? (ALLEGRO_MIN_LINEAR|ALLEGRO_MAG_LINEAR) : 0 ) | ( (mode&Texture::MIPMAP) ? (ALLEGRO_MIPMAP) : 0 ) );
	bitmap = al_load_bitmap( file.c_str() );
	return bitmap != NULL;
}

int Texture::GetWidth()
{
	return ( bitmap ? float( al_get_bitmap_width( bitmap ) ) : 0.0 );
}

int Texture::GetHeight()
{
	return ( bitmap ? float( al_get_bitmap_height( bitmap ) ) : 0.0 );
}

ALLEGRO_BITMAP * Texture::GetBitmapPtr()
{
	return bitmap;
}

void Texture::Use()
{
	std::cerr << "\n Texture::Use() is not in use. Please use: Texture::GetBitmapPtr() to pass as argument to al_draw_*_prim";
}

void Texture::Destroy()
{
	file = "";
	if( bitmap )
		al_destroy_bitmap( bitmap );
	bitmap = NULL;
	mode = 0;
}

Texture::Texture( const Texture * other )
{
	if( other )
	{
		mode = other->mode;
		if( other->bitmap )
		{
			al_set_new_bitmap_flags( ( (mode&Texture::LINEAR) ? (ALLEGRO_MIN_LINEAR|ALLEGRO_MAG_LINEAR) : 0 ) | ( (mode&Texture::MIPMAP) ? (ALLEGRO_MIPMAP) : 0 ) );
			bitmap = al_clone_bitmap( other->bitmap );
		}
		else
			bitmap = NULL;
	}
	else
	{
		mode = Texture::MIPMAP | Texture::LINEAR;
		bitmap = NULL;
	}
}

Texture::Texture()
{
	mode = 0;
	bitmap = NULL;
}

Texture::~Texture()
{
	Destroy();
}

#endif

