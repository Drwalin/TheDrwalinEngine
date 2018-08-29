

HEADERPATH = ./src/css
SOURCEPATH = ./src/mtd
LIBRARYPATH = ./src/lib
BINARYPATH = ./bin
BULLETINCLUDEPATH = /usr/local/include/bullet
LIBPNGPATH = ./lib

SOURCES_ = Engine.cpp Model.cpp EngineRayTraceData.cpp BasicWindow.cpp Keyboard.cpp Camera.cpp EventResponser.cpp StringToEnter.cpp TextPrinter.cpp Event.cpp Window.cpp Main.cpp World.cpp Object.cpp Texture.cpp VBO.cpp CustomCollisionData.cpp CollisionShapeManager.cpp SmartPtr.cpp
OBJECTS_ = $(SOURCES_:.cpp=.o)
SOURCES = $(addprefix $(SOURCEPATH)/,$(SOURCES_))
OBJECTS = $(addprefix $(BINARYPATH)/,$(OBJECTS_))


GAMESOURCEPATH = ./src/game

GAMESOURCES_ = Converter.cpp MeshLoader.cpp Character.cpp Player.cpp
GAMEOBJECTS_ = $(GAMESOURCES_:.cpp=.o)
GAMESOURCES = $(addprefix $(GAMESOURCEPATH)/,$(GAMESOURCES_))
GAMEOBJECTS = $(addprefix $(BINARYPATH)/,$(GAMEOBJECTS_))

OBJECTS += $(GAMEOBJECTS)


EXECUTABLE = main

CFLAGS = -m64 -s -Ofast -std=c++11 -I$(HEADERPATH) -I$(BULLETINCLUDEPATH) -I$(LIBRARYPATH) -w
LIBS = -lm -lpthread -lSM -lICE -lX11 -lXext -lXcursor -lXpm -lXi -lXinerama -lXrandr -lGL -lGLU -lpng16 -lz -lpulse-simple -lpulse -lasound -lopenal -lfreetype -lallegro_monolith-static -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath
CC = g++


$(BINARYPATH)/%.o: $(SOURCEPATH)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINARYPATH)/%.o: $(GAMESOURCEPATH)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -L$(LIBPNGPATH) -o $@ $^ $(LIBS)



.PHONY : clean

clean:
	rm -f main
	rm -f $(BINARYPATH)/*.o



