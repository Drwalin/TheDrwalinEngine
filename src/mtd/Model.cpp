
#ifndef MODEL_CPP
#define MODEL_CPP

#include <Model.h>
#include <Engine.h>

void Model::Draw()
{
	for( int i = 0; i < vbo.size(); ++i )
	{
		vbo[i].Draw();
	}
}

bool Model::LoadFromObj( Engine * engine, std::string fileName )
{
	std::ifstream mtl, obj;
	std::string path = fileName;
	for( int i = path.size(); i > 0; --i )
	{
		if( path[i] == '/' )
		{
			path.resize( i+1 );
			break;
		}
	}
	
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
	
	if( mtl.good() )
	{
		while( !mtl.eof() )
		{
			std::stringstream sstream;
			line = "";
			std::getline( mtl, line );
			text = "";
			sstream << line;
			sstream.flush();
			sstream >> text;
			if( text == "newmtl" )
			{
				currentMaterial = "";
				sstream >> currentMaterial;
			}
			else if( text == "map_Kd" )
			{
				text = "";
				sstream >> text;
				vbos[currentMaterial].SetTexture( engine->GetTexture( path+text ) );
			}
		}
	}
	
	float currentTextureWidth, currentTextureHeight;
	while( !obj.eof() )
	{
		std::stringstream sstream;
		line = "";
		std::getline( obj, line );
		text = "";
		sstream << line;
		sstream.flush();
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
				temp.resize( 0 );
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
					temp.back()[0]--;
					temp.back()[1]--;
					temp.back()[2]--;
					if( temp.back()[0] < 0 || temp.back()[1] < 0 || temp.back()[2] < 0 )
						temp.resize( temp.size() - 1 );
				}

#define ADDTRIANGLE \
						currentVBO->AddTriangle\
										(\
											(ALLEGRO_VERTEX){ v[f[0][0]][0]*0.02f, v[f[0][0]][1]*0.02f, v[f[0][0]][2]*0.02f, vt[f[0][1]][0]*currentTextureWidth, (0.0f+vt[f[0][1]][1])*currentTextureHeight, c },\
											(ALLEGRO_VERTEX){ v[f[1][0]][0]*0.02f, v[f[1][0]][1]*0.02f, v[f[1][0]][2]*0.02f, vt[f[1][1]][0]*currentTextureWidth, (0.0f+vt[f[2][1]][1])*currentTextureHeight, c },\
											(ALLEGRO_VERTEX){ v[f[2][0]][0]*0.02f, v[f[2][0]][1]*0.02f, v[f[2][0]][2]*0.02f, vt[f[2][1]][0]*currentTextureWidth, (0.0f+vt[f[1][1]][1])*currentTextureHeight, c }\
										);
				if( temp.size() >= 3 )
				{
					int f[3][2];
					int i;
					const static ALLEGRO_COLOR c = al_map_rgb(255,255,255);
					switch( temp.size() )
					{
					case 3:
						f[0][0] = temp[0][0];
						f[0][1] = temp[0][1];
						f[1][0] = temp[1][0];
						f[1][1] = temp[1][1];
						f[2][0] = temp[2][0];
						f[2][1] = temp[2][1];
						ADDTRIANGLE
						break;
						
					case 4:
						f[0][0] = temp[0][0];
						f[0][1] = temp[0][1];
						f[1][0] = temp[1][0];
						f[1][1] = temp[1][1];
						f[2][0] = temp[2][0];
						f[2][1] = temp[2][1];
						ADDTRIANGLE
						f[1][0] = temp[3][0];
						f[1][1] = temp[3][1];
						ADDTRIANGLE
						break;
						
					default:
						for( i = 2; i < temp.size(); ++i )
						{
							f[0][0] = temp[i-2][0];
							f[0][1] = temp[i-2][1];
							f[1][0] = temp[i-1][0];
							f[1][1] = temp[i-1][1];
							f[2][0] = temp[i][0];
							f[2][1] = temp[i][1];
							
							ADDTRIANGLE
						}
					}
#undef ADDTRIANGLE					
					
					
				}
			}
		}
		else if( text == "usemtl" )
		{
			currentMaterial = "";
			sstream >> currentMaterial;
			currentVBO = &(vbos[currentMaterial]);
			currentTextureWidth = currentVBO->GetTexture()->GetWidth();
			currentTextureHeight = currentVBO->GetTexture()->GetHeight();
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
	std::vector < btVector3 > * vertices = new std::vector < btVector3 >;		// does it shouldn't be destroyed
	
	for( int i = 0; i < vbo.size(); ++i )
	{
		for( int j = 0; j < vbo[i].vertices.size(); ++j )
		{
			vertices->resize( vertices->size() + 1 );
			vertices->back() = btVector3( vbo[i].vertices[j].x, vbo[i].vertices[j].y, vbo[i].vertices[j].z );
		}
	}
	
	std::vector < int > * indices = new std::vector < int >;
	indices->resize( vertices->size() );
	for( int i = 0; i < indices->size(); ++i )
	{
		indices->operator[](i) = i;
	}
	
	auto * triangles = new btTriangleIndexVertexArray( indices->size() / 3, &(indices->front()), sizeof(int) * 3, vertices->size(), vertices->front().m_floats, sizeof(btVector3) );		// it should be destroyed after btCollisionShape
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

