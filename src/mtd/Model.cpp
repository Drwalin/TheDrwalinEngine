
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

void Model::RescaleAndMove( std::map < std::string, std::vector < ALLEGRO_VERTEX > > & trianglesMaterial, btVector3 min, btVector3 max, bool scaleToSize, btVector3 size, Engine * engine, const std::map < std::string, std::string > & materialTexture )
{
	for( int i = 0; i < 3; ++i )
		if( min.m_floats[i] > max.m_floats[i] )
			std::swap( min.m_floats[0], max.m_floats[0] );
	
	btVector3 scale = size / ( max - min );
	btVector3 move = ( min + max ) * (-0.5f);
	
	float currentTextureWidth, currentTextureHeight;
	int i = 0;
	for( auto it = trianglesMaterial.begin(); it != trianglesMaterial.end(); ++it, ++i )
	{
		currentTextureWidth = currentTextureHeight = 1.0f;
		if( engine && materialTexture.size() )
		{
			SmartPtr<Texture> texture = engine->GetTexture( materialTexture.at( it->first ) );
			if( texture )
			{
				currentTextureWidth = texture->GetWidth();
				currentTextureHeight = texture->GetHeight();
				texture = NULL;
			}
		}
		
		for( int j = 0; j < it->second.size(); ++j )
		{
			it->second[j].x += move.x();
			it->second[j].y += move.y();
			it->second[j].z += move.z();
			if( scaleToSize )
			{
				it->second[j].x *= scale.x();
				it->second[j].y *= scale.y();
				it->second[j].z *= scale.z();
			}
			it->second[j].u *= currentTextureWidth;
			it->second[j].v *= currentTextureHeight;
		}
	}
}

bool Model::LoadObj( std::string objFileName, std::map < std::string, std::vector < ALLEGRO_VERTEX > > & trianglesMaterial, btVector3 & min, btVector3 & max )
{
	std::ifstream file( objFileName );
	
	if( file.good() )
	{
		std::vector < AR<float,3> > v;
		std::vector < AR<float,2> > vt;
		std::string line, text;
			char sign;
		
		std::vector < ALLEGRO_VERTEX > * currentMaterial = NULL;
		
		while( !file.eof() )
		{
			std::stringstream sstream;
			line = "";
			std::getline( file, line );
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
				
				if( v.size() == 1 )
				{
					min = btVector3( v.back()[0], v.back()[1], v.back()[2] );
					max = min;
				}
				else
				{
					for( int i = 0; i < 3; ++i )
					{
						if( min.m_floats[i] > v.back()[i] )
							min.m_floats[i] = v.back()[i];
						if( max.m_floats[i] < v.back()[i] )
							max.m_floats[i] = v.back()[i];
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
				std::vector < AR<int,3> > temp;
				temp.clear();
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
				
				if( temp.size() >= 3 )
				{
					int f[3][2];
					int i;
					const static ALLEGRO_COLOR c = al_map_rgb(255,255,255);
					f[0][0] = temp[0][0];
					f[0][1] = temp[0][1];
					for( i = 2; i < temp.size(); ++i )
					{
						f[1][0] = temp[i-1][0];
						f[1][1] = temp[i-1][1];
						f[2][0] = temp[i][0];
						f[2][1] = temp[i][1];
						
						currentMaterial->resize( currentMaterial->size() + 3 );
						
						currentMaterial->operator[]( currentMaterial->size()-3 ) = (ALLEGRO_VERTEX){ v[f[0][0]][0], v[f[0][0]][1], v[f[0][0]][2], vt[f[0][1]][0], (vt[f[0][1]][1]), c };
						currentMaterial->operator[]( currentMaterial->size()-2 ) = (ALLEGRO_VERTEX){ v[f[1][0]][0], v[f[1][0]][1], v[f[1][0]][2], vt[f[1][1]][0], (vt[f[1][1]][1]), c };
						currentMaterial->operator[]( currentMaterial->size()-1 ) = (ALLEGRO_VERTEX){ v[f[2][0]][0], v[f[2][0]][1], v[f[2][0]][2], vt[f[2][1]][0], (vt[f[2][1]][1]), c };
					}
				}
			}
			else if( text == "usemtl" )
			{
				text = "";
				sstream >> text;
				trianglesMaterial[text].resize( trianglesMaterial[text].size() + 1 );
				currentMaterial = &(trianglesMaterial[text]);
				currentMaterial->resize( currentMaterial->size() - 1 );
			}
		}
		
		return true;/////////////////////////////////////////////////////// check number of triangles
	}
	return false;
}

bool Model::LoadMtl( std::string mtlFileName, std::map < std::string, std::string > & materialTexture )
{
	std::ifstream file( mtlFileName );
	
	std::string path = mtlFileName;
	for( int i = path.size(); i > 0; --i )
	{
		if( path[i] == '/' )
		{
			path.resize( i+1 );
			break;
		}
	}
	
	if( file.good() )
	{
		std::string line, text, currentMaterial;
		
		while( !file.eof() )
		{
			std::stringstream sstream;
			line = "";
			std::getline( file, line );
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
				materialTexture[currentMaterial] = path + text;
			}
		}
		
		return true;
	}
	
	return false;
}

