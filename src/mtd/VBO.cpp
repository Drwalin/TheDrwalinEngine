
#ifndef VBO_CPP
#define VBO_CPP

#include <VBO.h>

void VBO::Draw()
{
	if( vertices.size() && indices.size() )
		al_draw_indexed_prim( &vertices.front(), NULL, texture ? texture->GetBitmapPtr() : NULL, &indices.front(), indices.size(), ALLEGRO_PRIM_TRIANGLE_LIST );
}

SmartPtr<Texture> VBO::GetTexture()
{
	return texture;
}

void VBO::Generate()
{
	indices.resize( vertices.size() );
	for( int i = 0; i < indices.size(); ++i )
		indices[i] = i;
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
	vertices.clear();
	indices.clear();
	vertices.shrink_to_fit();
	indices.shrink_to_fit();
}

void VBO::SetTexture( SmartPtr<Texture> texture )
{
	this->texture = texture;
}

VBO & VBO::operator = ( const VBO & other )
{
	DEBUG( "Copy" )
	this->vertices = other.vertices;
	this->indices = other.indices;
	this->texture = texture;
	return *this;
}

VBO::VBO( const VBO & other )
{
	DEBUG( "Construct from other" )
	this->vertices = other.vertices;
	this->indices = other.indices;
	this->texture = texture;
}

VBO::VBO()
{
}

VBO::~VBO()
{
	Destroy();
}

#endif

