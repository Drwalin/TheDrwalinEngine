
#ifndef HEADER_H
#define HEADER_H

#include <Model.h>
#include <Engine.h>

#include <SmartPtr.h>

#include <sstream>

bool Convert( std::string objFile, std::string meshFile, bool scale = false, btVector3 size = btVector3(1,1,1) );
void ConvertMeshes( std::string convertMeshesListFile );

void LoadMeshes( std::string loadMeshesListFile, Engine * engine );

#endif

