
#ifndef MODEL_CPP
#define MODEL_CPP

#include <Model.h>
#include <Engine.h>

void Model::Draw()
{
	for( int i = 0; i < vbo.size(); ++i )
		vbo[i].Draw();
}

bool Model::LoadFromObj( Engine * engine, std::string fileName )
{
	std::ifstream mtl, obj;
	
	{
		std::string fileMtlName = fileName;
		fileMtlName[fileMtlName.size()-3] = 'm';
		fileMtlName[fileMtlName.size()-2] = 't';
		fileMtlName[fileMtlName.size()-1] = 'l';
		
		obj.open( fileName );
		mtl.open( fileMtlName );
		if( !obj.good() )
			return false;
	}
	
	std::map < std::string, VBO > vbos;
	std::vector < AR<float,3> > v;
	std::vector < AR<float,2> > vt;
	
	char sign;
	std::string line, text, currentMaterial;
	VBO * currentVBO = NULL;
	std::stringstream sstream;
	
	while( !obj.eof() )
	{
		std::getline( obj, line );
		sstream.clear();
		sstream >> text;
		
		if( text == "v" )
		{
			v.resize( v.size() + 1 );
			sstream >> v.back()[0];
			sstream >> v.back()[1];
			sstream >> v.back()[2];
		}
		else if( text == "vt" )
		{
			vt.resize( vt.size() + 1 );
			sstream >> vt.back()[0];
			sstream >> vt.back()[1];
		}
		else if( text == "f" )
		{
			if( currentVBO )
			{
				std::vector < AR<int,3> > temp;
				while( !sstream.eof() )
				{
					temp.resize( temp.size() + 1 );
					temp.back()[0] = -1;
					temp.back()[1] = -1;
					temp.back()[2] = -1;
					sstream >> temp.back()[0];
					sstream >> sign;
					sstream >> temp.back()[1];
					sstream >> sign;
					sstream >> temp.back()[2];
					if( temp.back()[0] == -1 || temp.back()[1] == -1 || temp.back()[2] == -1 )
						temp.resize( temp.size() - 1 );
				}
				
				if( temp.size() )
				{
					
					int f[3][2];
					f[0][0] = temp[0][0];
					f[0][1] = temp[0][1];
					for( int i = 2; i < temp.size(); ++i )
					{
						f[1][0] = temp[i-1][0];
						f[1][1] = temp[i-1][1];
						f[2][0] = temp[i][0];
						f[2][1] = temp[i][1];
						
						const static ALLEGRO_COLOR c = al_map_rgb(255,255,255);
						currentVBO->AddTriangle
										(
											(ALLEGRO_VERTEX){ v[f[0][0]][0], v[f[0][0]][1], v[f[0][0]][2], vt[f[0][1]][0], vt[f[0][1]][1], c },
											(ALLEGRO_VERTEX){ v[f[1][0]][0], v[f[1][0]][1], v[f[1][0]][2], vt[f[1][1]][0], vt[f[2][1]][1], c },
											(ALLEGRO_VERTEX){ v[f[2][0]][0], v[f[2][0]][1], v[f[2][0]][2], vt[f[2][1]][0], vt[f[1][1]][1], c }
										);
					}
				}
			}
		}
		else if( text == "usemtl" )
		{
			sstream >> currentMaterial;
			vbos[currentMaterial] = VBO();
			currentVBO = &(vbos[currentMaterial]);
		}
	}
	
	if( mtl.good() )
	{
		while( !mtl.eof() )
		{
			std::getline( mtl, line );
			sstream.clear();
			sstream >> text;
			
			if( text == "newmtl" )
			{
				sstream >> currentMaterial;
			}
			else if( text == "map_Kd" )
			{
				sstream >> text;
				vbos["currentMaterial"].SetTexture( engine->GetTexture( text ) );
			}
		}
	}
	
	int i = 0;
	vbo.resize( vbos.size() );
	for( auto it = vbos.begin(); it != vbos.end(); ++it, ++i )
	{
		vbo[i] = it->second;
		it->second.Destroy();
		vbo[i].Generate();
	}
	
	return true;
}

//bool Model::loadFromCompressedFile( std::stringfileName );

btCollisionShape * Model::MakeConvexCollisionShape()
{
	std::map < btVector3, int > pointsMap;
	std::vector < btVector3 > points;		// does it shouldn't be destroyed
	
	for( int i = 0; i < vbo.size(); ++i )
	{
		for( int j = 0; j < vbo[i].vertices.size(); ++j )
		{
			pointsMap[ btVector3( vbo[i].vertices[j].x, vbo[i].vertices[j].y, vbo[i].vertices[j].z ) ] += 1;
		}
	}
	
	points.resize( pointsMap.size() );
	int i = 0;
	for( auto it = pointsMap.begin(); it != pointsMap.end(); ++it, ++i )
	{
		points[i] = it->first;
	}
	
	if( points.size() > 0 )
		return new btConvexHullShape( &points.front().x(), points.size(), sizeof(btVector3) );
	return NULL;
}

btCollisionShape * Model::MakeStaticTriangleCollisionShape()
{
	std::vector < btVector3 > vertices;		// does it shouldn't be destroyed
	
	for( int i = 0; i < vbo.size(); ++i )
	{
		for( int j = 0; j < vbo[i].vertices.size(); ++j )
		{
			vertices.resize( vertices.size() + 1 );
			vertices.back() = btVector3( vbo[i].vertices[j].x, vbo[i].vertices[j].y, vbo[i].vertices[j].z );
		}
	}
	
	std::vector < int > indices;
	indices.resize( vertices.size() );
	for( int i = 0; i < indices.size(); ++i )
	{
		indices[i] = i;
	}
	
	btTriangleIndexVertexArray * triangles = new btTriangleIndexVertexArray( indices.size() / 3, &(indices.front()), sizeof(int) * 3, vertices.size(), vertices.front().m_floats, sizeof(btVector3) );		// it should be destroyed after btCollisionShape
	btCollisionShape * shape = new btBvhTriangleMeshShape( triangles, true, true );
	//delete triangles;////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return shape;
	
	/*	
	s32 * indices;
	btScalar * vertices;
	physicsMesh = new btTriangleIndexVertexArray(mesh->indices.size() / 3, indices, sizeof(s32) * 3, mesh->vertices.size(), vertices, sizeof(btScalar) * 3);
	physicsShape = new btBvhTriangleMeshShape(physicsMesh, true, true);
	*/
}

void Model::Destroy()
{
	for( int i = 0; i < vbo.size(); ++i )
		vbo[i].Destroy();
	vbo.clear();
	vbo.shrink_to_fit();
}

Model::Model()
{
}

Model::~Model()
{
	Destroy();
}

#endif

