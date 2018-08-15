
#ifndef VBO_CPP
#define VBO_CPP

#include <VBO.h>

void VBO::Draw()
{
//	if( vertices.size() && indices.size() )
//		al_draw_indexed_prim( &vertices.fron(), NULL, texture ? texture->GetBitmapPtr() : NULL, &indices.front(), indices.size(), ALLEGRO_PRIM_TRIANGLE_LIST );
	if( vertices.size() >= 3 )
		al_draw_prim( &vertices.front(), NULL, texture ? texture->GetBitmapPtr() : NULL, 0, vertices.size(), ALLEGRO_PRIM_TRIANGLE_LIST );
}

void VBO::Generate()
{
	std::cerr << "\n VBO::Generate() is not in use";
}

void VBO::AddTriangle( ALLEGRO_VERTEX a, ALLEGRO_VERTEX b, ALLEGRO_VERTEX c )
{
	vertices.resize( vertices.size() + 3 );
	vertices[vertices.size()-3] = a;
	vertices[vertices.size()-2] = b;
	vertices[vertices.size()-1] = c;
}

void VBO::Destroy()
{
	texture = NULL;
	vertices.clear();
//	indices.clear();
	vertices.shrink_to_fit();
//	indices.shrink_to_fit();
}

void VBO::SetTexture( Texture * texture )
{
	this->texture = texture;
}

VBO::VBO()
{
	texture = NULL;
}

VBO::~VBO()
{
	Destroy();
}

#endif