bool Model::SaveMeshFile( std::string meshFileName, bool containPhysicsBool, float friction, float restitution, std::map < std::string, std::vector < ALLEGRO_VERTEX > > & trianglesMaterial, std::map < std::string, std::string > & materialTexture )
{
	std::ofstream file( meshFileName, std::ofstream::binary );
	
	int temp;
	
	if( file.good() )
	{
		int numberOfVBOs = trianglesMaterial.size();
		file.write( (const char*)&numberOfVBOs, sizeof(int) );
		
		for( auto it = trianglesMaterial.begin(); it != trianglesMaterial.end(); ++it )
		{
			int stringSize = materialTexture[it->first].size();
			file.write( (const char*)&stringSize, sizeof(int) );
			file.write( (const char*)materialTexture[it->first].c_str(), stringSize );
			
			int numberOfVertices = it->second.size();
			file.write( (const char*)&numberOfVertices, sizeof(int) );
			for( int i = 0; i < numberOfVertices; ++i )
			{
				file.write( (const char*)&(it->second[i].x), sizeof(float) );
				file.write( (const char*)&(it->second[i].y), sizeof(float) );
				file.write( (const char*)&(it->second[i].z), sizeof(float) );
				file.write( (const char*)&(it->second[i].u), sizeof(float) );
				file.write( (const char*)&(it->second[i].v), sizeof(float) );
			}
		}
		
		char containPhysics = containPhysicsBool ? 'A' : 0;
		file.write( &containPhysics, sizeof(char) );
		
		if( containPhysicsBool )
		{
			CustomCollisionShapeData collisionShapeData;
			{
				float acceptableDistanceToJoinVertices = 0.0311 * 1.5;
				float squareAcceptableDistance = acceptableDistanceToJoinVertices * acceptableDistanceToJoinVertices;
				int currentIndex = 0;
				
				for( auto it = trianglesMaterial.begin(); it != trianglesMaterial.end(); ++it )
				{
					for( int j = 0; j < it->second.size(); ++j, ++currentIndex )
					{
						int k;
						for( k = 0; k < collisionShapeData.vertices.size(); ++k )
						{
							if( btVector3( it->second[j].x, it->second[j].y, it->second[j].z ).distance2( collisionShapeData.vertices[k] ) < squareAcceptableDistance )
							{
								break;
							}
						}
						
						collisionShapeData.indices.resize( collisionShapeData.indices.size() + 1 );
						collisionShapeData.indices.back() = k;
						if( k >= collisionShapeData.vertices.size() )
						{
							collisionShapeData.vertices.resize( collisionShapeData.vertices.size() + 1 );
							collisionShapeData.vertices.back() = btVector3( it->second[j].x, it->second[j].y, it->second[j].z );
						}
					}
				}
			}
			
			{
				int numberOfVertices = collisionShapeData.vertices.size();
				int numberOfIndices = collisionShapeData.indices.size();
				
				file.write( (const char*)&friction, sizeof(float) );
				file.write( (const char*)&restitution, sizeof(float) );
				file.write( (const char*)&numberOfVertices, sizeof(int) );
				file.write( (const char*)&numberOfIndices, sizeof(int) );
				
				for( int i = 0; i < numberOfVertices; ++i )
					file.write( (const char*)&(collisionShapeData.vertices[i]), sizeof(float) * 3 );
				
				file.write( (const char*)&(collisionShapeData.indices.front()), sizeof(int) * numberOfIndices );
			}
		}
		else
		{
			
		}
		
		return true;
	}
	return false;
}

