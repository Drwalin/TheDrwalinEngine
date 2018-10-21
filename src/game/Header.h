
#ifndef HEADER_H
#define HEADER_H

#include <Model.h>
#include <Engine.h>

#include <SmartPtr.h>

#include <sstream>
#include <fstream>
#include <map>
#include <string>

bool Convert( std::string objFile, std::string meshFile, bool scale = false, btVector3 size = btVector3(1,1,1) );
void ConvertMeshes( std::string convertMeshesListFile );

void LoadMeshes( std::string loadMeshesListFile, Engine * engine );


// between every word need to be a white sign ( ' ', '\n', '\t' )
int LoadMapPause( Engine * engine, std::string mapFile );		// return 0 -> no errors


#endif

