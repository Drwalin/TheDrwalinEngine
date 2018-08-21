
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

bool Model::LoadFromObj( Engine * engine, std::string fileName, int flags, btVector3 arg1, btVector3 origin )
{
	this->engine = engine;
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
	
	btVector3 barycenter(0,0,0);
	btVector3 aabbMin(0,0,0);
	btVector3 aabbMax(0,0,0);
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
			
			barycenter += btVector3( v.back()[0], v.back()[1], v.back()[2] );
			if( v.size() == 1 )
			{
				aabbMin = btVector3( v.back()[0], v.back()[1], v.back()[2] );
				aabbMax = aabbMin;
			}
			else
			{
				for( int i = 0; i < 3; ++i )
				{
					if( aabbMin.m_floats[i] > v.back()[i] )
						aabbMin.m_floats[i] = v.back()[i];
					if( aabbMax.m_floats[i] < v.back()[i] )
						aabbMax.m_floats[i] = v.back()[i];
				}
			}
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
											(ALLEGRO_VERTEX){ v[f[0][0]][0], v[f[0][0]][1], v[f[0][0]][2], vt[f[0][1]][0]*currentTextureWidth, (0.0f+vt[f[0][1]][1])*currentTextureHeight, c },\
											(ALLEGRO_VERTEX){ v[f[1][0]][0], v[f[1][0]][1], v[f[1][0]][2], vt[f[1][1]][0]*currentTextureWidth, (0.0f+vt[f[1][1]][1])*currentTextureHeight, c },\
											(ALLEGRO_VERTEX){ v[f[2][0]][0], v[f[2][0]][1], v[f[2][0]][2], vt[f[2][1]][0]*currentTextureWidth, (0.0f+vt[f[2][1]][1])*currentTextureHeight, c }\
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
			if( currentVBO->GetTexture() )
			{
				currentTextureWidth = currentVBO->GetTexture()->GetWidth();
				currentTextureHeight = currentVBO->GetTexture()->GetHeight();
			}
			else
			{
				currentTextureWidth = 1.0f;
				currentTextureHeight = 1.0f;
			}
		}
	}
	
	barycenter /= v.size();
	btVector3 scale(1,1,1), move(0,0,0);
	
	if( flags & Model::CENTER_BARYCENTER )
	{
		move = origin - barycenter;
	}
	else if( flags & Model::CENTER_AABB_MID )
	{
		move = origin - ( ( aabbMin + aabbMax ) * 0.5f );
	}
	
	if( flags & Model::RESCALE_TO_SIZE )
	{
		scale = arg1 / ( aabbMax - aabbMin );
	}
	else if( flags & Model::RESCALE_WITH_SCALE )
	{
		scale = arg1;
	}
	
	int i = 0;
	vbo.resize( vbos.size() );
	for( auto it = vbos.begin(); it != vbos.end(); ++it, ++i )
	{
		vbo[i] = it->second;
		it->second.Destroy();
		if( flags )
		{
			for( int j = 0; j < vbo[i].vertices.size(); ++j )
			{
				vbo[i].vertices[j].x += move.x();
				vbo[i].vertices[j].x *= scale.x();
				vbo[i].vertices[j].y += move.y();
				vbo[i].vertices[j].y *= scale.y();
				vbo[i].vertices[j].z += move.z();
				vbo[i].vertices[j].z *= scale.z();
			}
		}
		vbo[i].Generate();
	}
	
	return true;
}

//bool Model::loadFromCompressedFile( std::stringfileName );

CustomCollisionShapeData * Model::GetCustomCollisionShapeData( btScalar acceptableDistanceToJoinVertices )
{
	btScalar squareAcceptableDistance = acceptableDistanceToJoinVertices * acceptableDistanceToJoinVertices;
	if( collisionShapeData == NULL )
	{
		collisionShapeData = new CustomCollisionShapeData;
		
		int currentIndex = 0;
		
		for( int i = 0; i < vbo.size(); ++i )
		{
			for( int j = 0; j < vbo[i].vertices.size(); ++j, ++currentIndex )
			{
				int k;
				for( k = 0; k < collisionShapeData->vertices.size(); ++k )
				{
					if( btVector3( vbo[i].vertices[j].x, vbo[i].vertices[j].y, vbo[i].vertices[j].z ).distance2( collisionShapeData->vertices[k] ) < squareAcceptableDistance )
					{
						break;
					}
				}
				
				collisionShapeData->indices.resize( collisionShapeData->indices.size() + 1 );
				collisionShapeData->indices.back() = k;
				if( k >= collisionShapeData->vertices.size() )
				{
					collisionShapeData->vertices.resize( collisionShapeData->vertices.size() + 1 );
					collisionShapeData->vertices.back() = btVector3( vbo[i].vertices[j].x, vbo[i].vertices[j].y, vbo[i].vertices[j].z );
				}
			}
		}
	}
	return collisionShapeData;
}

void Model::NullCustomCollisionShape()
{
	collisionShapeData = NULL;
}

void Model::Destroy()
{
	for( int i = 0; i < vbo.size(); ++i )
		vbo[i].Destroy();
	vbo.clear();
	vbo.shrink_to_fit();
	collisionShapeData = NULL;
}

Model::Model()
{
	engine = NULL;
	collisionShapeData = NULL;
}

Model::~Model()
{
	Destroy();
}

#endif