bool Model::ConvertObjToMesh( std::string objFileName, bool containPhysicsBool, float friction, float restitution, bool scaleToSize, btVector3 size )
{
	if( objFileName.size() < 5 )
		return false;
	
	std::string mtlFileName = objFileName;
	std::string meshFileName = objFileName;
	
	meshFileName.resize( meshFileName.size()-3 );
	meshFileName += "phmesh";
	
	mtlFileName.resize( mtlFileName.size()-3 );
	mtlFileName += "mtl";
	
	btVector3 min, max;
	
	std::map < std::string, std::string > materialTexture;
	std::map < std::string, std::vector < ALLEGRO_VERTEX > > trianglesMaterial;
	
	Model::LoadMtl( mtlFileName, materialTexture );
	Model::LoadObj( objFileName, trianglesMaterial, min, max );
	
	Model::RescaleAndMove( trianglesMaterial, min, max, scaleToSize, size );
	
	return Model::SaveMeshFile( meshFileName, containPhysicsBool, friction, restitution, trianglesMaterial, materialTexture );
}

bool Model::LoadFromObj( Engine * engine, std::string objFileName )
{
	Destroy();
	
	if( objFileName.size() < 5 )
		return false;
	
	this->engine = engine;
	
	std::string mtlFileName = objFileName;
	mtlFileName.resize( mtlFileName.size()-3 );
	mtlFileName += "mtl";
	
	btVector3 min, max;
	
	std::map < std::string, std::string > materialTexture;
	std::map < std::string, std::vector < ALLEGRO_VERTEX > > trianglesMaterial;
	
	Model::LoadMtl( mtlFileName, materialTexture );
	if( Model::LoadObj( objFileName, trianglesMaterial, min, max ) == false )
	{
		DEBUG(1);
		return false;
	}
	
	Model::RescaleAndMove( trianglesMaterial, min, max, false, btVector3(), engine, materialTexture );
	
	vbo.resize( trianglesMaterial.size() );
	
	int i = 0;
	for( auto it = trianglesMaterial.begin(); it != trianglesMaterial.end(); ++it, ++i )
	{
		vbo[i].Destroy();
		if( engine )
		{
			vbo[i].SetTexture( engine->GetTexture( materialTexture[it->first] ) );
		}
		if( !vbo[i].GetTexture() )
			DEBUG( std::string( "Texture not exist" ) );
		vbo[i].vertices = it->second;
		vbo[i].Generate();
	}
	
	return true;
}

