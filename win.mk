
BINARYPATH = ./bin-win
BULLETINCLUDEPATH = C:/mingw-w64/mingw64/include/bullet
BULLETLIBPATH = C:\mingw-w64\mingw64\lib\bullet
STANDARDINCLUDEPATH = C:\mingw-w64\mingw64\include

CFLAGS += -L$(BULLETLIBPATH) -I$(BULLETINCLUDEPATH) -I$(STANDARDINCLUDEPATH)
LIBS += "C:/mingw-w64/mingw64/lib/allegro/liballegro_monolith.dll.a" "C:\mingw-w64\mingw64\lib\glew\glew64.lib" -lopengl32 -lglu32

EXECUTABLE = main.exe


