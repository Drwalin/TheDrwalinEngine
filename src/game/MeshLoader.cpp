
#ifndef MESH_LOADER_CPP
#define MESH_LOADER_CPP

#include "Header.h"

void LoadMeshes( std::string loadMeshesListFile, Engine * engine )
{
	std::ifstream file( loadMeshesListFile );
	if( file.good() )
	{
		while( !file.eof() )
		{
			std::string  meshFile(""), customName("");
			std::getline( file, meshFile );
			std::getline( file, customName );
			if( file.eof() )
				break;
			
			if( meshFile.size() )
			{
				if( meshFile[meshFile.size()-1] == 13 )
					meshFile.resize( meshFile.size() - 1 );
			}
			if( customName.size() )
			{
				if( customName[customName.size()-1] == 13 )
					customName.resize( customName.size() - 1 );
			}
			
			SmartPtr<Model> ptr = engine->LoadModel( meshFile );
			assert( (bool)ptr );
			if( ptr )
			{
				if( engine->SetCustomModelName( customName, ptr ) == false )
				{
					DEBUG( std::string("Couldn't set custom mesh: \"") + customName + "\"" );
				}
			}
			else
			{
				DEBUG( std::string("Couldn't load mesh: \"") + meshFile + "\"" );
			}
		}
	}
}

#endif

