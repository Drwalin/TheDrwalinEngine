
#ifndef MODEL_CPP
#define MODEL_CPP

#include <Model.h>
#include <Engine.h>

void Model::SetMaterialsToNode( irr::scene::ISceneNode * node )
{
	node->setMaterialFlag( irr::video::EMF_LIGHTING, false );
}

irr::scene::IAnimatedMesh * Model::GetMesh()
{
	return mesh;
}

bool Model::LoadCustomCollisionShapeFromObj( std::string objFileName )
{
	std::vector < btVector3 > vertices;
	std::vector < Model::Face > faces;
	
	std::ifstream file( objFileName );
	
	if( file.good() )
	{
		std::vector < AR<float,3> > vn;
		std::vector < AR<float,3> > v;
		std::string line, text;
		char sign;
		
		while( !file.eof() )
		{
			std::stringstream sstream;
			line = "";
			std::getline( file, line );
			
			if( line.size() )
			{
				if( line[line.size()-1] == 13 )
					line.resize( line.size()-1 );
			}
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
				
				vertices.resize( vertices.size() + 1 );
				vertices.back() = btVector3( v.back()[0], v.back()[1], v.back()[2] );
				
				if( v.size() == 1 )
				{
					minAABB = btVector3( v.back()[0], v.back()[1], v.back()[2] );
					maxAABB = minAABB;
				}
				else
				{
					for( int i = 0; i < 3; ++i )
					{
						if( minAABB.m_floats[i] > v.back()[i] )
							minAABB.m_floats[i] = v.back()[i];
						if( maxAABB.m_floats[i] < v.back()[i] )
							maxAABB.m_floats[i] = v.back()[i];
					}
				}
			}
			else if( text == "vn" )
			{
				vn.resize( vn.size() + 1 );
				sstream >> vn.back()[0];
				sstream >> vn.back()[1];
				sstream >> vn.back()[2];
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
					{
						temp.resize( temp.size() - 1 );
						break;
					}
				}
				
				
				if( temp.size() >= 3 )
				{
					for( unsigned i = 2; i < temp.size(); ++i )
					{
						btVector3 normal( vn[temp[0][2]][0], vn[temp[0][2]][1], vn[temp[0][2]][2] );
						normal += btVector3( vn[temp[i][2]][0], vn[temp[i][2]][1], vn[temp[i][2]][2] );
						normal += btVector3( vn[temp[i-1][2]][0], vn[temp[i-1][2]][1], vn[temp[i-1][2]][2] );
						
						faces.resize( faces.size() + 1 );
						faces.back().a = temp[0][0];
						faces.back().b = temp[i][0];
						faces.back().c = temp[i-1][0];
						
						btVector3 a, b;
						a = vertices[faces.back().a] - vertices[faces.back().b];
						b = vertices[faces.back().a] - vertices[faces.back().c];
						
						faces.back().normal = a.cross( b ).normalized();
						
						if( faces.back().normal.dot( normal ) < 0.0f )
						{
							faces.back().normal *= -1.0f;
						}
					}
				}
			}
		}
	}
	else
	{
		return false;
	}
	
	// calculate all physics data
	{
		collisionShapeData = std::shared_ptr<CustomCollisionShapeData>( new CustomCollisionShapeData );
		collisionShapeData->vertices = vertices;
		collisionShapeData->indices.resize( faces.size() * 3 );
		
		for( int i = 0; i < faces.size(); ++i )
		{
			collisionShapeData->indices[(i*3)+0] = faces[i].a;
			collisionShapeData->indices[(i*3)+1] = faces[i].b;
			collisionShapeData->indices[(i*3)+2] = faces[i].c;
		}
	}
	
	return true;
}

bool Model::LoadFromObj( Engine * engine, std::string objFileName )
{
	Destroy();
	
	if( engine == NULL )
		return false;
	
	mesh = engine->GetWindow()->sceneManager->getMesh( objFileName.c_str() );
	
	if( objFileName.size() < 5 )
		return false;
	
	this->engine = engine;
	
	std::string mtlFileName = objFileName;
	mtlFileName.resize( mtlFileName.size()-3 );
	mtlFileName += "mtl";
	
	LoadCustomCollisionShapeFromObj( objFileName );
	
	return true;
}

std::shared_ptr<CustomCollisionShapeData> Model::GetCustomCollisionShapeData( float acceptableDistanceToJoinVertices )
{
	/*
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
					if( btVector3( ((ALLEGRO_VERTEX*)&(vbo[i].vertices.front()))[j].x, ((ALLEGRO_VERTEX*)&(vbo[i].vertices.front()))[j].y, ((ALLEGRO_VERTEX*)&(vbo[i].vertices.front()))[j].z ).distance2( collisionShapeData->vertices[k] ) < squareAcceptableDistance )
					{
						break;
					}
				}
				
				collisionShapeData->indices.resize( collisionShapeData->indices.size() + 1 );
				collisionShapeData->indices.back() = k;
				if( k >= collisionShapeData->vertices.size() )
				{
					collisionShapeData->vertices.resize( collisionShapeData->vertices.size() + 1 );
					collisionShapeData->vertices.back() = btVector3( ((ALLEGRO_VERTEX*)&(vbo[i].vertices.front()))[j].x, ((ALLEGRO_VERTEX*)&(vbo[i].vertices.front()))[j].y, ((ALLEGRO_VERTEX*)&(vbo[i].vertices.front()))[j].z );
				}
			}
		}
	}
	*/
	return collisionShapeData;
}

btVector3 Model::GetInertia() const
{
	return minAABB + maxAABB;
}

void Model::NullCustomCollisionShape()
{
	collisionShapeData = NULL;
}

void Model::Destroy()
{
	if( mesh )
	{
		mesh->drop();
		mesh = NULL;
	}
	collisionShapeData = NULL;
}

Model::Model( const Model * other )
{
	mesh = NULL;
	if( other )
	{
		mesh = other->mesh;
		materials = other->materials;
		engine = other->engine;
		collisionShapeData = other->collisionShapeData;
		minAABB = other->minAABB;
		maxAABB = other->maxAABB;
	}
}

Model::Model()
{
	mesh = NULL;
	engine = NULL;
}

Model::~Model()
{
	Destroy();
}

#endif
 