bool Model::loadFromMeshFile( Engine * engine, std::string meshFileName )
{
	Destroy();
	
	std::ifstream file( meshFileName, std::ifstream::binary );
	
	if( file.good() )
	{
		float currentTextureWidth, currentTextureHeight;
		
		int numberOfVBOs;
		file.read( (char*)&numberOfVBOs, sizeof(int) );
		
		vbo.resize( numberOfVBOs );
		
		ALLEGRO_COLOR color = al_map_rgb( 255, 255, 255 );
		
		for( int i = 0; i < numberOfVBOs; ++i )
		{
			currentTextureWidth = currentTextureHeight = 1.0f;
			
			int bytesPerTextureName;
			int numberOfVertices;
			
			file.read( (char*)&bytesPerTextureName, sizeof(int) );
			
			char textureFileNameWithPath[bytesPerTextureName+1];
			
			file.read( textureFileNameWithPath, bytesPerTextureName );
			textureFileNameWithPath[bytesPerTextureName] = 0;
			
			vbo[i].Destroy();
			
			if( engine )
			{	
				SmartPtr<Texture> texture = engine->GetTexture( textureFileNameWithPath );
				if( texture )
				{
					currentTextureWidth = texture->GetWidth();
					currentTextureHeight = texture->GetHeight();
				}
				vbo[i].SetTexture( texture );
			}
			
			file.read( (char*)&numberOfVertices, sizeof(int) );
				
			vbo[i].vertices.resize( numberOfVertices );
				
			for( int j = 0; j < numberOfVertices; ++j )
			{
				file.read( (char*)&(vbo[i].vertices[j].x), sizeof(float) );
				file.read( (char*)&(vbo[i].vertices[j].y), sizeof(float) );
				file.read( (char*)&(vbo[i].vertices[j].z), sizeof(float) );
				file.read( (char*)&(vbo[i].vertices[j].u), sizeof(float) );
				file.read( (char*)&(vbo[i].vertices[j].v), sizeof(float) );
				vbo[i].vertices[j].u *= currentTextureWidth;
				vbo[i].vertices[j].v *= currentTextureHeight;
				vbo[i].vertices[j].color = color;
			}	
			
			vbo[i].Generate();
		}
		
		if( !file )
		{
			Destroy();
			return false;
		}
		
		char containPhysics = 0;
		file.read( &containPhysics, sizeof(char) );
		
		if( containPhysics != 0 )
		{
			collisionShapeData = new CustomCollisionShapeData;
			
			float friction = 0.5f;
			float restitution = 0.0f;
			int numberOfVertices = 0;
			int numberOfIndices = 0;
			
			file.read( (char*)&friction, sizeof(float) );
			file.read( (char*)&restitution, sizeof(float) );
			file.read( (char*)&numberOfVertices, sizeof(int) );
			file.read( (char*)&numberOfIndices, sizeof(int) );
			
			if( !file )
			{
				collisionShapeData.Delete();
				return true;
			}
			
			collisionShapeData->friction = friction;
			collisionShapeData->restitution = restitution;
			collisionShapeData->vertices.resize( numberOfVertices );
			collisionShapeData->indices.resize( numberOfIndices );
			
			for( int i = 0; i < numberOfVertices; ++i )
			{
				file.read( (char*)&(collisionShapeData->vertices[i]), sizeof(float) * 3 );
				if( !file )
				{
					collisionShapeData.Delete();
					return true;
				}
			}
			
			file.read( (char*)&(collisionShapeData->indices.front()), sizeof(int) * numberOfIndices );
			if( !file )
			{
				collisionShapeData.Delete();
				return true;
			}
		}
		
		return true;
	}
	
	return false;
}

bool Model::LoadFromFile( Engine * engine, std::string fileName )
{
	Destroy();
	
	std::string extension;
	int i;
	for( i = fileName.size()-1; i > 0; --i )
		if( fileName[i-1] == '.' )
			break;
	
	if( i == 0 )
		return false;
	
	extension = (fileName.c_str() + i);
	
	if( extension == "obj" )
	{
		return LoadFromObj( engine, fileName );
	}
	else if( extension == "phmesh" )
	{
		return loadFromMeshFile( engine, fileName );
	}
	
	return false;
}

SmartPtr<CustomCollisionShapeData> Model::GetCustomCollisionShapeData( float acceptableDistanceToJoinVertices )
{
	if( !collisionShapeData )
	{
		float squareAcceptableDistance = acceptableDistanceToJoinVertices * acceptableDistanceToJoinVertices;
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

Model::Model( const Model * other )
{
	if( other )
	{
		vbo = other->vbo;
		engine = other->engine;;
	}
}

Model::Model()
{
	engine = NULL;
}

Model::~Model()
{
	Destroy();
}

#endif
 
